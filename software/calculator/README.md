# EMP-Guardian — Engineering Calculator

A functional Python tool (not just documentation) for the physics formulas
referenced across `docs/theory_of_operation.md` and `docs/hardware_specs.md`.

No dependencies beyond the Python 3 standard library.

## What it computes

| Command | What it gives you |
|---|---|
| `skin-depth` | Skin depth δ of a conductor at a given frequency |
| `shielding` | Shielding effectiveness SE = A (absorption) + R (reflection), in dB |
| `rogowski` | Rogowski coil mutual inductance and induced voltage for a given dI/dt |
| `latency` | Sums the detection-to-activation latency budget and flags if it's over the 10 µs target |

## Quick start

```bash
cd software/calculator

# Skin depth of copper at 1 MHz
python3 emp_calculator.py skin-depth --freq 1e6 --material copper

# Shielding effectiveness of a 1mm copper enclosure wall at 1 MHz
python3 emp_calculator.py shielding --freq 1e6 --thickness-mm 1.0 --material copper

# Rogowski sensor: 100 turns, 50mm^2 loop area, 80mm mean length, dI/dt = 1e9 A/s
python3 emp_calculator.py rogowski --n-turns 100 --area-mm2 50 --length-mm 80 --didt 1e9

# Latency budget using the default stage estimates from theory_of_operation.md
python3 emp_calculator.py latency

# ...or override with your own measured stage timings
python3 emp_calculator.py latency --decision 0.5 1.5 --actuator 1 2
```

## Use as a library

```python
from emp_calculator import skin_depth, shielding_effectiveness, rogowski_sensitivity, latency_budget

d = skin_depth(frequency_hz=1e6, material="copper")
se = shielding_effectiveness(frequency_hz=1e6, thickness_mm=1.0, material="copper")
print(se.total_se_db)
```

Built-in materials: `copper`, `aluminum`, `steel_mild`, `stainless_304`, `silver`,
`brass`, `mu_metal`. You can override any material's conductivity/permeability
with `--sigma` / `--mu-r` (or the `sigma=` / `mu_r=` keyword arguments).

## Tests

```bash
python3 -m unittest test_emp_calculator.py -v
```

16 tests, including a cross-check against the textbook reference value for
copper's skin depth at 60 Hz (~8.5 mm).

## Honesty notes (read before using these numbers for real hardware)

- **Shielding effectiveness** uses SE = A + R, the standard far-field
  plane-wave approximation (Ott, *Electromagnetic Compatibility
  Engineering*). It **omits the multiple-reflection correction term B**,
  which only matters when A < ~15 dB (thin foils or low-frequency/high-µr
  shields). The tool warns you explicitly when that's the case — check the
  `multiple_reflection_note` field / CLI output.
- **`mu_r` for magnetic materials** (mild steel, mu-metal) is strongly
  frequency- and field-level dependent. The built-in values are indicative
  reference points, not a datasheet substitute — override with `--mu-r` once
  you have real data for your material and operating field.
- **Latency budget** defaults are the same design-time estimates from
  `docs/theory_of_operation.md`, explicitly *not* lab-measured values. Once
  you have real hardware, feed in your measured stage timings from
  `docs/test_procedures.md` instead of the defaults.
- None of this replaces validation by an accredited lab for any
  institutional or safety-critical deployment — see `docs/compliance.md`.
