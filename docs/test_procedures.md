# Proceduri de testare

**Autor:** Ciprian Ștefan Pleșca

## Principiu general

Testarea se face în etape, de la simulare software la validare hardware în laborator acreditat. Nu se recomandă și nu se descrie testarea cu surse EMP de mare putere în afara unui laborator specializat, autorizat și dotat cu ecranare adecvată.

## 1. Simulare (fără hardware)

- Rulează modelul din `simulation/spice/emp_pulse_sim.sp` cu ngspice pentru a valida răspunsul teoretic al circuitului de condiționare a semnalului.
- Ajustează parametrii sursei de impuls (amplitudine, timp de creștere) pentru a acoperi scenariile din `docs/threat_model.md`.
- Verifică marginile de siguranță ale componentelor (tensiuni maxime pe intrarea ADC etc.).

## 2. Testare unitară a firmware-ului

- Folosește fișierele din `firmware/tests/` pentru a valida algoritmul de detecție cu semnale sintetice (injectate prin funcții mock, nu prin hardware real).
- Verifică: praguri corecte, comportament la histerezis, absența falselor pozitive pe seturi de date "semnal normal".

## 3. Injectare de semnal controlat (laborator, tensiune joasă)

- Folosește un generator de impulsuri de laborator (nu o sursă EMP de mare putere) pentru a injecta tranzienți controlați, de amplitudine mică-medie, direct pe intrarea circuitului de condiționare.
- Măsoară timpul real detecție → activare actuator, folosind un osciloscop cu bandă suficientă.
- Documentează rezultatele față de valorile țintă din `docs/hardware_specs.md`.

## 4. Validare a ecranării pasive

- Măsoară atenuarea incintei folosind echipament de testare EMC (generator de semnal + antenă de emisie + antenă de recepție în interior), într-o cameră anecoică sau printr-un laborator acreditat.
- Compară rezultatele cu ținta declarată (> 80 dB) pe intervalul de frecvențe relevant.

## 5. Testare la nivel de sistem (laborator specializat, opțional)

- Testarea la niveluri de energie reprezentative pentru un eveniment EMP real trebuie efectuată **exclusiv** în facilități specializate, autorizate pentru acest tip de testare (de exemplu, laboratoare EMC/EMP acreditate conform IEC 61000-4-25 sau echivalent militar).
- Acest proiect nu oferă și nu va oferi instrucțiuni pentru construirea unei surse de testare de mare putere.

## Șablon de raport de testare

```
Data testului: 
Configurație hardware: 
Parametri semnal injectat: 
Timp de reacție măsurat: 
Rezultat (PASS/FAIL față de țintă): 
Observații: 
```
