#ifndef COMM_H
#define COMM_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint8_t	type;
	float	latitude;
	float	longitude;
	uint8_t	sendet_id;
} CommPacket_t;

void	Comm_init(void);
void	Comm_process(void);

bool	send_location(float lat, float lon, uint8_t id);

bool	has_packet(void);
bool	get_packet(CommPacket_t *pkt);


#endif 
