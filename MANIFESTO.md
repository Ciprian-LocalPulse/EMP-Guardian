# Manifest EMP-Guardian

**Autor:** Ciprian Ștefan Pleșca
**Versiune:** 1.0 — septembrie 2026

## Preambul

Acest document nu este o specificație tehnică — acelea există deja în `docs/`. Este o declarație de poziție: de ce a fost inițiat proiectul EMP-Guardian, ce statut epistemic are în acest moment și ce fel de contribuție științifică își propune să fie.

Îl scriem pentru că un repository tehnic, oricât de bine structurat, nu comunică de la sine motivația și limitele lui reale. Un cercetător care evaluează acest proiect are dreptul să știe exact ce are în față: un artefact validat experimental sau un cadru conceptual deschis pentru validare colectivă. Răspunsul, la momentul redactării acestui manifest, este al doilea.

## Poziționare

Protecția infrastructurii critice împotriva efectelor pulsului electromagnetic — fie de origine geomagnetică, fie industrială — este un domeniu tratat istoric în două moduri: prin standarde militare cu acces restricționat (MIL-STD-188-125, MIL-STD-461) și prin soluții comerciale proprietare, nedocumentate public. Între aceste două extreme există un gol: lipsa unui cadru **deschis, reproductibil și auditabil** pe care cercetătorii independenți, universitățile mici sau operatorii de infrastructură fără resurse de apărare să îl poată studia, adapta și îmbunătăți.

EMP-Guardian își propune să ocupe acest spațiu. Nu ca alternativă la standardele existente, ci ca implementare de referință, documentată integral, care traduce principiile acelor standarde într-o formă pe care oricine o poate compila, testa și critica.

## Statutul epistemic al proiectului

În spiritul onestității intelectuale pe care orice lucrare tehnică publicată ar trebui să o respecte, declarăm explicit stadiul actual:

- **Algoritmul de detecție** este implementat și testat unitar (teste sintetice, fără hardware real). Corectitudinea sa logică este verificabilă; performanța sa reală, nu.
- **Parametrii de prag și histerezis** sunt valori de proiectare inițială, nu rezultate de calibrare experimentală. Ele trebuie tratate ca atare de orice implementator.
- **Componentele hardware** (schemă electrică, PCB, carcasă ecranată) există la nivel de specificație și listă de componente, nu de fișiere de fabricație validate.
- **Nicio măsurătoare de laborator** nu a fost încă efectuată și publicată în acest repository.

Această secțiune nu este o slăbiciune de ascuns, ci punctul de plecare corect pentru orice evaluare externă. Un proiect care își declară limitele cu precizie este mai util științific decât unul care lasă impresia unei validări inexistente.

## Ce înseamnă „complet" pentru acest proiect

Completitudinea, în sensul urmărit aici, nu înseamnă absența lacunelor — orice sistem tehnic aflat la început are lacune. Înseamnă:

1. **Trasabilitate** — fiecare afirmație tehnică poate fi urmărită până la sursa ei (cod, standard citat, sau marcată explicit ca ipoteză de proiectare).
2. **Reproductibilitate** — oricine poate clona repository-ul, compila firmware-ul, rula simularea și obține aceleași rezultate.
3. **Falsificabilitate** — proiectul este construit astfel încât afirmațiile sale (timp de reacție, atenuare, prag de detecție) să poată fi testate și, eventual, infirmate experimental de oricine.
4. **Deschidere la corecție** — structura de `CONTRIBUTING.md` există tocmai pentru ca lacunele identificate mai sus să fie închise de o comunitate, nu doar de un singur autor.

## Angajament etic

Reiterăm aici, într-un registru mai puțin tehnic decât în `docs/threat_model.md` și `docs/compliance.md`, angajamentul fundamental al proiectului: EMP-Guardian este și va rămâne un proiect **strict defensiv**. Publicarea sa deschisă este o alegere deliberată — considerăm că reziliența colectivă a infrastructurii civile și științifice în fața evenimentelor electromagnetice este un bun public, iar cunoașterea necesară pentru a o construi nu ar trebui să fie monopolizată.

## Invitație

Acest manifest este o invitație explicită către cercetători, ingineri și instituții: verificați, contestați, măsurați, corectați. Un singur set de date experimentale publicat printr-un pull request valorează, pentru acest proiect, mai mult decât orice altă extindere a documentației teoretice.

---

*Ciprian Ștefan Pleșca, autor și menținător al proiectului EMP-Guardian.*
