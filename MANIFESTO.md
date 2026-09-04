# EMP-Guardian Manifesto

**Author:** Ciprian Ștefan Pleșca — Independent Researcher, Romania
**Version:** 1.0 — September 2026

## Preamble

This document is not a technical specification — those already exist in [`docs/`](docs/). It is a position statement: why the EMP-Guardian project was started, what its current epistemic status is, and what kind of scientific contribution it aims to be.

We write it because a technical repository, however well structured, does not communicate its real motivation and limitations on its own. A researcher evaluating this project has the right to know exactly what they are looking at: an experimentally validated artifact, or an open conceptual framework awaiting collective validation. As of this writing, the honest answer is the latter.

## Positioning

Protecting critical infrastructure against electromagnetic pulse effects — whether geomagnetic or industrial in origin — has historically been addressed in two ways: through access-restricted military standards (MIL-STD-188-125, MIL-STD-461), or through proprietary commercial solutions with no public documentation. Between these two extremes lies a gap: the absence of an **open, reproducible, and auditable** framework that independent researchers, small universities, or resource-constrained infrastructure operators can study, adapt, and improve.

EMP-Guardian aims to occupy that space — not as a replacement for existing standards, but as a fully documented reference implementation that translates the principles behind those standards into a form anyone can compile, test, and critique.

## Epistemic status of the project

In the spirit of intellectual honesty that any published technical work should uphold, we state the current status explicitly:

- **The detection algorithm** (v2: dual-criterion state machine — adaptive amplitude threshold combined with a rate-of-rise/slope criterion, see `docs/theory_of_operation.md`) is implemented and unit-tested against six scenarios, including explicit rejection of slow high-amplitude events (e.g. industrial switching) that a naive threshold-only detector would false-trigger on. Its logical correctness is verifiable; its real-world performance is not — yet.
- **Threshold, margin, slope, and hysteresis parameters** are initial design values, not results of experimental calibration. Any implementer must treat them as such.
- **Hardware components** (schematic, PCB, shielded enclosure) exist at the specification and bill-of-materials level, not as validated manufacturing files.
- **No laboratory measurement** has yet been performed and published in this repository.

This section is not a weakness to hide — it is the correct starting point for any external evaluation. A project that states its limitations precisely is more scientifically useful than one that leaves the impression of validation that does not exist.

## What "complete" means for this project

Completeness, in the sense pursued here, does not mean the absence of gaps — any technical system at an early stage has gaps. It means:

1. **Traceability** — every technical claim can be traced back to its source (code, a cited standard, or explicitly flagged as a design assumption).
2. **Reproducibility** — anyone can clone the repository, build the firmware, run the simulation, and obtain the same results.
3. **Falsifiability** — the project is built so that its claims (reaction time, attenuation, detection threshold) can be tested and, if warranted, experimentally disproven by anyone.
4. **Openness to correction** — the structure in [`CONTRIBUTING.md`](CONTRIBUTING.md) exists precisely so the gaps identified above can be closed by a community, not just by a single author.

## Ethical commitment

We reiterate here, in a less technical register than in [`docs/threat_model.md`](docs/threat_model.md) and [`docs/compliance.md`](docs/compliance.md), the project's foundational commitment: EMP-Guardian is and will remain a **strictly defensive** project. Publishing it openly is a deliberate choice — we consider the collective resilience of civilian and scientific infrastructure against electromagnetic events to be a public good, and the knowledge needed to build it should not be monopolized.

## Invitation

This manifesto is an explicit invitation to researchers, engineers, and institutions: verify, challenge, measure, correct. A single set of experimental data published through a pull request is worth more to this project than any further expansion of the theoretical documentation.

---

*Ciprian Ștefan Pleșca, author and maintainer of the EMP-Guardian project.*
