#ifndef Q_KEYBOARD_H
#define Q_KEYBOARD_H

void initHID();
void sendKeyBuffer(uint8_t meta, uint8_t keys[]);

#endif
