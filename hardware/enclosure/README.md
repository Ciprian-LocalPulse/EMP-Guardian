# Enclosure / Shielded Housing

**Author:** Ciprian Ștefan Pleșca
**License:** CERN-OHL-S v2 (see `../../LICENSE-HARDWARE`)

This directory is reserved for 3D models (STL/STEP) of the shielded enclosure (Faraday cage) housing the protected electronics and the EMP-Guardian board.

## Design recommendations

- Full electrical continuity of the shielded surface — every opening (ventilation, cables) must be treated with dedicated EMI filters or conductive gaskets.
- Power/data cable feed-through points must pass through line filters, not directly through the wall.
- Access doors/covers must have conductive gaskets to maintain shielding continuity.

Target attenuation details are in [`docs/hardware_specs.md`](../../docs/hardware_specs.md); validation follows [`docs/test_procedures.md`](../../docs/test_procedures.md).
