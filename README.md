# EMP-Guardian

![EMP-Guardian](assets/emp-guardian-cover.png)

Sistem embedded open-source pentru detecția și atenuarea efectelor pulsului electromagnetic (EMP) asupra echipamentelor electronice critice.

**Autor:** Ciprian Ștefan Pleșca
**Licență:** MIT (software) · CERN-OHL-S (hardware) · CC BY-SA 4.0 (documentație)
**Status:** proiect deschis, în dezvoltare, publicat gratuit pentru comunitatea științifică și tehnică

> **Scop declarat:** acest proiect este strict **defensiv**. Conține doar mecanisme de detecție, ecranare și protecție a electronicii. Nu conține, nu descrie și nu promovează nicio metodă de generare sau amplificare a unui puls electromagnetic. Orice utilizare trebuie să respecte legislația națională și internațională aplicabilă (compatibilitate electromagnetică, export de tehnologii dual-use etc.).

## De ce există acest proiect

Un eveniment EMP — fie natural (furtună geomagnetică de tip Carrington), fie de origine umană (descărcare electrostatică industrială, impuls electromagnetic de înaltă putere folosit accidental sau ostil) — poate distruge instantaneu electronica nescreenată: rețele electrice, comunicații, echipamente medicale, servere, vehicule.

EMP-Guardian este un punct de plecare tehnic, complet documentat și reproductibil, pentru oricine vrea să construiască sau să studieze un sistem de protecție: cercetători, instituții, ingineri independenți, echipe universitare.

## Ce face sistemul

1. **Detectează** un eveniment electromagnetic anormal folosind un senzor de bandă largă și un algoritm de prag adaptiv.
2. **Confirmă** evenimentul (elimină falsele pozitive de la comutații industriale normale sau interferențe RF obișnuite).
3. **Activează** ecranarea/deconectarea electronicii protejate în microsecunde.
4. **Înregistrează** evenimentul și îl raportează printr-o aplicație de monitorizare.

## Structura repository-ului

```
EMP-Guardian/
├── README.md
├── MANIFESTO.md
├── LICENSE
├── LICENSE-HARDWARE
├── CONTRIBUTING.md
├── CODE_OF_CONDUCT.md
├── CITATION.cff
├── assets/
│   └── emp-guardian-cover.png
├── docs/
│   ├── architecture.md
│   ├── theory_of_operation.md
│   ├── hardware_specs.md
│   ├── threat_model.md
│   ├── test_procedures.md
│   ├── compliance.md
│   └── user_manual.md
├── firmware/
│   ├── src/
│   ├── include/
│   ├── tests/
│   └── Makefile
├── software/
│   ├── monitor/
│   └── api/
├── hardware/
│   ├── schematics/
│   ├── pcb/gerber/
│   ├── enclosure/
│   └── bom.csv
├── simulation/
│   ├── spice/
│   └── models/
└── .github/workflows/ci.yml
```

## Pornire rapidă

Compilare firmware (target implicit: STM32F4, portabil pe orice MCU ARM Cortex-M cu ADC rapid):

```bash
cd firmware
make
st-flash write build/emp_guardian.bin 0x08000000
```

Aplicația de monitorizare (Python 3.9+):

```bash
cd software/monitor
pip install -r requirements.txt
python emp_monitor.py --port /dev/ttyUSB0
```

Simulare a circuitului de detecție (necesită ngspice):

```bash
cd simulation/spice
ngspice emp_pulse_sim.sp
```

## Documentație

Vezi directorul [`docs/`](docs/) pentru arhitectură, principiul de funcționare, specificații hardware, model de amenințare, proceduri de testare și considerații legale.

Pentru poziționarea proiectului în raport cu literatura de specialitate și pentru motivația academică a acestei abordări, vezi [`MANIFESTO.md`](MANIFESTO.md).

## Cum contribui

Vezi [`CONTRIBUTING.md`](CONTRIBUTING.md). Orice contribuție — cod, documentație, corecții, date de testare — trebuie să păstreze caracterul strict defensiv al proiectului.

## Citare

Dacă folosești acest proiect în lucrări academice, vezi [`CITATION.cff`](CITATION.cff).

## Autor și mulțumiri

Proiect inițiat și menținut de **Ciprian Ștefan Pleșca**, publicat gratuit, fără scop comercial, pentru a sprijini cercetarea și infrastructura critică.
