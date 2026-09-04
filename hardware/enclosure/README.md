# Carcasă / incintă ecranată

**Autor:** Ciprian Ștefan Pleșca
**Licență:** CERN-OHL-S v2 (vezi `../../LICENSE-HARDWARE`)

Acest director este rezervat pentru modele 3D (STL/STEP) ale carcasei ecranate (cușcă Faraday) în care se montează electronica protejată și placa EMP-Guardian.

## Recomandări de proiectare

- Continuitate electrică totală a suprafeței ecranate — orice deschidere (ventilație, cabluri) trebuie tratată cu filtre EMI dedicate sau garnituri conductive.
- Punctele de trecere a cablurilor de alimentare/date trebuie să treacă prin filtre de linie, nu direct prin perete.
- Ușile/capacele de acces trebuie să aibă garnituri conductive pentru a menține continuitatea ecranării.

Detaliile de atenuare țintă sunt în `docs/hardware_specs.md`; validarea se face conform `docs/test_procedures.md`.
