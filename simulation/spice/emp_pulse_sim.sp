* EMP-Guardian - Simulare circuit de condiționare a semnalului de detecție
* Autor: Ciprian Ștefan Pleșca
* Licență: CERN-OHL-S v2 (vezi LICENSE-HARDWARE)
*
* Model simplificat al lanțului senzor -> condiționare -> intrare ADC,
* folosit pentru a valida teoretic răspunsul circuitului la un tranzient
* rapid. A se rula cu ngspice: ngspice emp_pulse_sim.sp

* Sursa de tranzient de test (impuls rapid, NU un model de sursă EMP reală -
* doar un semnal de test pentru validarea circuitului de detecție)
V1 in 0 PULSE(0 1000 0 1n 1n 100n 1u)

* Impedanța de intrare / cuplaj de la senzor
R1 in node1 50

* Filtru de condiționare (limitare bandă + protecție)
L1 node1 out 10u
C1 out 0 100p
R2 out 0 1k

* Analiza tranzitorie
.tran 0.1n 10u

* Directive de plot (pentru rulare interactivă în ngspice)
.control
run
plot v(out)
.endc

.end
