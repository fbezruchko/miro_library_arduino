#ifndef protocol_h
#define protocol_h

#define TO_HOST '>'
#define FROM_HOST '<'

#define MAX_PAYLOAD 17 //Размер полезной нагрузки, байт
#define PROTOCOL_MAX_PACKET (MAX_PAYLOAD + 5)

struct tPacket {
	char _header[2] = { '$', 'M' }; //преамбула
	char _dir = FROM_HOST; //байт направления пакета: ">" - в хост от робота, "<" - в робота от хоста
	unsigned char _size; //реальый размер payload
	unsigned char _payload[MAX_PAYLOAD]; //массив payload (содержит в себе cmd и data, может быть от 1 (только код пакета) до 17 байт (1 байт кода пакета + 16 байт данных))
	unsigned char _crc; //байт суммы по модулю 2 байтов полей _size и полезной части payload
};

int _protocol_crc_calc(struct tPacket *packet);

bool _protocol_crc_check(struct tPacket *packet);

int _protocol_p2d(struct tPacket *packet, unsigned char *data);

int _protocol_d2p(struct tPacket *packet, unsigned char *data);

#endif