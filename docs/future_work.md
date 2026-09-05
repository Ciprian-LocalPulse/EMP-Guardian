# Future Work / Research Roadmap

This document tracks directions that are worth exploring but are **not**
implemented yet. It exists so research context isn't lost, without
expanding the scope of the current firmware/software beyond what's been
built and tested. Nothing here should be read as a claim about the
current system's capabilities — see [`MANIFESTO.md`](../MANIFESTO.md) for
the honest status of what exists today.

## Layered view of "smarter" detection

The literature on real-time automated threat detection spans several
families of approaches (hybrid anomaly-detection + classifier pipelines,
LSTM/autoencoder sequence models, reinforcement-learning-driven adaptive
thresholds, classical ML ensembles, timed-automata formal methods,
feature-extraction + cascaded classifiers, edge/cloud low-latency
architectures). Almost none of these are appropriate to run where
EMP-Guardian's firmware actually lives:

- **`firmware/` (MCU, <10 µs decision budget):** stays a lightweight
  adaptive-threshold + rate-of-rise state machine (what's implemented
  today in `emp_detector.c`). This is deliberate, not a limitation to
  fix — a microcontroller with a microsecond-scale latency budget cannot
  run an LSTM or a Deep-Q-Learning policy in the loop. This tier maps
  closest to the "classical/formal-methods" end of the literature
  (threshold + time-window + hysteresis is itself a small state machine,
  conceptually adjacent to the timed-automata approach).
- **`software/monitor` (runs on a host/edge machine, not latency-critical
  at the microsecond scale):** a reasonable place to eventually add a
  classical ML ensemble (Random Forest / Gradient Boosting) or a simple
  autoencoder — not on the raw ≥1 MSPS ADC stream, but on the *event
  log* (timestamps, baseline drift, confirmation counts, false-positive
  patterns) to help distinguish recurring nuisance triggers from genuine
  events across multiple deployed units. Not started.
- **Fleet-level / multi-node correlation:** if multiple EMP-Guardian
  units report to the same `software/api` backend, there may be value in
  correlating events across nodes (e.g., a geomagnetic storm affecting
  many units simultaneously vs. a single unit's local switching noise).
  Not started; would live in `software/api`, not firmware.

## Shielding-effectiveness modeling

The reflection/absorption/multiple-reflection framing already matches
what's documented in
[`docs/theory_of_operation.md`](theory_of_operation.md) and the honesty
notes in the calculator's README (SE = A + R, with an explicit warning
about the omitted multiple-reflection term B when A < ~15 dB). No new
model is needed here today; the existing SE = A + R approximation and
its documented limitation already reflect the standard reference
(Ott, *Electromagnetic Compatibility Engineering*). A possible future
addition is implementing the B correction term for the thin-shield /
low-frequency case, once real measured data justifies it (see
`docs/compliance.md` for why lab validation, not a better formula alone,
is the actual gating factor for institutional deployments).

## Explicitly out of scope for this document

Per [`CONTRIBUTING.md`](../CONTRIBUTING.md)'s core principle, none of the
above extends to modeling, generating, amplifying, or directing an EMP —
every item here is about detecting, classifying, or shielding against
one.
