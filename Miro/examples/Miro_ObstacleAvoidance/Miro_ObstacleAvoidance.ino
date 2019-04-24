#define TONE_PIN 11   //пин пъезоизлучателя
#define US_TRIG_PIN 13 //пин TRIG УЗ-дальномера
#define US_ECHO_PIN 12 //пин EСHO УЗ-дальномера

#define PWM_L_PIN 5
#define DIR_L_PIN 4
#define PWM_R_PIN 6
#define DIR_R_PIN 7

#define LIN_SPEED 128

//Коды состояний автомата объезда
#define STATE_FW 0
#define STATE_L 1
#define STATE_R 2

//Переменная состояния автомата объезда
int state = STATE_FW;

//расстояние до препятствия, ближе которого совершать объезд
#define DISTANCE_TH 20

//Функция движения вперед
void forward()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, LIN_SPEED);
  analogWrite(PWM_R_PIN, LIN_SPEED);
}

// Функция движения назад
void back()
{
  digitalWrite(DIR_L_PIN, HIGH);
  digitalWrite(DIR_R_PIN, HIGH);
  analogWrite(PWM_L_PIN, 255 - LIN_SPEED);
  analogWrite(PWM_R_PIN, 255 - LIN_SPEED);
}

// Функция остановки обоих двигателей
void stop()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, 0);
  analogWrite(PWM_R_PIN, 0);
}

// Функция проезда вперед на 50см
void forward1000()
{
  forward();
  delay(1000);
  stop();
}

// Функция поворота на 90 градусы налево
void left90()
{
  digitalWrite(DIR_L_PIN, HIGH);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, 255-LIN_SPEED);
  analogWrite(PWM_R_PIN, LIN_SPEED);

  delay(600);

  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, 0);
  analogWrite(PWM_R_PIN, 0);
}

// Функция поворота на 90 градусов направо
void right90()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, HIGH);
  analogWrite(PWM_L_PIN, LIN_SPEED);
  analogWrite(PWM_R_PIN, 255 - LIN_SPEED);

  delay(600);

  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, 0);
  analogWrite(PWM_R_PIN, 0);
}

// Функция разворота на 180 градусов
void turn180()
{
  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, HIGH);
  analogWrite(PWM_L_PIN, LIN_SPEED);
  analogWrite(PWM_R_PIN, 255 - LIN_SPEED);

  delay(1200);

  digitalWrite(DIR_L_PIN, LOW);
  digitalWrite(DIR_R_PIN, LOW);
  analogWrite(PWM_L_PIN, 0);
  analogWrite(PWM_R_PIN, 0);
}

//Функция объезда препятствия
void detour()
{
  float dist = get_distance();

  switch (state)
  {
    case STATE_FW:
      if (dist < DISTANCE_TH)
      {
        stop();
        delay(50);
        left90();
        state = STATE_L;
      }
      else
      {
        forward();
      }
      break;

    case STATE_L:
      if (dist < DISTANCE_TH)
      {
        turn180();
        state = STATE_R;
      }
      else
      {
        forward1000();
        delay(50);
        right90();
        state = STATE_FW;
      }
      break;

    case STATE_R:
      if (dist < DISTANCE_TH)
      {
        left90();
        state = STATE_FW;
      }
      else
      {
        forward1000();
        delay(50);
        left90();
        state = STATE_FW;
      }
      break;

    default: 
      break;
  }
}

// Функция считывания расстояния с УЗ-дальномера
float get_distance()
{
  digitalWrite(US_TRIG_PIN, LOW);
  delayMicroseconds(4);

  digitalWrite(US_TRIG_PIN, HIGH);
  delayMicroseconds(20);

  digitalWrite(US_TRIG_PIN, LOW);
  long i_distance = pulseIn(US_ECHO_PIN, HIGH, 23280);

  //long i_distance = pulseIn(US_ECHO_PIN, HIGH);
  //пересчет значений расстояния в сантимметры
  float distance = ((float)i_distance) / 58.2;

  if (distance >= 400) 
  {
    //Serial.println("US Distance infinite");
    distance = 401;
  }
  else if (distance <= 2)
  {
    //Serial.println("US Distance 0");
  }
  else
  {
    //Для отладки
    //Serial.print("US Distance: ");
    //Serial.println(distance);
  }
  return distance;
}

//Функция сигнализации приближения к препятствию
void us_beep()
{
  //объявляем переменные для хранения моментов времени
  unsigned long current_time = millis();
  static unsigned long last_time = 0;

  // получаем расстояние до объекта
  float dist = get_distance();

  if (dist > 0)
  {
    if ((current_time - last_time) >= 25 * dist)
    {
      last_time = current_time;
      tone(TONE_PIN, 2500, 30);
    }
  }
}

void setup()
{
  //Настраиваем линии управления двигателями
  pinMode(PWM_L_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);
  pinMode(PWM_R_PIN, OUTPUT);
  pinMode(DIR_R_PIN, OUTPUT);

  //настраиваем линии управления УЗ-дальномером
  pinMode(TONE_PIN, OUTPUT);
  pinMode(US_TRIG_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void loop()
{
  us_beep();
  detour();
  delay(50);
}
