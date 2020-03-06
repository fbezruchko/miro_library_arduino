#define LINE_C A1 //пин подключения сигнальной линии датчика линии
#define LINE_L A2 //пин подключения сигнальной линии левого датчика линии
#define LINE_R A3 //пин подключения сигнальной линии правого датчика линии

#define PWM_L_PIN 5
#define DIR_L_PIN 4
#define PWM_R_PIN 6
#define DIR_R_PIN 7

#define LIN_SPEED 130 //значение ШИМ, определяющее скорость двигателей

void setup()
{
  pinMode(PWM_L_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);
  pinMode(PWM_R_PIN, OUTPUT);
  pinMode(DIR_R_PIN, OUTPUT);

  pinMode(LINE_C, INPUT);
  pinMode(LINE_L, INPUT);
  pinMode(LINE_R, INPUT);
}

void forward()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, LIN_SPEED);
  analogWrite(PWM_R_PIN, LIN_SPEED);
}

void left_slow()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, LIN_SPEED - 40);
  analogWrite(PWM_R_PIN, LIN_SPEED + 40);
}

void left()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, 0);
  analogWrite(PWM_R_PIN, LIN_SPEED);
}

void right_slow()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, LIN_SPEED + 40);
  analogWrite(PWM_R_PIN, LIN_SPEED - 40);
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
  unsigned int ls_left = digitalRead(LINE_L);
  unsigned int ls_right = digitalRead(LINE_R);
  unsigned int ls_middle = digitalRead(LINE_C);

  if ((ls_left == 1) && (ls_middle == 1) && (ls_right == 0))
  {
    left_slow();
  }
  else if ((ls_left == 0) && (ls_middle == 1) && (ls_right == 1))
  {
    right_slow();
  }
  else if ((ls_left == 0) && (ls_middle == 0) && (ls_right == 1))
  {
    right();
  }
  else if ((ls_left == 1) && (ls_middle == 0) && (ls_right == 0))
  {
    left();
  }
  else
  {
    forward();
  }

  //delay(50);
}
