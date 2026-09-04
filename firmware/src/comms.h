/**
 * EMP-Guardian - Modul de comunicație (raportare evenimente)
 * Autor: Ciprian Ștefan Pleșca
 * Licență: MIT
 */

#ifndef COMMS_H
#define COMMS_H

void comms_init(void);
void comms_send_alert(const char *message);
void comms_send_status(const char *key, int value);

#endif /* COMMS_H */
