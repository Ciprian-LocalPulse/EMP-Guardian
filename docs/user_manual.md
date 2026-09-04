# Manual de utilizare

**Autor:** Ciprian Ștefan Pleșca

## 1. Cerințe

- Placă de dezvoltare STM32F4 (sau MCU compatibil) cu toolchain ARM GCC instalat
- Programator/debugger (ST-Link sau echivalent)
- Python 3.9+ pentru aplicația de monitorizare
- Senzor EMP conectat conform schemei din `hardware/schematics/`

## 2. Instalare firmware

```bash
cd firmware
make clean && make
st-flash write build/emp_guardian.bin 0x08000000
```

## 3. Configurare

Editează `firmware/src/config.h` pentru a ajusta:

- pragul de detecție (`EMP_THRESHOLD_ADC`)
- durata ferestrei temporale de confirmare
- pinii folosiți pentru actuator și comunicație

## 4. Pornire aplicație de monitorizare

```bash
cd software/monitor
pip install -r requirements.txt
python emp_monitor.py --port /dev/ttyUSB0 --baud 115200
```

Aplicația afișează în timp real evenimentele raportate de firmware și le salvează într-un jurnal local.

## 5. Interpretarea alertelor

| Mesaj | Semnificație |
|---|---|
| `EMP DETECTED` | Eveniment confirmat, ecranare activată automat |
| `THRESHOLD_ADJUSTED` | Pragul de detecție a fost recalibrat |
| `SELF_TEST_OK` / `SELF_TEST_FAIL` | Rezultatul auto-testului la pornire |

## 6. Resetare după eveniment

După activare, sistemul așteaptă implicit un interval configurabil (`shield_control_wait_reset`) înainte de a reveni la starea normală. Pentru medii critice, se recomandă resetare manuală, controlată de operator, în loc de resetare automată.

## 7. Depanare

- **Nu se conectează pe portul serial:** verifică drepturile de acces la port (`sudo usermod -a -G dialout $USER` pe Linux) și că firmware-ul a fost flashat cu succes.
- **False pozitive frecvente:** mărește pragul (`EMP_THRESHOLD_ADC`) sau ajustează fereastra temporală de confirmare; verifică ecranarea cablajului senzorului.
- **Timp de reacție prea mare:** verifică frecvența de eșantionare ADC și elimină operații blocante din bucla principală.
