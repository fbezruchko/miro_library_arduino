// даём разумное имя для пинов 4, 5, 6, 7 - линии управления 
// двигателями робота
// Так нам не нужно постоянно вспоминать куда они подключёны
#define PWM_L_PIN 5
#define DIR_L_PIN 4
#define PWM_R_PIN 6
#define DIR_R_PIN 7

#define LIN_SPEED 120

void setup()
{
  // настраиваем пины в режим выхода,
  // как и раньше
  pinMode(PWM_L_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);
  pinMode(PWM_R_PIN, OUTPUT);
  pinMode(DIR_R_PIN, OUTPUT);

}

void loop()
{
  digitalWrite(DIR_L_PIN, HIGH);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, 255 - LIN_SPEED);
  analogWrite(PWM_R_PIN, LIN_SPEED);

  delay(2500);

  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, HIGH);
  analogWrite(PWM_L_PIN, LIN_SPEED);
  analogWrite(PWM_R_PIN, 255 - LIN_SPEED);

  delay(2500);
}
