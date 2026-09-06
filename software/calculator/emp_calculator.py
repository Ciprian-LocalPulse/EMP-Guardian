"""
EMP-Guardian - Engineering calculator
Author: Ciprian Ștefan Pleșca
License: MIT

Functional implementation of the physics formulas referenced across
docs/theory_of_operation.md and docs/hardware_specs.md. No dependencies
beyond the Python 3 standard library.

See README.md (in this directory) for the CLI quick-start, the honesty
notes on each formula's limitations, and the built-in materials list.
"""

from __future__ import annotations

import argparse
import math
import sys
from dataclasses import dataclass
from typing import NamedTuple, Optional, Tuple

MU_0 = 4 * math.pi * 1e-7  # permeability of free space, H/m


class Material(NamedTuple):
    """Conductivity (S/m) and relative permeability of a shielding material.

    Values are standard reference points (IACS-based conductivities,
    textbook mu_r) - see the "Honesty notes" in README.md, especially for
    the ferromagnetic materials (steel_mild, mu_metal), whose real mu_r is
    strongly frequency- and field-level dependent.
    """

    sigma: float  # electrical conductivity, S/m
    mu_r: float  # relative permeability, dimensionless


# Reference conductivities (S/m) and relative permeabilities. Copper is
# taken as the 100% IACS reference (sigma = 5.96e7 S/m) that the
# reflection-loss formula below normalizes against.
MATERIALS = {
    "copper": Material(sigma=5.96e7, mu_r=1.0),
    "silver": Material(sigma=6.30e7, mu_r=1.0),
    "aluminum": Material(sigma=3.64e7, mu_r=1.0),
    "brass": Material(sigma=1.67e7, mu_r=1.0),
    "steel_mild": Material(sigma=1.03e7, mu_r=1000.0),
    "stainless_304": Material(sigma=1.39e6, mu_r=1.02),
    "mu_metal": Material(sigma=1.60e6, mu_r=20000.0),
}


def _resolve_material(
    material: Optional[str], sigma: Optional[float], mu_r: Optional[float]
) -> Tuple[str, float, float]:
    """Resolves the (name, sigma, mu_r) to actually use.

    An explicit sigma and/or mu_r always overrides the named material's
    reference values, and the effective material is then labeled
    "custom" - even if a material name was also given - since the
    computation no longer matches that material's textbook properties.
    """
    if sigma is not None or mu_r is not None:
        base = MATERIALS.get(material) if material else None
        resolved_sigma = sigma if sigma is not None else (base.sigma if base else None)
        resolved_mu_r = mu_r if mu_r is not None else (base.mu_r if base else None)
        if resolved_sigma is None or resolved_mu_r is None:
            raise ValueError(
                "When overriding only one of sigma/mu_r, a known 'material' "
                "must also be given to supply the other value."
            )
        return "custom", resolved_sigma, resolved_mu_r

    if material is None:
        raise ValueError("Either 'material' or 'sigma'/'mu_r' must be given.")
    if material not in MATERIALS:
        known = ", ".join(sorted(MATERIALS))
        raise ValueError(f"Unknown material '{material}'. Known materials: {known}")
    m = MATERIALS[material]
    return material, m.sigma, m.mu_r


def skin_depth(
    frequency_hz: float,
    material: Optional[str] = "copper",
    sigma: Optional[float] = None,
    mu_r: Optional[float] = None,
) -> float:
    """Skin depth (delta) of a conductor at the given frequency, in meters.

        delta = 1 / sqrt(pi * f * mu_0 * mu_r * sigma)

    Cross-checked against the textbook reference value for copper at
    60 Hz (~8.5 mm) and at 1 MHz (~65.2 um) - see test_emp_calculator.py.
    """
    if frequency_hz <= 0:
        raise ValueError("frequency_hz must be positive.")
    _, resolved_sigma, resolved_mu_r = _resolve_material(material, sigma, mu_r)
    mu = MU_0 * resolved_mu_r
    return 1.0 / math.sqrt(math.pi * frequency_hz * mu * resolved_sigma)


