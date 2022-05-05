#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

void BootComInit(uint32_t ulWantedBaud);
void BootComSend (uint8_t *buf,uint8_t size);
void BootComSendByte(uint8_t dt);
uint8_t  BootComRecv (uint8_t *buf,uint16_t size,uint32_t timeout);

#endif

