#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

/* Packet types */
typedef enum
{
    PKT_LOCATION     = 0x01,
    PKT_SCAN_REQUEST = 0x02,
} ProtocolPacketType_t;

/* Encoders */
uint8_t PROTOCOL_EncodeLocation(uint8_t *buf,
                                float lat,
                                float lon,
                                uint8_t id);

uint8_t PROTOCOL_EncodeScanRequest(uint8_t *buf,
                                   uint8_t id);

/* Decoders */
bool PROTOCOL_DecodeLocation(const uint8_t *buf,
                             uint8_t len,
                             float *lat,
                             float *lon,
                             uint8_t *id);

/* Utilities */
ProtocolPacketType_t PROTOCOL_GetType(const uint8_t *buf);

#endif

