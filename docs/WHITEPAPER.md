# EMP-Guardian: An Open, Reproducible Reference Architecture for Real-Time Electromagnetic Pulse Detection and Shielding in Critical Infrastructure

**Author:** Ciprian Ștefan Pleșca
**Affiliation:** Independent Researcher, Romania
**Version:** 1.0
**License:** CC BY-SA 4.0 (this document)

---

## Abstract

Electronic control systems now underpin nearly every critical infrastructure sector — power distribution, water treatment, telecommunications, transportation, and healthcare. This dependence creates an exposure to electromagnetic pulse (EMP) events, whether of natural origin (geomagnetic storms), industrial origin (switching transients, electrostatic discharge), or hostile origin, that can disable or permanently damage unshielded electronics in microseconds. Historically, the engineering knowledge required to detect and mitigate such events has resided either in access-restricted military standards or in undocumented proprietary commercial products, leaving independent researchers, universities, and resource-constrained infrastructure operators without a reproducible starting point.

This paper presents EMP-Guardian, an open reference architecture for real-time electromagnetic transient detection and protective shielding activation, together with the physical theory that motivates its design choices. The system couples a broadband sensing front-end with a dual-criterion adaptive-threshold detection algorithm — combining an adaptively tracked noise baseline with a rate-of-rise (dV/dt) criterion — to distinguish genuine fast transients from ordinary industrial switching noise, and activates a protective actuator within a microsecond-scale decision budget. The architecture is deliberately partitioned by latency tier: detection and actuation remain a minimal, deterministic state machine on the microcontroller, while higher-latency analysis (event classification, cross-node correlation) is confined to a host-side monitoring layer where computational cost is not safety-critical.

We derive the governing shielding-effectiveness equations from Maxwell's equations, present the detection algorithm's formal state machine, describe a hardware-abstraction-layer (HAL) methodology that keeps the decision logic portable and unit-testable without physical hardware, and outline a validation protocol bridging simulation, host-based testing, and eventual laboratory measurement. Throughout, we distinguish explicitly between what has been implemented and verified and what remains a design assumption pending experimental validation — a distinction we consider essential to the scientific usefulness of an open hardware/firmware project.

**Keywords:** Electromagnetic Pulse Hardening, Critical Infrastructure Protection, Real-Time Embedded Detection, Shielding Effectiveness, Adaptive Threshold Detection, Open Hardware, Reproducible Engineering

---

## 1. Introduction

### 1.1 Motivation

The reliability of modern infrastructure rests on an implicit assumption: that the electronic control systems governing it will continue to function. Electromagnetic pulse events threaten that assumption directly, inducing transient voltages and currents in conductors and printed-circuit traces that can exceed the breakdown limits of semiconductor junctions long before any software-level safeguard has a chance to react. Three broad sources of such events are relevant to civilian infrastructure protection:

1. **Geomagnetic disturbances** — large solar coronal mass ejections (Carrington-class events) that induce quasi-DC currents in long conductors such as power transmission lines and pipelines, with time constants on the order of tens of seconds to minutes.
2. **Industrial and switching transients** — inductive kickback from motor drives, contactors, and power-electronic converters, typically in the microsecond-to-millisecond range, which are common and must be reliably distinguished from genuine hazardous events to avoid nuisance shutdowns.
3. **High-energy electromagnetic transients**, whether naturally occurring (lightning-induced) or resulting from a fast-rising electromagnetic field incident on equipment, characterized by nanosecond-scale rise times and broadband frequency content.

Existing engineering guidance for hardening against such events is concentrated in two places: defense-sector standards (e.g., MIL-STD-188-125, MIL-STD-461) that are written for a specialized audience and are not typically accompanied by an openly buildable reference implementation, and commercial products whose internal design is proprietary. Between these two poles lies an underserved gap: an openly documented, independently reproducible system that a university laboratory, a research group, or an infrastructure operator without access to classified standards can build, test, measure, and improve.

### 1.2 Contribution

This paper documents the design rationale, governing physics, detection theory, and system architecture of EMP-Guardian, an open embedded system addressing this gap. Our contribution is threefold:

1. A **dual-criterion detection algorithm** that combines an adaptively tracked amplitude baseline with an explicit rate-of-rise criterion, formalized as a finite state machine, aimed at rejecting the false-positive class most damaging to operational trust — slow, high-amplitude industrial switching events.
2. A **latency-tiered system architecture** in which the sub-microsecond decision path is kept intentionally simple and verifiable, while richer analysis is deliberately excluded from the time-critical path and confined to a non-safety-critical monitoring layer.
3. A **portable, host-testable firmware methodology** built around a narrow hardware-abstraction contract, allowing the decision logic to be unit-tested on commodity development machines without cross-compilation or physical sensor hardware, and ported across microcontroller families by implementing a small, fixed set of primitives.

