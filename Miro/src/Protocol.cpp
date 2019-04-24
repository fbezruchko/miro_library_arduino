#include "Protocol.h"

int _protocol_crc_calc(struct tPacket *packet)
{
	if (packet == nullptr) return -1;
	unsigned char crc = *(((unsigned char*)packet) + 3);
	for (int i = 1; i < packet->_size + 1; i++)
	{
		crc ^= *(((unsigned char*)packet) + 3 + i);
	}

	packet->_crc = crc;

	return 0;
}

bool _protocol_crc_check(struct tPacket *packet)
{
	if (packet == nullptr) return false;
	unsigned char crc = *(((unsigned char*)packet) + 3);
	for (int i = 1; i < packet->_size + 1; i++)
	{
		crc ^= *(((unsigned char*)packet) + 3 + i);
	}

	if (packet->_crc == crc) return true;

	return false;
}

int _protocol_p2d(struct tPacket *packet, unsigned char *data)
{
	if ((packet == nullptr) || (data == nullptr)) return -1;
	*data = '$';
	*(data + 1) = 'M';
	*(data + 2) = packet->_dir;
	*((unsigned char*)(data + 3)) = packet->_size;
	for (int i = 0; i < packet->_size; i++)
	{
		*(data + 4 + i) = packet->_payload[i];
	}

	*(data + 4 + packet->_size) = packet->_crc;

	return 0;
}

int _protocol_d2p(struct tPacket *packet, unsigned char *data)
{
	if ((packet == nullptr) || (data == nullptr)) return -1;

	if ((*data != '$') || (*(data + 1) != 'M')) return -2;

	packet->_header[0] = '$';
	packet->_header[1] = 'M';

	packet->_dir = *(data + 2);
	packet->_size = *((unsigned char*)(data + 3));
	if (packet->_size > MAX_PAYLOAD) return -3;

	for (int i = 0; i < packet->_size; i++)
	{
		packet->_payload[i] = *(data + 4 + i);
	}

	packet->_crc = *(data + 4 + packet->_size);

	if (!_protocol_crc_check(packet)) return -4;

	return 0; //Возвращает номер команды
}