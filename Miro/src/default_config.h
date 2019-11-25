#pragma once
	
#if !defined(ENCODERS_ON)
	//#define ENCODERS_ON
#endif
	
#if !defined(ROBOT_MAX_DEVICES)
	#define ROBOT_MAX_DEVICES 12	//Maximum device count, that can be connect to robot
#endif

#if !defined(PIN_VBAT)
	#define PIN_VBAT A0	//Analog pin for battery voltage
#endif

#if !defined(VOLTAGE_DIVIDER)
	#define VOLTAGE_DIVIDER 1
#endif

#if !defined(WHEEL_COUNT)
	#define WHEEL_COUNT 2	//Number of motorized wheels
#endif

#if !defined(ROBOT_DIAMETER)	
	#define ROBOT_DIAMETER (0.163) //Distance between left and right wheels
#endif	
#if !defined(WHEEL_SEGMENTS)	
	#define WHEEL_SEGMENTS (30.0) //Count of segments on encoders disk
#endif	
#if !defined(WHEEL_RADIUS)	
	#define WHEEL_RADIUS (0.0345) //Wheel radius (meters)
#endif
#if !defined(MEAN_DEPTH)
	#define MEAN_DEPTH 8 //Wheel speed filter depth (array size)
#endif	

/*This is approximate values used during wheel calibration process only.*/
#if !defined(WHEEL_ANGLE_SPEED_MAX)
	#define WHEEL_ANGLE_SPEED_MAX (1500) //Maximum wheel angle speed (degrees/sec)
#endif
#if !defined(WHEEL_ANGLE_SPEED_MIN)
	#define WHEEL_ANGLE_SPEED_MIN (100) //Maximum wheel angle speed (degrees/sec)
#endif	
#if !defined(MAX_WHEEL_BREAK_DELAY)
	#define MAX_WHEEL_BREAK_DELAY (60)	//Maximum wheel break delay (milis)
#endif

#if !defined(WHEEL_BREAK_DELAY_MULT)
	#define WHEEL_BREAK_DELAY_MULT (2.0)	//Break delay from wheel table multipier
#endif

#if !defined(EEPROM_WHEEL_TABLE_ADDR)
	#define EEPROM_WHEEL_TABLE_ADDR 0
#endif
#if !defined(WHEEL_TABLE_SIZE)
	#define WHEEL_TABLE_SIZE (10)
#endif

#if !defined (DEFAULT_PID_Kp)
	#define DEFAULT_PID_Kp (0.015)
#endif

#if !defined (DEFAULT_PID_Kd)
	#define DEFAULT_PID_Kd (0.127)
#endif