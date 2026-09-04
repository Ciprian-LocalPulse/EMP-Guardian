# Model de amenințare

**Autor:** Ciprian Ștefan Pleșca

## Scop

Acest document definește ce tipuri de evenimente EMP-Guardian este proiectat să detecteze și să atenueze, și ce rămâne explicit în afara scopului proiectului.

## În scopul proiectului (defensiv)

- **EMP de origine naturală:** furtuni geomagnetice severe (evenimente de tip Carrington), care induc curenți intenși în rețelele electrice lungi.
- **Descărcări electrostatice industriale (ESD)** de intensitate ridicată în medii industriale.
- **Interferențe electromagnetice accidentale** provenite de la echipamente de mare putere (radare, transmițători, utilaje industriale).
- **Evenimente EMP de origine umană asupra cărora entitatea care operează sistemul nu are control** — sistemul reacționează defensiv, indiferent de cauză.

## Explicit în afara scopului

- Proiectul **nu conține** și **nu va conține** scheme, cod sau instrucțiuni pentru generarea, amplificarea sau direcționarea unui puls electromagnetic.
- Proiectul **nu este** un ghid de atac și nu trebuie folosit ca atare.
- Orice pull request sau contribuție care introduce funcționalități ofensive va fi respinsă.

## Actori și cazuri de utilizare vizate

| Actor | Caz de utilizare |
|---|---|
| Operator de infrastructură critică (energie, apă, telecom) | Protejarea echipamentelor de control (SCADA/ICS) |
| Instituție de cercetare / universitate | Platformă de studiu pentru duritate electromagnetică |
| Entitate guvernamentală / militară | Protecția centrelor de comandă, comunicații, echipamente sensibile (conform standardelor proprii, ex. MIL-STD-188-125) |
| Operator de centru de date | Linie suplimentară de apărare pentru echipamente critice |
| Radioamatori / cercetători independenți | Experimentare, educație, contribuții open-source |

## Limitări explicite

- Sistemul oferă protecție **suplimentară**, nu garantată 100%, mai ales împotriva evenimentelor de intensitate extremă.
- Eficiența reală depinde de calitatea implementării hardware (ecranare pasivă, calitatea senzorului, timpul de reacție validat experimental).
- Documentul `docs/compliance.md` detaliază obligațiile legale asociate utilizării și distribuirii proiectului.
