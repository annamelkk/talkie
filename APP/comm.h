#ifndef COMM_H
#define COMM_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    COMM_NONE = 0,
    COMM_LOCATION,
    COMM_SCAN_REQUEST
} CommPacketType_t;

typedef struct
{
    CommPacketType_t type;
    float latitude;
    float longitude;
    uint8_t sender_id;
} CommPacket_t;

/* Lifecycle */
void COMM_Init(void);
void COMM_Process(void);

/* TX */
bool COMM_SendLocation(float lat, float lon, uint8_t id);
bool COMM_SendScanRequest(uint8_t id);

/* RX */
bool COMM_HasPacket(void);
bool COMM_GetPacket(CommPacket_t *pkt);

#endif