### 1.3 Scope and Non-Goals

This work is strictly defensive in scope. It addresses the detection, classification, and mitigation of electromagnetic transients affecting protected electronics; it does not address, and is not intended to inform, the generation, amplification, or directional projection of electromagnetic pulses. Any physical construction or deployment of the concepts described here must comply with applicable electromagnetic-compatibility regulation and export-control law in the implementer's jurisdiction.

---

## 2. Theoretical Foundations of Electromagnetic Shielding

### 2.1 Governing Equations

Shielding effectiveness (SE) quantifies the attenuation a conductive enclosure provides against an incident electromagnetic field, and is conventionally decomposed into three additive terms expressed in decibels:

```
SE_total = R + A + M
```

where R is the reflection loss at the material interface, A is the absorption loss through the material bulk, and M is a multiple-internal-reflection correction that is significant only for thin shields relative to the skin depth.

**Absorption loss.** Starting from Maxwell's equations in a good conductor (σ ≫ ωε), the wave equation for the electric field reduces to

```
∇²E = jωμσE
```

with plane-wave solutions of the form E(z) = E₀·e^(−γz), where the propagation constant γ = √(jωμσ) = (1 + j)/δ, and δ is the skin depth:

```
δ = √(2 / (ωμσ))
```

The absorption loss through a shield of thickness t follows directly:

```
A = 8.686 · (t / δ)     [dB]
```

**Reflection loss.** The impedance mismatch between free space (Z₀ = 377 Ω) and the shield's intrinsic impedance Zₘ = √(jωμ/(σ + jωε)) governs the reflection term:

```
R = 20 · log₁₀( |(Z₀ + Zₘ)² / (4 · Z₀ · Zₘ)| )     [dB]
```

For thick shields (t ≫ δ), M becomes negligible and SE_total ≈ R + A, a simplification adequate for most structural enclosures at the frequencies of interest to industrial hardening. For thin shields or low-frequency, high-permeability materials, the multiple-reflection correction is required and, if omitted, the approximation overstates the true shielding effectiveness — a limitation that must be stated explicitly whenever the simplified formula is used for engineering decisions.

### 2.2 Aperture and Seam Coupling

In practice, shielding effectiveness is rarely limited by the bulk material; it is limited by discontinuities — vents, cable penetrations, and seams. For a slot or aperture of maximum dimension L relative to wavelength λ:

```
SE_aperture = 20 · log₁₀( λ / (2L) )
```

Every unfiltered cable penetration and every unbonded seam constitutes a potential aperture of this kind, which is why practical hardening design treats gasketing, bonding, and filtered feedthroughs as first-order design elements rather than afterthoughts.

### 2.3 Material Selection

The choice of shielding material trades conductivity, permeability, mass, and cost. High-conductivity, non-magnetic materials (copper, aluminum) provide excellent performance against high-frequency content through the absorption and reflection mechanisms above; high-permeability alloys (e.g., mu-metal) provide superior low-frequency magnetic shielding at higher cost and with more demanding handling requirements (annealing sensitivity). A hardening design for a specific facility should be derived from the frequency content of its threat model, not from a single material chosen a priori.

---

## 3. Detection Theory

### 3.1 Design Requirement

A detection algorithm intended to trigger a protective actuator faces an asymmetric cost structure: a missed detection may result in destroyed equipment, while a false positive interrupts operation and, if frequent, erodes operator trust to the point of the protection being disabled altogether. The detection algorithm must therefore be evaluated not only on sensitivity but explicitly on its rejection of the dominant nuisance-trigger class in the deployment environment — in an industrial setting, this is switching noise from motor drives, contactors, and power converters, which can reach amplitudes comparable to a genuine transient but rises over a materially longer time window.

### 3.2 Dual-Criterion Formulation

We formalize detection as requiring the conjunction of two independent conditions, evaluated per ADC sample xₜ:

**Amplitude criterion.** The sample must exceed an adaptively tracked baseline b_t by a configurable margin m:

```
C_amplitude(t): xₜ > b_t + m
```

The baseline itself is updated as an exponentially weighted estimate of the recent signal floor during periods when no candidate event is active, so that the detector self-calibrates to ambient noise conditions rather than relying on a fixed absolute threshold that would need manual re-tuning across installations.

**Rate-of-rise criterion.** The sample-to-sample change must exceed a configurable slope threshold s:

```
C_slope(t): (xₜ − xₜ₋₁) > s
```

This criterion directly encodes the physical argument that voltage induced in any coupling loop is proportional to dB/dt rather than to field amplitude alone; a slow-rising event, however large its eventual amplitude, does not couple energy into a victim circuit the way a fast transient does, and should not be classified identically.

