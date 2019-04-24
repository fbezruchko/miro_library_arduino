
#include "Protocol.h"
#include "misp.h"

int _misp_get_ident(struct tPacket *packet)
{
	if (packet == nullptr) return -1;
	packet->_dir = FROM_HOST;
	packet->_payload[0] = MiSP_GET_IDENT;
	packet->_size = 1;
	return _protocol_crc_calc(packet);
}

int _misp_ident(struct tPacket *packet, unsigned long ident)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_IDENT;
    unsigned char *p;
	*((unsigned long*)p) = packet->_payload + 1;
	packet->_size = 1*sizeof(unsigned long) + 1;
	return _protocol_crc_calc(packet);
}

int _misp_move(struct tPacket *packet, float lin_speed, float ang_speed, float distance)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_MOVE;
	unsigned char *p = packet->_payload + 1;
	*((float*)p) = lin_speed;
	p = p + sizeof(float);
	*((float*)p) = ang_speed;
	p = p + sizeof(float);
	*((float*)p) = distance;

	packet->_size = 3 * sizeof(float) + 1;

	return _protocol_crc_calc(packet);
}

int _misp_rotate(struct tPacket *packet, float ang_speed, float ang)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_MOVE;
	unsigned char *p = packet->_payload + 1;
	*((float*)p) = ang_speed;
	p = p + sizeof(float);
	*((float*)p) = ang;

	packet->_size = 2 * sizeof(float) + 1;

	return _protocol_crc_calc(packet);
}

int _misp_get_speed(struct tPacket *packet)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_GET_SPEED;

	packet->_size = 1;

	return _protocol_crc_calc(packet);
}

int _misp_speed(struct tPacket *packet, float lin_speed, float ang_speed)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_SPEED;
	unsigned char *p = packet->_payload + 1;
	*((float*)p) = lin_speed;
	p = p + sizeof(float);
	*((float*)p) = ang_speed;

	packet->_size = 2 * sizeof(float) + 1;

	return _protocol_crc_calc(packet);
}

int _misp_get_path(struct tPacket *packet)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_GET_PATH;

	packet->_size = 1;

	return _protocol_crc_calc(packet);
}

int _misp_path(struct tPacket *packet, float path)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_PATH;
	unsigned char *p = packet->_payload + 1;
	*((float*)p) = path;

	packet->_size = 1 * sizeof(float) + 1;

	return _protocol_crc_calc(packet);
}

int _misp_led_on(struct tPacket *packet, unsigned char led, unsigned char value)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_LED_ON;
	*(packet->_payload + 1) = led;
	*(packet->_payload + 2) = value;

	packet->_size = 3;

	return _protocol_crc_calc(packet);
}

int _misp_get_us_dist(struct tPacket *packet, unsigned char sensor)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_GET_US_DIST;
	*(packet->_payload + 1) = sensor;

	packet->_size = 2;

	return _protocol_crc_calc(packet);
}

int _misp_us_dist(struct tPacket *packet, unsigned char sensor, float value)
{
	if (packet == nullptr) return -1;
	unsigned char *p = packet->_payload + 1;
	packet->_payload[0] = MiSP_US_DIST;
	*p = sensor;
	p++;
	*((float*)p) = value;

	packet->_size = 2;

	return _protocol_crc_calc(packet);
}

int _misp_get_line_sense(struct tPacket *packet, unsigned char sensor)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_GET_LINE_SENSE;
	*(packet->_payload + 1) = sensor;

	packet->_size = 2;

	return _protocol_crc_calc(packet);
}

int _misp_line_sense(struct tPacket *packet, unsigned char sensor, unsigned char value)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_LINE_SENSE;
	*(packet->_payload + 1) = sensor;
	*(packet->_payload + 2) = value;

	packet->_size = 3;

	return _protocol_crc_calc(packet);
}

int _misp_get_light_sense(struct tPacket *packet, unsigned char sensor)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_GET_LIGHT_SENSE;
	*(packet->_payload + 1) = sensor;

	packet->_size = 2;

	return _protocol_crc_calc(packet);
}

int _misp_light_sense(struct tPacket *packet, unsigned char sensor, float value)
{
	if (packet == nullptr) return -1;
	unsigned char *p = packet->_payload + 1;
	packet->_payload[0] = MiSP_LIGHT_SENSE;
	*p = sensor;
	p++;
	*((float*)p) = value;

	packet->_size = 2;

	return _protocol_crc_calc(packet);
}

int _misp_buzzer_tone(struct tPacket *packet, unsigned char buzzer, unsigned int frequency, unsigned long duration)
{
	if (packet == nullptr) return -1;
	packet->_payload[0] = MiSP_MOVE;
	unsigned char *p = packet->_payload + 1;
	*p = buzzer;
	p = p++;
	*((unsigned int*)p) = frequency;
	p = p + sizeof(unsigned int);
	*((unsigned long*)p) = duration;

	packet->_size = 1 + 3 * sizeof(unsigned int) + 1;

	return _protocol_crc_calc(packet);
}


/***************************************************************************/

unsigned char _rx_get_cmd(struct tPacket *packet)
{
	unsigned char *p = packet->_payload;
	return *p;
}

float _rx_get_1f32(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 1;
	return *((float*)p);
}

unsigned char _rx_get_1b8(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 1;
	return *p;
}

unsigned char _rx_get_2b8(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 2;
	return *p;
}

float _rx_get_2f32(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 2;
	return *((float*)p);
}

float _rx_get_5f32(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 5;
	return *((float*)p);
}

float _rx_get_9f32(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 9;
	return *((float*)p);
}

unsigned int _rx_get_1u32(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 1;
	return *((unsigned long*)p);
}

unsigned int _rx_get_2u16(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 2;
	return *((unsigned int*)p);
}

unsigned int _rx_get_4u16(struct tPacket *packet)
{
	unsigned char *p = packet->_payload + 4;
	return *((unsigned int*)p);
}
