/*
  EEPROM Write

  Сохраняет в энергонезависимой памяти EEPROM значения,
  считанные с аналогового входа analog input 0.
  Данные значения останутся в памяти и после отключения питания
  от платы и в будущем могут быть доступны для
  другого скетча.
*/
#include <EEPROM.h>
// текущее значение адреса EEPROM
int address = 0;

void setup()
{
  int value = 0;
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(115200);

  for (int i = 0; i < 256; i++)
  {
    // записываем значение в энергонезависимую память
    EEPROM.write(address, value);

    // устанавливаем следующую ячейку памяти.
    // т.к. EEPROM содержит всего 512 ячеек – при достижении
    // конца памяти – возвращаемся на начало :)
    address = address + 1;
    value = value + 1;

    analogWrite(9, address);
    analogWrite(10, address);

    delay(30);
  }

  EEPROM.write(27, 0);

  for (address = 255; address >= 0; address--)
  {
    // считываем значение по текущему адресу EEPROM
    value = EEPROM.read(address);

    Serial.print(address);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.print("\t");
    if (value == address) Serial.print("OK"); else Serial.print("ERROR");

    Serial.println();

    analogWrite(9, address);
    analogWrite(10, address);
    
    delay(30);
  }
}

void loop()
{


}