**Confirmation window.** An event is confirmed only when both criteria hold for a sustained window of W consecutive samples, which further suppresses single-sample noise spikes without materially increasing the microsecond-scale detection latency.

### 3.3 State Machine

The detector is implemented as a four-state machine:

```
IDLE      -- baseline tracking, no candidate event
  --> RISING     [both criteria satisfied]
RISING    -- candidate event, confirmation window in progress
  --> LATCHED    [criteria sustained for W samples: event confirmed]
  --> IDLE       [criteria drop before confirmation: reset]
LATCHED   -- event reported, actuator engaged, awaiting external reset
  --> COOLDOWN   [external reset received]
COOLDOWN  -- baseline re-stabilization period
  --> IDLE       [cooldown elapsed without incident]
```

Hysteresis is applied around the LATCHED state's exit condition specifically to prevent rapid oscillation near the trigger boundary, a standard technique in relay and comparator design applied here at the algorithmic level.

### 3.4 Why Not a Learned Classifier at This Tier

A natural question is whether a statistical or machine-learned classifier (autoencoder reconstruction error, isolation forest, recurrent sequence models) could improve detection accuracy over the threshold-based approach above. We consider this the wrong tier at which to introduce such methods. A decision budget on the order of microseconds, executed on a resource-constrained microcontroller, is not compatible with the inference latency or memory footprint of the models typically proposed in the broader anomaly-detection literature. The appropriate location for such techniques — if and when they are justified by measured data — is a downstream, non-latency-critical layer operating on the *event log* (timestamps, baseline drift, confirmation statistics) accumulated across one or more deployed units, where the goal shifts from microsecond-scale actuation to longer-horizon nuisance-pattern classification and fleet-level correlation. We regard the clean separation of these two concerns — deterministic real-time protection versus statistical post-hoc analysis — as a design principle, not a limitation to be engineered away.

---

## 4. System Architecture

### 4.1 Overview

```
Sensor + Signal Conditioning --> ADC --> Decision Unit (firmware) --> Shielding Actuator
                                              |
                                              v
                                     Communication Layer --> Monitoring Application --> REST API
```

The sensing front end (a broadband or Rogowski-type sensor followed by an amplifier, limiter, and anti-aliasing filter) feeds a high-speed analog-to-digital converter. The decision unit — the dual-criterion state machine of Section 3 — runs entirely within the microcontroller's main control loop and drives a protective actuator (MOSFET, IGBT, or solid-state relay) directly on confirmation. A parallel, lower-priority communication path reports confirmed events and telemetry to a host-side monitoring application, which in turn exposes event history through a REST interface for integration with SCADA or other operational systems.

### 4.2 Portability via Hardware Abstraction

To keep the decision logic auditable and independent of any single vendor's software development kit, the firmware is structured around a narrow hardware-abstraction contract: a small, fixed set of primitives (board initialization, ADC read, actuator GPIO write, blocking delay, UART initialization and write) that every board port must implement. The portable modules — the detector, the actuator controller, the communication handler, and the application loop — depend only on this contract and never include a vendor SDK header directly.

This has two practical consequences. First, the same decision logic can be ported to a new microcontroller family by writing a single board-port file against the abstraction contract, without modifying the algorithm itself. Second, and more importantly for scientific reproducibility, the decision logic can be exercised on a commodity development machine using a mocked implementation of the same contract, with no physical sensor, actuator, or board present — allowing the algorithm's logical correctness to be verified independently of, and prior to, any hardware validation.

### 4.3 Explicit Separation of Verified and Unverified Claims

Consistent with the design principle articulated in Section 3.4, we distinguish three categories of claim about this system, and maintain that distinction throughout its documentation:

1. **Logically verified** — properties of the algorithm established by unit tests executed against synthetic and adversarial input sequences (e.g., correct rejection of slow high-amplitude events, correct hysteresis behavior, correct state transitions under boundary conditions).
2. **Simulated** — properties established through circuit-level simulation of the sensing front end and actuator drive circuit, useful for design iteration but not a substitute for physical measurement.
3. **Experimentally unvalidated** — threshold, margin, slope, and hysteresis parameter values, which are principled initial design choices but have not yet been calibrated against measured field data or laboratory pulse testing, and must be treated as such by any implementer prior to deployment in a safety-relevant context.

We consider this tripartite distinction — rather than a single undifferentiated claim of "the system detects EMP events" — to be a necessary component of responsible engineering communication for an open hardware project of this kind.

---

## 5. Validation Methodology

A complete validation program for a system of this type proceeds through three stages of increasing fidelity, each addressing a different class of question:

**Stage 1 — Algorithmic verification (host-side).** The detection state machine is exercised with synthetic waveforms representing both target events and the dominant nuisance class (slow-rising industrial transients), using plain assertion-based tests that require no cross-compiler, board, or sensor. This stage answers the question "is the algorithm's logic correct," independent of any physical realization.

