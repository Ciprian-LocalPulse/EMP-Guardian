# Fișiere PCB (Gerber)

**Autor:** Ciprian Ștefan Pleșca
**Licență:** CERN-OHL-S v2 (vezi `../../../LICENSE-HARDWARE`)

Acest director va conține fișierele Gerber și proiectul KiCad PCB pentru placa de bază EMP-Guardian, odată ce schema din `hardware/schematics/` este finalizată și verificată.

## Convenție de denumire

- `emp_guardian_v1_gerber.zip` — pachet complet Gerber (RS-274X) pentru fabricație
- `emp_guardian_v1.kicad_pcb` — fișier sursă KiCad PCB

## Note pentru fabricație

- Se recomandă minim 4 straturi pentru izolarea corectă a planului de masă față de traseele de semnal rapid.
- Traseele dintre senzor și circuitul de condiționare trebuie ținute cât mai scurte, pentru a minimiza inductanța parazită.
