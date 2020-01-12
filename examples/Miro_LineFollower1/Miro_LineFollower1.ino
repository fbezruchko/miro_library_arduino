#define LINE_C A1 //пин подключения сигнальной линии датчика линии

#define PWM_L_PIN 5
#define DIR_L_PIN 4
#define PWM_R_PIN 6
#define DIR_R_PIN 7

#define LIN_SPEED 150 //значение ШИМ, определяющее скорость двигателей

void setup()
{
  pinMode(PWM_L_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);
  pinMode(PWM_R_PIN, OUTPUT);
  pinMode(DIR_R_PIN, OUTPUT);

  pinMode(LINE_C, INPUT);
}

void left()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, 0);
  analogWrite(PWM_R_PIN, LIN_SPEED);
}
void right()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, LIN_SPEED);
  analogWrite(PWM_R_PIN, 0);
}
void loop()
{
  unsigned int middle = digitalRead(LINE_C);
  if (middle == 1)
  {
    right();
  }
  else
  {
    left();
  }
  delay(50);
}