@dataclass(frozen=True)
class ShieldingResult:
    material: str
    frequency_hz: float
    thickness_mm: float
    absorption_loss_db: float
    reflection_loss_db: float
    total_se_db: float
    multiple_reflection_note: str

    def summary(self) -> str:
        return (
            f"Shielding effectiveness ({self.material} @ {self.frequency_hz:.3g} Hz, "
            f"{self.thickness_mm:.3g} mm thick):\n"
            f"  Absorption loss (A):  {self.absorption_loss_db:8.2f} dB\n"
            f"  Reflection loss (R):  {self.reflection_loss_db:8.2f} dB\n"
            f"  Total SE = A + R:     {self.total_se_db:8.2f} dB\n"
            f"  Note: {self.multiple_reflection_note}"
        )


def shielding_effectiveness(
    frequency_hz: float,
    thickness_mm: float,
    material: Optional[str] = "copper",
    sigma: Optional[float] = None,
    mu_r: Optional[float] = None,
) -> ShieldingResult:
    """Shielding effectiveness SE = A + R, in dB (Ott's standard far-field,
    plane-wave approximation).

    - Absorption loss:  A(dB) = 8.686 * thickness / skin_depth
    - Reflection loss:  R(dB) = 168 + 10*log10(sigma_r / (mu_r * f))
      where sigma_r is conductivity relative to copper (copper = 1).

    This deliberately OMITS the multiple-reflection correction term B,
    which only matters when A < ~15 dB (thin foils, or low-frequency/
    high-mu_r shields) - see the "Honesty notes" in README.md. When that
    condition is met, `multiple_reflection_note` says so explicitly
    instead of silently under-reporting the true attenuation.
    """
    if frequency_hz <= 0:
        raise ValueError("frequency_hz must be positive.")
    if thickness_mm < 0:
        raise ValueError("thickness_mm must be non-negative.")

    resolved_material, resolved_sigma, resolved_mu_r = _resolve_material(
        material, sigma, mu_r
    )

    delta = skin_depth(frequency_hz, material=None, sigma=resolved_sigma, mu_r=resolved_mu_r)
    thickness_m = thickness_mm / 1000.0

    absorption_db = 8.686 * (thickness_m / delta)

    sigma_r = resolved_sigma / MATERIALS["copper"].sigma
    reflection_db = 168.0 + 10.0 * math.log10(sigma_r / (resolved_mu_r * frequency_hz))

    total_se_db = absorption_db + reflection_db

    if absorption_db < 15.0:
        note = (
            "A < ~15 dB: the multiple-reflection correction term B is "
            "NOT negligible here (thin shield and/or low frequency / "
            "high mu_r material) - the true SE is somewhat lower than "
            "A + R alone suggests. See README.md's honesty notes."
        )
    else:
        note = (
            "A >= ~15 dB: the omitted multiple-reflection correction term "
            "B is negligible in this regime - SE = A + R is a reasonable "
            "approximation."
        )

    return ShieldingResult(
        material=resolved_material,
        frequency_hz=frequency_hz,
        thickness_mm=thickness_mm,
        absorption_loss_db=absorption_db,
        reflection_loss_db=reflection_db,
        total_se_db=total_se_db,
        multiple_reflection_note=note,
    )


@dataclass(frozen=True)
class RogowskiResult:
    n_turns: int
    area_mm2: float
    length_mm: float
    didt_a_per_s: float
    mutual_inductance_h: float
    output_voltage_v: float

    def summary(self) -> str:
        return (
            f"Rogowski coil ({self.n_turns} turns, {self.area_mm2:.3g} mm^2 "
            f"loop area, {self.length_mm:.3g} mm mean length):\n"
            f"  Mutual inductance M:  {self.mutual_inductance_h * 1e9:8.3f} nH\n"
            f"  dI/dt:                {self.didt_a_per_s:8.3g} A/s\n"
            f"  Induced voltage:      {self.output_voltage_v:8.4f} V"
        )


