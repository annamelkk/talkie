#ifndef COMM_H
#define COMM_H

#include <stdint.h>
#include <stdbool.h>

#define MSG_TEST   0x01
#define MSG_SCAN   0x02

uint16_t COMM_Init(void);
void COMM_Process(void);

/* TX */
bool COMM_SendTest(uint8_t id);
bool COMM_SendScan(uint8_t id);

/* RX */
bool COMM_HasMessage(void);
uint8_t COMM_GetMessage(uint8_t *sender_id);

#endif

