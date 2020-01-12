#pragma once

#define ENCODERS_ON

#define ROBOT_MAX_DEVICES 12
#define PIN_VBAT A0
#define VOLTAGE_DIVIDER 1
#define WHEEL_COUNT 2
#define ROBOT_DIAMETER (0.163)
#define WHEEL_SEGMENTS (30.0)
#define WHEEL_RADIUS (0.0345)
#define MEAN_DEPTH_POWER 3
	
#define WHEEL_ANGLE_SPEED_MAX (1500)
#define WHEEL_ANGLE_SPEED_MIN (100)
#define MAX_WHEEL_BREAK_DELAY (60)

#define WHEEL_BREAK_DELAY_MULT 2.0

#define EEPROM_WHEEL_TABLE_ADDR 0
#define WHEEL_TABLE_SIZE (10)

#define DEFAULT_PID_Kp (0.015)
#define DEFAULT_PID_Kd (0.127)

//#define DEBUG_WHEEL_PID 1
#if defined (DEBUG_WHEEL_PID)
	#define DEBUG_WHEEL_PID_I_DEPTH (3*(int)WHEEL_SEGMENTS)
#endif

//#define DEBUG_wheelGetU 1
//#define DEBUG_wheel_rotate_sync 1
//#define DEBUG_wheelCalibrate 1