def rogowski_sensitivity(
    n_turns: int, area_mm2: float, length_mm: float, didt_a_per_s: float
) -> RogowskiResult:
    """Rogowski coil mutual inductance and induced voltage for a given dI/dt.

    Uses the standard single-layer toroidal-solenoid approximation:

        M = mu_0 * N * A / l
        V = M * dI/dt

    where N is the number of turns, A the per-turn loop area, and l the
    coil's mean (magnetic path) length. This ignores core permeability
    (air-cored, as is typical for genuine Rogowski coils) and proximity/
    winding-pitch effects - see README.md for scope.
    """
    if n_turns <= 0:
        raise ValueError("n_turns must be positive.")
    if area_mm2 <= 0:
        raise ValueError("area_mm2 must be positive.")
    if length_mm <= 0:
        raise ValueError("length_mm must be positive.")

    area_m2 = area_mm2 * 1e-6
    length_m = length_mm * 1e-3

    mutual_inductance_h = MU_0 * n_turns * area_m2 / length_m
    output_voltage_v = mutual_inductance_h * didt_a_per_s

    return RogowskiResult(
        n_turns=n_turns,
        area_mm2=area_mm2,
        length_mm=length_mm,
        didt_a_per_s=didt_a_per_s,
        mutual_inductance_h=mutual_inductance_h,
        output_voltage_v=output_voltage_v,
    )


# Default stage estimates (min, max) in microseconds, taken directly from
# the latency table in docs/theory_of_operation.md. These are design-time
# estimates, NOT lab-measured values - see README.md's honesty notes.
DEFAULT_PROPAGATION_US = (0.5, 1.0)
DEFAULT_ADC_US = (0.4, 1.0)
DEFAULT_DECISION_US = (1.0, 4.0)
DEFAULT_ACTUATOR_US = (1.0, 5.0)
DEFAULT_TARGET_US = 10.0


@dataclass(frozen=True)
class LatencyResult:
    propagation_us: Tuple[float, float]
    adc_us: Tuple[float, float]
    decision_us: Tuple[float, float]
    actuator_us: Tuple[float, float]
    target_us: float
    total_min_us: float
    total_max_us: float

    def summary(self) -> str:
        stages = (
            f"  Signal propagation:   {self.propagation_us[0]:.2f}-{self.propagation_us[1]:.2f} us\n"
            f"  ADC sampling:         {self.adc_us[0]:.2f}-{self.adc_us[1]:.2f} us\n"
            f"  Decision algorithm:   {self.decision_us[0]:.2f}-{self.decision_us[1]:.2f} us\n"
            f"  Actuator switching:   {self.actuator_us[0]:.2f}-{self.actuator_us[1]:.2f} us\n"
        )
        totals = (
            f"  Total (best case):    {self.total_min_us:.2f} us\n"
            f"  Total (worst case):   {self.total_max_us:.2f} us\n"
            f"  Target:               {self.target_us:.2f} us\n"
        )
        if self.total_max_us > self.target_us:
            verdict = (
                f"  Verdict: OVER BUDGET by {self.total_max_us - self.target_us:.2f} us "
                "in the worst case."
            )
        else:
            verdict = "  Verdict: OK - worst-case total is within target."
        return "Latency budget:\n" + stages + totals + verdict


def latency_budget(
    propagation_us: Tuple[float, float] = DEFAULT_PROPAGATION_US,
    adc_us: Tuple[float, float] = DEFAULT_ADC_US,
    decision_us: Tuple[float, float] = DEFAULT_DECISION_US,
    actuator_us: Tuple[float, float] = DEFAULT_ACTUATOR_US,
    target_us: float = DEFAULT_TARGET_US,
) -> LatencyResult:
    """Sums the detection-to-activation latency budget and flags whether
    the worst case exceeds `target_us` (default: the 10 us target from
    docs/theory_of_operation.md).

    Each stage is a (min, max) pair in microseconds. Defaults match the
    latency table in docs/theory_of_operation.md exactly.
    """
    stages = (propagation_us, adc_us, decision_us, actuator_us)
    total_min_us = sum(stage[0] for stage in stages)
    total_max_us = sum(stage[1] for stage in stages)

    return LatencyResult(
        propagation_us=propagation_us,
        adc_us=adc_us,
        decision_us=decision_us,
        actuator_us=actuator_us,
        target_us=target_us,
        total_min_us=total_min_us,
        total_max_us=total_max_us,
    )


