#include <SPI.h>
#include "RF24.h"
#include <Wire.h>
#include "Kalman.h"

int msg[5] = {0,0,0,0,0};
int msg1 = 0;
int msg2 = 0;
int msg3 = 0;
int msg4 = 0;
int msg5 = 0;

byte address[6] = "1Node";
RF24 radio(7, 8);
 
int16_t gyroX, gyroZ;
 
int Sensitivity = 600;
int delayi = 3;
 
uint32_t timer;
uint8_t i2cData[14];
int flex_1 = A1;
int flex_2 = A2;
int flex_3 = A3;
 
void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.stopListening();
  
  Wire.begin();
  i2cData[0] = 7;
  i2cData[1] = 0x00;
  
  i2cData[3] = 0x00;
  while(i2cWrite(0x19,i2cData,4,false));
  while(i2cWrite(0x6B,0x01,true));
  while(i2cRead(0x75,i2cData,1));
  if(i2cData[0] != 0x68) {
    Serial.print(F("Error reading sensor"));
    while(1);
  }
 
  delay(100);
 
  while(i2cRead(0x3B,i2cData,6));
  timer = micros();
 
}
 
void loop() {
  int flex_pos_1;
  int flex_pos_2;
  int flex_pos_3;

  flex_pos_1 = analogRead(flex_1);
  flex_pos_2 = analogRead(flex_2);
  flex_pos_3 = analogRead(flex_3);

  msg3 = flex_pos_1;
  msg[2] = msg3;
  
  msg4 = flex_pos_2;
  msg[3] = msg4;
  
  msg5 = flex_pos_3;
  msg[4] = msg5;
  
  while(i2cRead(0x3B,i2cData,14));
  gyroX = ((i2cData[8] << 8) | i2cData[9]);
  gyroZ = ((i2cData[12] << 8) | i2cData[13]);
 
  gyroX = gyroX / Sensitivity / 1.1  * -1;
  gyroZ = gyroZ / Sensitivity  * -1;
 
  Serial.print("\t");
  Serial.print(gyroX);
  Serial.print(gyroZ);

  msg1=gyroX;
  msg[0]=msg1;
  
  msg2=gyroZ;
  msg[1]=msg2;
 
  
  radio.write(&msg, sizeof(msg));
  
 // Mouse.move(gyroZ, gyroX);
 
  Serial.print("\r\n");
  delay(delayi);
}
