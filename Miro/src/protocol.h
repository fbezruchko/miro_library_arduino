#ifndef protocol_h
#define protocol_h

#include <stdint.h>

#define TO_HOST '>'
#define FROM_HOST '<'

#define MAX_PAYLOAD 25 //Размер полезной нагрузки, байт
#define PROTOCOL_MAX_PACKET (MAX_PAYLOAD + 7)

struct tPacket
{
	char _header[2] = {'$', 'M'};  //преамбула
	char _dir = FROM_HOST;		   //байт направления пакета: ">" - в хост от робота, "<" - в робота от хоста
	uint8_t _size;				   //payload size of packet
	uint8_t _payload[MAX_PAYLOAD]; //payload
	uint8_t _crc;				   //байт суммы по модулю 2 байтов полей _size, и полезной части payload
};

int _protocol_crc_calc(struct tPacket *packet);

bool _protocol_crc_check(struct tPacket *packet);

int _protocol_p2d(struct tPacket *packet, uint8_t *data);

int _protocol_d2p(struct tPacket *packet, uint8_t packets, uint8_t current, uint8_t *data);

#endif