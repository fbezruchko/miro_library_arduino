
#include "Protocol.h"
#include "misp.h"
#include <string.h>

int _misp_prepare(struct tPacket *packet, uint8_t id) // Basic (empty) packet prepare function
{
	if (packet == nullptr) return -1;
	if (id == MiSP_DATA) packet->_dir = TO_HOST; 
	else packet->_dir = FROM_HOST;
	packet->_size = 2;
	for (uint8_t i = 0; i < MAX_PAYLOAD; i++) packet->_payload[i] = 0;
	packet->_payload[0] = id;

	return 0;
}

int _misp_set_r(struct tPacket *packet, float angularSpeed, float angle) // Prepare MiSP_SET -r packet
{
	if (_misp_prepare(packet, MiSP_SET)) return -1;
	packet->_payload[1] = 'r';
	*((float*)(packet->_payload+2)) = angularSpeed;
	*((float*)(packet->_payload+2+sizeof(float))) = angle;
	packet->_size = 2 + 2*sizeof(float);

	return _protocol_crc_calc(packet);
}

int _misp_set_m(struct tPacket *packet, float linearSpeed, float angularSpeed, float distance) // Prepare MiSP_SET -r packet
{
	if (_misp_prepare(packet, MiSP_SET)) return -1;
	packet->_payload[1] = 'm';
	*((float*)(packet->_payload+2)) = linearSpeed;
	*((float*)(packet->_payload+2+sizeof(float))) = angularSpeed;
	*((float*)(packet->_payload+2+2*sizeof(float))) = distance;
	packet->_size = 2 + 3*sizeof(float);

	return _protocol_crc_calc(packet);
}

int _misp_set_d(struct tPacket *packet, uint8_t deviceIndex, uint8_t parameter, uint8_t *value, uint8_t valueSize) // Prepare MiSP_SET -d packet
{
	if (_misp_prepare(packet, MiSP_SET)) return -1;
	if (value == nullptr) return -1;
	packet->_payload[1] = 'd';
	packet->_payload[2] = deviceIndex;
	packet->_payload[3] = parameter;
	packet->_size = 4 + valueSize; 
	for(uint8_t i = 0; i < valueSize; i++) packet->_payload[4+i] = value[i];

	return _protocol_crc_calc(packet);
}

/*--------------------------------------------------------*/

int _misp_get(struct tPacket *packet, char type)  // Prepare MiSP_GET (-f, -s, -v, -w, -W) packet
{
	if (_misp_prepare(packet, MiSP_GET)) return -1;
	packet->_payload[1] = type;
	return _protocol_crc_calc(packet);
}

int _misp_get_i(struct tPacket *packet, uint8_t deviceID) // Prepare MiSP_GET -i packet
{
	if (_misp_prepare(packet, MiSP_GET)) return -1;
	packet->_payload[1] = 'i';
	packet->_payload[2] = deviceID;
	packet->_size = 3;
	return _protocol_crc_calc(packet);
}

int _misp_get_d(struct tPacket *packet, uint8_t deviceIndex, uint8_t parameter) // Prepare MiSP_GET -d packet
{
	if (_misp_prepare(packet, MiSP_GET)) return -1;
	packet->_payload[1] = 'd';
	packet->_payload[2] = deviceIndex;
	packet->_payload[3] = parameter;
	packet->_size = 4;
	return _protocol_crc_calc(packet);
}

int _misp_data(struct tPacket *packet, uint8_t* data, uint8_t dataSize) // Prepare MiSP_DATA packet
{
	if (_misp_prepare(packet, MiSP_DATA)) return -1;
	if (data == nullptr) return -1;
	for (uint8_t i = 0; i < dataSize; i++) packet->_payload[1+i] = data[i];
	packet->_size = 1 + dataSize;
	return _protocol_crc_calc(packet);
}

int _misp_attach(struct tPacket *packet, uint8_t deviceID, uint8_t *pins, uint8_t pinsCount) // Prepare MiSP_ATTACH packet
{
	if (_misp_prepare(packet, MiSP_ATTACH)) return -1; 
	packet->_payload[1] = deviceID;
	if (pins == nullptr)
	{
		packet->_size = 2;
		return _protocol_crc_calc(packet);
	}
	for (uint8_t i = 0; i < pinsCount; i++) packet->_payload[2+i] = pins[i];
	packet->_size = 2 + pinsCount;
	return _protocol_crc_calc(packet);
}

int _misp_detach(struct tPacket *packet, uint8_t deviceIndex) // Prepare MiSP_DETACH packet
{
	if (_misp_prepare(packet, MiSP_DETACH)) return -1; 
	packet->_payload[1] = deviceIndex;
	packet->_size = 2;
	return _protocol_crc_calc(packet);
}

/***************************************************************************/

uint8_t _rx_get_id(struct tPacket *packet)
{
	uint8_t *p = packet->_payload;
	return *p;
}

float _rx_get_1f32(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 1;
	return *((float *)p);
}

uint8_t _rx_get_1b8(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 1;
	return *p;
}

uint8_t _rx_get_2b8(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 2;
	return *p;
}

float _rx_get_2f32(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 2;
	return *((float *)p);
}

float _rx_get_5f32(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 5;
	return *((float *)p);
}

float _rx_get_9f32(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 9;
	return *((float *)p);
}

uint32_t _rx_get_1u32(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 1;
	return *((uint32_t *)p);
}

uint16_t _rx_get_2u16(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 2;
	return *((uint16_t *)p);
}

uint16_t _rx_get_4u16(struct tPacket *packet)
{
	uint8_t *p = packet->_payload + 4;
	return *((uint16_t *)p);
}
