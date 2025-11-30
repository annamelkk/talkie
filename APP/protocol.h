#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

typedef	enum
{
	PKT_LOCATION = 0x01,
	PKT_SCAN_REQUEST = 0x02,
	PKT_ACK = 0x03,
} ProtocolPacketType_t;

#define PROTOCOL_MAX_PAYLOAD 32

uint16_t	encode_location(uint8_t *buf,
			 float lat, float lon, uint8_t id);

bool	decode_location(const uint8_t *buf, uint16_t len,
		     float *lat, float *lon, uint8_t *id);

ProtocolPacketType_t get_type(const uint8_t *buf);

#endif
