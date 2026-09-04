# Principiul de funcționare

**Autor:** Ciprian Ștefan Pleșca

## Ce este un eveniment EMP, din perspectiva unui sistem de protecție

Din punctul de vedere al acestui sistem, un "EMP" este orice tranzient electromagnetic cu:

- amplitudine peste un prag configurabil,
- timp de creștere foarte scurt (de ordinul nanosecundelor–microsecundelor),
- conținut spectral larg (de la kHz la sute de MHz sau GHz, în funcție de sursă).

Sistemul nu încearcă să identifice *cauza* evenimentului (natural sau nu) — tratează orice tranzient care depășește criteriile ca pe o amenințare și reacționează.

## De ce este nevoie de un algoritm de decizie, nu doar de un prag simplu

Un simplu comparator de tensiune ar declanșa false alarme la:

- comutarea unor sarcini industriale mari,
- descărcări electrostatice minore,
- interferențe radio locale (stații radio, radare, telefonie mobilă).

De aceea, algoritmul de detecție combină:

1. **Prag de amplitudine** — nivelul minim al semnalului.
2. **Fereastră temporală** — durata minimă/maximă cât semnalul rămâne peste prag.
3. **Rată de creștere (dV/dt)** — cât de abrupt este frontul semnalului; evenimentele EMP au fronturi mult mai rapide decât comutațiile industriale obișnuite.
4. **Histerezis** — pentru a evita oscilația rapidă între stările "activ"/"inactiv" în jurul pragului.

## Latența sistemului

Bugetul de timp total (țintă: sub 10 µs de la debutul evenimentului la activarea completă a ecranării) se împarte aproximativ astfel:

| Etapă | Timp estimat |
|---|---|
| Propagare semnal prin circuitul de condiționare | ~0.5–1 µs |
| Eșantionare ADC | ~0.4–1 µs la 1–2.4 MSPS |
| Evaluare algoritm de decizie (firmware) | ~1–3 µs |
| Comutare actuator (MOSFET/IGBT) | ~1–5 µs |

Aceste valori sunt orientative și trebuie validate experimental pentru fiecare implementare hardware concretă — vezi `docs/test_procedures.md`.

## Limitări cunoscute

- Sistemul protejează echipamentul din interiorul incintei ecranate; nu poate proteja echipamente aflate în afara acesteia.
- Un eveniment EMP de intensitate extremă, apărut înainte ca sistemul să ajungă la starea protejată, poate cauza totuși daune parțiale — de aceea proiectul recomandă și o ecranare pasivă permanentă (cușcă Faraday) ca primă linie de apărare, sistemul activ fiind un supliment, nu un înlocuitor.
- Timpul de reacție depinde puternic de calitatea senzorului și a circuitului de condiționare; valorile din acest document sunt teoretice.
