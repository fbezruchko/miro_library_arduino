// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <misp.h>

struct tPacket packet;
byte data[PROTOCOL_MAX_PACKET];

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
    Serial.begin(57600);
  _misp_ident(&packet);
  _protocol_p2d(&packet, data);
}

byte x = 0;

void loop() {
  Wire.beginTransmission(8); // transmit to device #8
  Serial.println();
  for (int i = 0; i < packet._size+6; i++)
  {
      Serial.print(data[i]);              // sends one byte
  }

  for (int i = 0; i < packet._size+6; i++)
  {
      Wire.write(data[i]);              // sends one byte
  }
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(2000);
}
