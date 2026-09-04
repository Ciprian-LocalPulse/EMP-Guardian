* EMP-Guardian - Detection signal-conditioning circuit simulation
* Author: Ciprian Ștefan Pleșca
* License: CERN-OHL-S v2 (see LICENSE-HARDWARE)
*
* Simplified model of the sensor -> conditioning -> ADC input chain,
* used to theoretically validate the circuit's response to a fast
* transient. Run with ngspice: ngspice emp_pulse_sim.sp

* Test transient source (fast pulse, NOT a real EMP source model -
* just a test signal to validate the detection circuit)
V1 in 0 PULSE(0 1000 0 1n 1n 100n 1u)

* Input impedance / coupling from the sensor
R1 in node1 50

* Conditioning filter (bandwidth limiting + protection)
L1 node1 out 10u
C1 out 0 100p
R2 out 0 1k

* Transient analysis
.tran 0.1n 10u

* Plot directives (for interactive ngspice runs)
.control
run
plot v(out)
.endc

.end