**Stage 2 — Circuit-level simulation.** The sensing front end and actuator drive circuit are modeled in a SPICE-class simulator to verify that the analog signal chain (conditioning amplifier, limiter, anti-aliasing filter) delivers a signal within the ADC's operating range under representative pulse waveforms, and that the actuator drive meets its timing budget. This stage answers "does the analog design behave as intended," prior to committing to physical fabrication.

**Stage 3 — Laboratory measurement.** Shielding effectiveness and end-to-end system latency must ultimately be measured using a bounded-wave or TEM-cell test setup, comparing field measurements external and internal to the shielded enclosure, per the classical relation:

```
SE_measured = 20 · log₁₀( V_external / V_internal )
```

This stage is the only one capable of validating the parameter values referenced in Section 4.3 and closing the gap between design assumption and experimental fact. We regard this stage as the actual gating factor for any claim of institutional-grade performance, and note that no formula improvement at Stage 1 or 2 can substitute for it.

---

## 6. Compliance and Ethical Considerations

Electromagnetic hardening and detection technology sits adjacent to dual-use concerns, and we address this directly rather than by omission. This work, and the reference implementation it describes, is restricted to the *defensive* side of the domain: sensing, classification, and protective response. It contains no description of pulse generation, amplification, or directional projection mechanisms, and none is required to understand, build, or validate the detection and shielding architecture presented here. Implementers remain responsible for compliance with applicable electromagnetic-compatibility regulation and dual-use export-control law in their jurisdiction, particularly where a system is intended for deployment in or adjacent to regulated infrastructure sectors.

---

## 7. Future Research Directions

We outline research directions that are, at the time of writing, deliberately not part of the verified system described above, to avoid conflating aspiration with implementation:

- **Fleet-level event correlation.** Where multiple units report to a shared monitoring backend, cross-node correlation of confirmed events could distinguish a geographically distributed cause (e.g., a geomagnetic disturbance) from a single unit's local switching environment. This is a data-analysis problem for the host-side monitoring layer, not the embedded decision path.
- **Classical statistical learning on event logs.** A Random Forest or Gradient Boosting classifier trained on accumulated event metadata (confirmation counts, baseline drift trends, time-of-day patterns) is a plausible tool for distinguishing recurring nuisance triggers from genuine rare events across a fleet of deployed units — again, applied to the log, not to the raw ADC stream.
- **Multiple-reflection shielding correction.** Implementing the thin-shield, low-frequency correction term omitted from the simplified SE = A + R approximation, once laboratory measurement data justifies the added model complexity for a specific enclosure geometry.
- **Formal timed-automata verification** of the confirmation-window and hysteresis timing behavior, as a complement to (not a replacement for) the assertion-based unit tests already in place, for deployments where a formal timing guarantee is a procurement requirement.

None of the above is claimed as implemented; each is stated here to preserve research context without expanding the description of the current system beyond what has been built and tested.

---

## 8. Conclusion

EMP-Guardian demonstrates that a scientifically honest, openly reproducible reference architecture for electromagnetic pulse detection and shielding is achievable without recourse to classified standards or proprietary design. The central engineering decision underlying the architecture — keeping the microsecond-scale decision path deterministic, minimal, and independently testable, while confining richer analysis to a non-safety-critical layer — is, we argue, not merely a resource-constraint workaround but a defensible design principle in its own right for any real-time protective system. We hope this reference implementation, together with its explicit documentation of what remains experimentally unvalidated, lowers the barrier for independent researchers and infrastructure operators to build upon, measure, and improve open electromagnetic hardening technology.

---

## References

1. Ott, H. W. *Electromagnetic Compatibility Engineering*. John Wiley & Sons, 2009.
2. Tesche, F. M., Ianoz, M. V., & Karlsson, T. *EMC Analysis Methods and Computational Models*. John Wiley & Sons, 1997.
3. MIL-STD-188-125-1, *High-Altitude Electromagnetic Pulse (HEMP) Protection for Ground-Based C4I Facilities Performing Critical, Time-Urgent Missions*, U.S. Department of Defense.
4. MIL-STD-461, *Requirements for the Control of Electromagnetic Interference Characteristics of Subsystems and Equipment*, U.S. Department of Defense.
5. IEC 61000 series, *Electromagnetic Compatibility (EMC)*, International Electrotechnical Commission.
6. Sutton, R. S., & Barto, A. G. *Reinforcement Learning: An Introduction*, 2nd ed. MIT Press, 2018.

---

*Corresponding author: Ciprian Ștefan Pleșca. This document is released under CC BY-SA 4.0 as part of the EMP-Guardian open research initiative.*
