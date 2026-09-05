"""
Unit tests for emp_calculator.py.
Run with: python -m pytest test_emp_calculator.py -v
(or plain: python -m unittest test_emp_calculator.py)
"""

import math
import unittest

from emp_calculator import (
    skin_depth,
    shielding_effectiveness,
    rogowski_sensitivity,
    latency_budget,
    MATERIALS,
)


class TestSkinDepth(unittest.TestCase):
    def test_copper_60hz_matches_textbook_value(self):
        # Reference value widely quoted in EMC textbooks: skin depth of
        # copper at 60 Hz is ~8.5 mm.
        d = skin_depth(60.0, material="copper")
        self.assertAlmostEqual(d * 1e3, 8.5, delta=0.3)

    def test_copper_1mhz_is_about_65_microns(self):
        d = skin_depth(1e6, material="copper")
        self.assertAlmostEqual(d * 1e6, 65.2, delta=1.0)

    def test_higher_frequency_gives_smaller_skin_depth(self):
        d_low = skin_depth(1e5, material="copper")
        d_high = skin_depth(1e7, material="copper")
        self.assertGreater(d_low, d_high)

    def test_rejects_nonpositive_frequency(self):
        with self.assertRaises(ValueError):
            skin_depth(0, material="copper")

    def test_unknown_material_rejected(self):
        with self.assertRaises(ValueError):
            skin_depth(1e6, material="unobtainium")

    def test_all_named_materials_compute(self):
        for name in MATERIALS:
            d = skin_depth(1e6, material=name)
            self.assertGreater(d, 0)


class TestShieldingEffectiveness(unittest.TestCase):
    def test_thicker_shield_gives_more_attenuation(self):
        thin = shielding_effectiveness(1e6, thickness_mm=0.1, material="copper")
        thick = shielding_effectiveness(1e6, thickness_mm=2.0, material="copper")
        self.assertGreater(thick.total_se_db, thin.total_se_db)

    def test_zero_thickness_gives_zero_absorption(self):
        r = shielding_effectiveness(1e6, thickness_mm=0.0, material="copper")
        self.assertAlmostEqual(r.absorption_loss_db, 0.0, places=6)
        # reflection loss alone should still be positive
        self.assertGreater(r.reflection_loss_db, 0.0)

    def test_low_a_triggers_b_correction_warning(self):
        # Very thin foil at low frequency -> A should be small -> warning note
        r = shielding_effectiveness(1e3, thickness_mm=0.01, material="copper")
        self.assertLess(r.absorption_loss_db, 15)
        self.assertIn("NOT negligible", r.multiple_reflection_note)

    def test_custom_material_override(self):
        r = shielding_effectiveness(1e6, thickness_mm=1.0, sigma=1e7, mu_r=1.0)
        self.assertEqual(r.material, "custom")
        self.assertGreater(r.total_se_db, 0)


class TestRogowski(unittest.TestCase):
    def test_voltage_scales_linearly_with_didt(self):
        r1 = rogowski_sensitivity(100, 50, 80, 1e9)
        r2 = rogowski_sensitivity(100, 50, 80, 2e9)
        self.assertAlmostEqual(r2.output_voltage_v, 2 * r1.output_voltage_v, places=6)

    def test_more_turns_more_sensitivity(self):
        r_few = rogowski_sensitivity(10, 50, 80, 1e9)
        r_many = rogowski_sensitivity(200, 50, 80, 1e9)
        self.assertGreater(r_many.output_voltage_v, r_few.output_voltage_v)

    def test_rejects_zero_turns(self):
        with self.assertRaises(ValueError):
            rogowski_sensitivity(0, 50, 80, 1e9)


class TestLatencyBudget(unittest.TestCase):
    def test_default_matches_theory_of_operation_table(self):
        result = latency_budget()
        # Values taken directly from docs/theory_of_operation.md
        self.assertAlmostEqual(result.total_min_us, 0.5 + 0.4 + 1.0 + 1.0)
        self.assertAlmostEqual(result.total_max_us, 1.0 + 1.0 + 4.0 + 5.0)

    def test_worst_case_over_10us_target_is_flagged(self):
        result = latency_budget()
        self.assertGreater(result.total_max_us, result.target_us)
        self.assertIn("OVER BUDGET", result.summary())

    def test_tighter_stage_estimates_can_meet_target(self):
        result = latency_budget(
            propagation_us=(0.3, 0.5),
            adc_us=(0.2, 0.4),
            decision_us=(0.5, 1.5),
            actuator_us=(1.0, 2.0),
        )
        self.assertLessEqual(result.total_max_us, result.target_us)
        self.assertIn(" OK", result.summary())


if __name__ == "__main__":
    unittest.main()
