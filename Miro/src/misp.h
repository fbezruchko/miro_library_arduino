#ifndef misp_h
#define misp_h

#include "Protocol.h"

#define MiSP_SET 0x01
#define MiSP_GET 0x02

#define MiSP_DATA 0xff

#define MiSP_ATTACH 0x80
#define MiSP_DETACH 0x40

int _misp_prepare(struct tPacket *packet, uint8_t id); // Basic (empty) packet prepare function
int _misp_set_r(struct tPacket *packet, float angularSpeed, float angle); // Prepare MiSP_SET -r packet
int _misp_set_m(struct tPacket *packet, float linearSpeed, float angularSpeed, float distance); // Prepare MiSP_SET -r packet
int _misp_set_d(struct tPacket *packet, uint8_t deviceIndex, uint8_t parameter, uint8_t *value, uint8_t valueSize); // Prepare MiSP_SET -d packet
int _misp_get(struct tPacket *packet, char type);  // Prepare MiSP_GET (-f, -s, -v, -w, -W) packet
int _misp_get_i(struct tPacket *packet, uint8_t deviceID); // Prepare MiSP_GET -i packet
int _misp_get_d(struct tPacket *packet, uint8_t deviceIndex, uint8_t parameter); // Prepare MiSP_GET -d packet
int _misp_data(struct tPacket *packet, uint8_t* data, uint8_t dataSize); // Prepare MiSP_DATA packet
int _misp_attach(struct tPacket *packet, uint8_t deviceID, uint8_t *pins, uint8_t pinsCount); // Prepare MiSP_ATTACH packet
int _misp_detach(struct tPacket *packet, uint8_t deviceIndex); // Prepare MiSP_DETACH packet

/*Функции извлечения данных из пакетов*/
uint8_t _rx_get_id(struct tPacket *packet);
float _rx_get_1f32(struct tPacket *packet); //Вернуть первый параметр типа float после кода пакета
uint8_t _rx_get_1b8(struct tPacket *packet);
uint8_t _rx_get_2b8(struct tPacket *packet);
float _rx_get_2f32(struct tPacket *packet);
float _rx_get_5f32(struct tPacket *packet);
float _rx_get_9f32(struct tPacket *packet);
uint32_t _rx_get_1u32(struct tPacket *packet);
uint16_t _rx_get_2u16(struct tPacket *packet);
uint16_t _rx_get_4u16(struct tPacket *packet);

// #define _rx_get_ident _rx_get_1u32
// #define _rx_get_path _rx_get_1f32
// #define _rx_get_lin_speed _rx_get_1f32
// #define _rx_get_ang_speed _rx_get_5f32
// #define _rx_get_dist _rx_get_9f32
// #define _rx_get_ang _rx_get_5f32
// #define _rx_get_buzzer _rx_get_1b8
// #define _rx_get_buzzer_tone_freq _rx_get_2u16
// #define _rx_get_buzzer_tone_dur _rx_get_4u16
// #define _rx_get_linesense _rx_get_1b8
// #define _rx_get_linesense_value _rx_get_2b8
// #define _rx_get_lightsense _rx_get_1b8
// #define _rx_get_lightsense_value _rx_get_2f32
// #define _rx_get_led _rx_get_1b8
// #define _rx_get_led_value _rx_get_2b8

#endif