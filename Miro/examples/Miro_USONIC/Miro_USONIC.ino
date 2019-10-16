//получение данных от дальномера 
//дальномер имеет два выхода излучатель и приемник, соответственно линии
//trig, echo

#define TRIG 13
#define ECHO 12

//переменные для хранения расстояния и времени распространения
long duration, distance; 

void setup()
{
  Serial.begin(57600); //инициализация последовательного порта
  pinMode(TRIG,OUTPUT); //настройка пина излучателя на выход
}
void loop()
{
  digitalWrite(TRIG,LOW);
  delayMicroseconds(4); 
 
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(20); 
  digitalWrite(TRIG,LOW);
  duration = pulseIn(ECHO, HIGH); 
  //пересчет значений расстояния в сантиметры
  distance = duration/58.0;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(1000);
}