# --------------------------------------------------------------------- #
# Fast-transient waveform model (double exponential, E1-style)          #
# --------------------------------------------------------------------- #


@dataclass
class WaveformResult:
    e0_v_per_m: float
    alpha: float
    beta: float
    t_peak_s: float
    rise_time_10_90_s: float

    def summary(self) -> str:
        return (
            f"Double-exponential waveform (E0={self.e0_v_per_m:.4g} V/m, "
            f"alpha={self.alpha:.3g}, beta={self.beta:.3g}): "
            f"peak at {self.t_peak_s * 1e9:.3f} ns, "
            f"10-90% rise time {self.rise_time_10_90_s * 1e9:.3f} ns"
        )


def e1_waveform_peak_time(alpha: float, beta: float) -> float:
    """Exact time (s) at which E(t) = k*(exp(-alpha*t) - exp(-beta*t))
    reaches its peak: t_peak = ln(beta/alpha) / (beta - alpha)."""
    if beta <= alpha:
        raise ValueError("beta must be greater than alpha (fast rise, slower decay)")
    return math.log(beta / alpha) / (beta - alpha)


def e1_waveform(t_s: float, e0_v_per_m: float = 50000.0,
                alpha: float = 4.0e6, beta: float = 4.76e8) -> float:
    """Double-exponential fast-transient waveform (V/m), of the general
    form used to describe the E1 phase of a HEMP event in civilian/
    military test standards (IEC 61000-2-9 style model):

        E(t) = E0 * k * (exp(-alpha*t) - exp(-beta*t))

    The normalization constant k is computed exactly so the waveform's
    peak equals e0_v_per_m. Default alpha/beta are illustrative
    order-of-magnitude values (rise ~ns, decay ~hundreds of ns) - for
    compliance work, take alpha/beta directly from the standard's text,
    not from these defaults.
    """
    if t_s < 0:
        return 0.0
    t_peak = e1_waveform_peak_time(alpha, beta)
    raw_peak = math.exp(-alpha * t_peak) - math.exp(-beta * t_peak)
    k = 1.0 / raw_peak
    raw = math.exp(-alpha * t_s) - math.exp(-beta * t_s)
    return e0_v_per_m * k * raw


def e1_waveform_rise_time_10_90(alpha: float = 4.0e6, beta: float = 4.76e8,
                                 e0_v_per_m: float = 50000.0,
                                 samples: int = 200000, t_max_s: float = 1e-6) -> float:
    """Numerically estimates the 10%-90% rise time (s) by dense sampling."""
    dt = t_max_s / samples
    threshold_10 = 0.10 * e0_v_per_m
    threshold_90 = 0.90 * e0_v_per_m
    t10 = None
    t90 = None
    for i in range(samples):
        t = i * dt
        v = e1_waveform(t, e0_v_per_m, alpha, beta)
        if t10 is None and v >= threshold_10:
            t10 = t
        if t90 is None and v >= threshold_90:
            t90 = t
            break
    if t10 is None or t90 is None:
        raise RuntimeError("Rise time not found within t_max_s - increase t_max_s")
    return t90 - t10


def waveform_summary(e0_v_per_m: float = 50000.0, alpha: float = 4.0e6,
                      beta: float = 4.76e8) -> WaveformResult:
    """Convenience wrapper bundling peak time and rise time into one result."""
    return WaveformResult(
        e0_v_per_m=e0_v_per_m,
        alpha=alpha,
        beta=beta,
        t_peak_s=e1_waveform_peak_time(alpha, beta),
        rise_time_10_90_s=e1_waveform_rise_time_10_90(alpha, beta, e0_v_per_m),
    )


# --------------------------------------------------------------------- #
# CLI                                                                    #
# --------------------------------------------------------------------- #


