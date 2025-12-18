#include "protocol.h"
#include <string.h>

ProtocolPacketType_t PROTOCOL_GetType(const uint8_t *buf)
{
    return (ProtocolPacketType_t)buf[0];
}

uint8_t PROTOCOL_EncodeLocation(uint8_t *buf,
                                float lat,
                                float lon,
                                uint8_t id)
{
    buf[0] = PKT_LOCATION;
    memcpy(&buf[1], &lat, sizeof(float));
    memcpy(&buf[5], &lon, sizeof(float));
    buf[9] = id;

    return 10;
}

bool PROTOCOL_DecodeLocation(const uint8_t *buf,
                             uint8_t len,
                             float *lat,
                             float *lon,
                             uint8_t *id)
{
    if (len < 10 || buf[0] != PKT_LOCATION)
        return false;

    memcpy(lat, &buf[1], sizeof(float));
    memcpy(lon, &buf[5], sizeof(float));
    *id = buf[9];

    return true;
}

uint8_t PROTOCOL_EncodeScanRequest(uint8_t *buf,
                                   uint8_t id)
{
    buf[0] = PKT_SCAN_REQUEST;
    buf[1] = id;
    return 2;
}

