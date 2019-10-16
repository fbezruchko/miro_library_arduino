#define LDR_L_PIN A2
#define LDR_R_PIN A3
#define LDR_ANG_PWM 65
#define LDR_LIN_PWM 105
#define Kp 0.4
#define MIN_LDR 190

#define PWM_L_PIN 5
#define DIR_L_PIN 4
#define PWM_R_PIN 6
#define DIR_R_PIN 7

void setup()
{
  pinMode(PWM_L_PIN, OUTPUT);
  pinMode(DIR_L_PIN, OUTPUT);
  pinMode(PWM_R_PIN, OUTPUT);
  pinMode(DIR_R_PIN, OUTPUT);

  pinMode(LDR_R_PIN, INPUT);
  pinMode(LDR_L_PIN, INPUT); 
}

void move_to_light()
{
  unsigned int right = analogRead(LDR_R_PIN);
  unsigned int left = analogRead(LDR_L_PIN);

  float error = left - right;
  float P = Kp * abs(error);

  if ((unsigned int)P > LDR_ANG_PWM) P = LDR_ANG_PWM;
  float mean = (right + left) / 2.0;

  if (mean < MIN_LDR)
  {
    if (mean >= left)
    {
      digitalWrite(DIR_L_PIN, LOW);
      digitalWrite(DIR_R_PIN, LOW);
      analogWrite(PWM_L_PIN, LDR_LIN_PWM);
      analogWrite(PWM_R_PIN, (unsigned int)P + LDR_LIN_PWM);
    }

    if (mean >= right)
    {
      digitalWrite(DIR_L_PIN, LOW);
      digitalWrite(DIR_R_PIN, LOW);
      analogWrite(PWM_L_PIN, (unsigned int)P + LDR_LIN_PWM);
      analogWrite(PWM_R_PIN, LDR_LIN_PWM);
    }
  }
  else
  {
    digitalWrite(DIR_L_PIN, LOW);
    digitalWrite(DIR_R_PIN, LOW);
    analogWrite(PWM_L_PIN, 0);
    analogWrite(PWM_R_PIN, 0);
  }
}
void loop()
{
  move_to_light();
}