def _add_material_args(parser: argparse.ArgumentParser) -> None:
    parser.add_argument(
        "--material",
        default="copper",
        help=f"Built-in material (default: copper). Known: {', '.join(sorted(MATERIALS))}",
    )
    parser.add_argument("--sigma", type=float, default=None, help="Override conductivity, S/m")
    parser.add_argument("--mu-r", type=float, default=None, help="Override relative permeability")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="emp_calculator.py",
        description="EMP-Guardian engineering calculator - see README.md for details.",
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    p_skin = subparsers.add_parser("skin-depth", help="Skin depth of a conductor")
    p_skin.add_argument("--freq", type=float, required=True, help="Frequency, Hz")
    _add_material_args(p_skin)

    p_shield = subparsers.add_parser("shielding", help="Shielding effectiveness (SE = A + R)")
    p_shield.add_argument("--freq", type=float, required=True, help="Frequency, Hz")
    p_shield.add_argument("--thickness-mm", type=float, required=True, help="Shield thickness, mm")
    _add_material_args(p_shield)

    p_rogowski = subparsers.add_parser("rogowski", help="Rogowski coil sensitivity")
    p_rogowski.add_argument("--n-turns", type=int, required=True, help="Number of turns")
    p_rogowski.add_argument("--area-mm2", type=float, required=True, help="Per-turn loop area, mm^2")
    p_rogowski.add_argument("--length-mm", type=float, required=True, help="Mean coil length, mm")
    p_rogowski.add_argument("--didt", type=float, required=True, help="dI/dt, A/s")

    p_latency = subparsers.add_parser("latency", help="Detection-to-activation latency budget")
    for stage, default in (
        ("propagation", DEFAULT_PROPAGATION_US),
        ("adc", DEFAULT_ADC_US),
        ("decision", DEFAULT_DECISION_US),
        ("actuator", DEFAULT_ACTUATOR_US),
    ):
        p_latency.add_argument(
            f"--{stage}",
            type=float,
            nargs=2,
            metavar=("MIN_US", "MAX_US"),
            default=None,
            help=f"Override {stage} stage (min max), us. Default: {default[0]}-{default[1]}",
        )
    p_latency.add_argument("--target", type=float, default=DEFAULT_TARGET_US, help="Target total, us")

    p_wave = subparsers.add_parser("waveform", help="Model the E1-style double-exponential waveform")
    p_wave.add_argument("--e0", type=float, default=50000.0, help="Peak field amplitude, V/m (default: 50000)")
    p_wave.add_argument("--alpha", type=float, default=4.0e6, help="Decay rate constant, 1/s (default: 4e6)")
    p_wave.add_argument("--beta", type=float, default=4.76e8, help="Rise rate constant, 1/s (default: 4.76e8)")

    return parser


def main(argv=None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)

    try:
        if args.command == "skin-depth":
            d = skin_depth(args.freq, material=args.material, sigma=args.sigma, mu_r=args.mu_r)
            print(f"Skin depth at {args.freq:.4g} Hz: {d * 1e6:.3f} um ({d * 1e3:.4f} mm)")

        elif args.command == "shielding":
            r = shielding_effectiveness(
                args.freq,
                args.thickness_mm,
                material=args.material,
                sigma=args.sigma,
                mu_r=args.mu_r,
            )
            print(r.summary())

        elif args.command == "rogowski":
            r = rogowski_sensitivity(args.n_turns, args.area_mm2, args.length_mm, args.didt)
            print(r.summary())

        elif args.command == "latency":
            kwargs = {}
            for stage, default in (
                ("propagation", DEFAULT_PROPAGATION_US),
                ("adc", DEFAULT_ADC_US),
                ("decision", DEFAULT_DECISION_US),
                ("actuator", DEFAULT_ACTUATOR_US),
            ):
                value = getattr(args, stage)
                kwargs[f"{stage}_us"] = tuple(value) if value is not None else default
            result = latency_budget(target_us=args.target, **kwargs)
            print(result.summary())

        elif args.command == "waveform":
            result = waveform_summary(e0_v_per_m=args.e0, alpha=args.alpha, beta=args.beta)
            print(result.summary())

    except ValueError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
