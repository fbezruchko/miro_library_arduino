#include <Miro.h>

byte PWM_pins[2] = { 5, 6 };
byte DIR_pins[2] = { 4, 7 };
byte ENCODER_pins[2] = { 2, 3 };
Miro robot(PWM_pins, DIR_pins, ENCODER_pins);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

#define STATE_MOVE 0
#define STATE_ROTATE 1

int laps = 0;
unsigned char state = STATE_MOVE;
unsigned long last_path = 0;
unsigned long last_angle = 0;

void loop() {
	// put your main code here, to run repeatedly:
	swith (state)
	{
		case STATE_MOVE:
			if ((robot.chassis.getPath() - last_path) >= 1.0)
			{
				robot.move(0, 0);
				last_path = robot.getPath();
				state = STATE_ROTATE;
			}
			else
			{
				robot.move(robot.getOptLinSpeed(), 0);
			}
			break;
		
		case STATE_ROTATE:
			if ((robot.chassis.getAngle() - last_angle) >= 90.0)
			{
				robot.rotate(0, 0);
				last_angle = robot.getAngle();
				state = STATE_STATE;
			}
			else
			{
				robot.rotate(-robot.getOptAngSpeed());
			}
			break;
			
		default:
			robot.move(0, 0);
	}
	
	if ((robot.chassis.getAngle() / 360) > laps)
	{
		laps++;
		Serial.print("Laps: ");
		Serial.println(laps);
	}
	robot.Sync();
}
