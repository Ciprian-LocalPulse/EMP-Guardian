# Arhitectura EMP-Guardian

**Autor:** Ciprian Ștefan Pleșca

## Prezentare generală

EMP-Guardian este format din patru subsisteme care lucrează împreună:

1. **Lanțul de detecție** (senzor + condiționare de semnal + ADC)
2. **Unitatea de decizie** (firmware, algoritm de detecție)
3. **Actuatorul de protecție** (ecranare/deconectare rapidă)
4. **Stratul de raportare** (comunicație, jurnalizare, monitorizare la distanță)

```
┌─────────────┐     ┌──────────────┐     ┌────────────────┐     ┌───────────────┐
│   Senzor    │────▶│ Condiționare │────▶│  Microcontroler │────▶│   Actuator    │
│ (bandă largă│     │  semnal +    │     │  (detecție +    │     │  de ecranare  │
│  / Rogowski)│     │     ADC      │     │    decizie)     │     │  (MOSFET/IGBT)│
└─────────────┘     └──────────────┘     └────────┬────────┘     └───────────────┘
                                                    │
                                                    ▼
                                          ┌───────────────────┐
                                          │  Comunicație /     │
                                          │  jurnalizare       │
                                          │  (UART/Ethernet)   │
                                          └────────┬───────────┘
                                                    ▼
                                          ┌───────────────────┐
                                          │  Aplicație de      │
                                          │  monitorizare      │
                                          │  (software/monitor)│
                                          └───────────────────┘
```

## Componente

- **Senzor EMP:** bobină Rogowski sau antenă de bandă largă, cu circuit de condiționare (amplificator + limitator de tensiune, esențial pentru a proteja intrarea ADC-ului).
- **Microcontroler:** eșantionează semnalul, rulează algoritmul de detecție (prag adaptiv + fereastră temporală), ia decizia de activare.
- **Actuator de ecranare:** comutatoare de mare viteză (MOSFET/IGBT sau relee cu stare solidă) care izolează electric echipamentul protejat sau îl conectează la o cale de descărcare controlată.
- **Interfață de comunicație:** UART, SPI sau Ethernet, pentru raportare către aplicația de monitorizare.
- **Alimentare de rezervă:** baterie sau supercondensator, astfel încât sistemul să rămână funcțional în timpul și imediat după eveniment.

## Flux de date

1. Senzorul produce un semnal analogic proporțional cu intensitatea câmpului electromagnetic detectat.
2. Circuitul de condiționare limitează și filtrează semnalul înainte de ADC.
3. ADC-ul eșantionează la o rată suficient de mare (minim 1 MSPS) pentru a surprinde fronturi rapide.
4. Algoritmul din firmware validează evenimentul (elimină comutații normale, zgomot RF, ESD minore).
5. Dacă evenimentul este confirmat, actuatorul este activat în mai puțin de 10 µs.
6. Evenimentul este scris în memorie nevolatilă și transmis către stratul de raportare.

## Principii de proiectare

- **Fail-safe:** dacă sistemul pierde alimentarea sau se defectează, ecranarea trebuie să rămână (sau să treacă) în starea protejată implicit.
- **Latență minimă:** timpul total detecție-activare este parametrul critic; orice optimizare software/hardware trebuie evaluată prin acest criteriu.
- **Testabilitate:** fiecare modul (detector, actuator, comunicație) trebuie să poată fi testat izolat, cu semnale simulate.
- **Portabilitate:** codul de firmware este scris pentru a fi ușor de portat între familii de microcontrolere ARM Cortex-M.
