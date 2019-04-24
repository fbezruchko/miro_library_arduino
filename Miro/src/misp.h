#ifndef misp_h
#define misp_h

#include "Protocol.h"

#define MiSP_GET_IDENT 100 //Запрос версии прошивки (->MIRO)
#define MiSP_IDENT 101 //Содержит версию прошивки (MIRO->)
#define MiSP_MOVE 111 //Команда на движение робота (->MIRO)
#define MiSP_ROTATE 112 //Команда на поворот робота (->MIRO)
#define MiSP_GET_SPEED 113 //Команда на получение данных о линейной и угловой скорости робота (->MIRO)
#define MiSP_SPEED 114 //Содержит данные о линейной и угловой скорости робота (MIRO->)
#define MiSP_GET_PATH 115 //Команда на получение длины пройденного пути (->MIRO)
#define MiSP_PATH 116 //Содержит длину пройденного пути (MIRO->)

#define MiSP_LED_ON	211 //Команда на включение светодиода (->MIRO)

#define MiSP_GET_US_DIST 212 //Запрос расстояния, измеренного УЗ-дальномером (->MIRO)
#define MiSP_US_DIST 213 //Содержит расстояние, измеренное УЗ-дальномером (MIRO->)

#define MiSP_GET_LINE_SENSE 214 //Запрос значения с датчикоа линии (->MIRO)
#define MiSP_LINE_SENSE 215 //Содержит значение с датчика линии (MIRO->)

#define MiSP_GET_LIGHT_SENSE 216 //Запрос значения с фоторезистора (->MIRO)
#define MiSP_LIGHT_SENSE 217 //Содержит значения с фоторезистора (MIRO->)

#define MiSP_BUZZER_TONE 218 //Запрос на проигрывание звука (->MIRO)

//То, что вообще пока не реализовано, но то, что надо обязательно реализовать

//#define MiSP_BUZZER_SONG //Запрос на проигрывание мелодии (->MIRO)
//#define MiSP_PMLIDAR //Запрос массива данных с ИК дальномера на сервоприводе
//#define MiSP_CAM_ANGLE //Установка угла сервопривода камеры

/*Функции формирования пакетов*/
int _misp_get_ident(struct tPacket *packet);
int _misp_ident(struct tPacket *packet, unsigned long ident);
int _misp_move(struct tPacket *packet, float lin_speed, float ang_speed, float distance);//если distance == 0, то это движение постоянное
int _misp_rotate(struct tPacket *packet, float ang_speed, float ang);//если ang == 0, то это движение постоянное
int _misp_get_speed(struct tPacket *packet);
int _misp_speed(struct tPacket *packet, float lin_speed, float ang_speed);
int _misp_get_path(struct tPacket *packet);
int _misp_path(struct tPacket *packet, float path);

int _misp_led_on(struct tPacket *packet, unsigned char led, unsigned char value);
int _misp_get_us_dist(struct tPacket *packet, unsigned char sensor);
int _misp_us_dist(struct tPacket *packet, unsigned char sensor, float value);
int _misp_get_line_sense(struct tPacket *packet, unsigned char sensor);
int _misp_line_sense(struct tPacket *packet, unsigned char sensor, unsigned char value);
int _misp_get_light_sense(struct tPacket *packet, unsigned char sensor);
int _misp_light_sense(struct tPacket *packet, unsigned char sensor, float value);
int _misp_buzzer_tone(struct tPacket *packet, unsigned char buzzer, unsigned int frequency, unsigned long duration);

/*Функции извлечения данных из пакетов*/
unsigned char _rx_get_cmd(struct tPacket *packet);
float _rx_get_1f32(struct tPacket *packet); //Вернуть первый параметр типа float после кода пакета
unsigned char _rx_get_1b8(struct tPacket *packet);
unsigned char _rx_get_2b8(struct tPacket *packet);
float _rx_get_2f32(struct tPacket *packet);
float _rx_get_5f32(struct tPacket *packet);
float _rx_get_9f32(struct tPacket *packet);
unsigned int _rx_get_2u16(struct tPacket *packet);
unsigned int _rx_get_4u16(struct tPacket *packet);

#define _rx_get_ident _rx_get_1u32
#define _rx_get_path _rx_get_1f32
#define _rx_get_lin_speed _rx_get_1f32
#define _rx_get_ang_speed _rx_get_5f32
#define _rx_get_dist _rx_get_9f32
#define _rx_get_ang _rx_get_5f32
#define _rx_get_buzzer _rx_get_1b8
#define _rx_get_buzzer_tone_freq _rx_get_2u16
#define _rx_get_buzzer_tone_dur _rx_get_4u16
#define _rx_get_linesense _rx_get_1b8
#define _rx_get_linesense_value _rx_get_2b8
#define _rx_get_lightsense _rx_get_1b8
#define _rx_get_lightsense_value _rx_get_2f32
#define _rx_get_led _rx_get_1b8
#define _rx_get_led_value _rx_get_2b8

#endif