#define LDR_L_PIN A2
#define LDR_R_PIN A3

void setup() //процедура setup
{
  Serial.begin(9600);
  Serial.println();
}

void loop() //процедура loop
{
  //объявляем переменную, которая будет хранить данные считанные 
  //из аналогового пина 
  int val_L = analogRead(LDR_L_PIN);
  int val_R = analogRead(LDR_R_PIN);
  //записать данные в последовательный порт
  Serial.println("Light sensor values: ");
  Serial.println("LEFT  RIGHT");
  Serial.print(val_L);
  Serial.print("  ");
  Serial.println(val_R);
  Serial.println();
   
  //выставляем задержку после вывода данных
  delay(1000);
}
