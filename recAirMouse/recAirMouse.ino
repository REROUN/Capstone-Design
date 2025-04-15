#include <SPI.h>
#include <Keyboard.h>
#include <Mouse.h>
#include "RF24.h"
 
int msg[5];
int msg1;
int msg2;
int msg3;
int msg4;
int msg5;
 
byte address[6] = "1Node";
 
RF24 radio(7,8);  // CE, CSN
 
 
void setup(void) {
 
  Serial.begin(9600);
 
  radio.begin();
  Mouse.begin();
  Keyboard.begin();
 
  radio.openReadingPipe(1, address);
 
  radio.startListening();
 
}
 
void loop(void) {
 
  if(radio.available()) {
    
    radio.read(&msg, sizeof(msg)); 

    
    
    
   
    Serial.print("Meassage (RX) = ");
 
    Serial.print(msg[0]);
    Serial.print(msg[1]);
    Serial.print(msg[2]);
    Serial.print(msg[3]);
    Serial.print(msg[4]);
    Serial.println(msg[5]);

    Mouse.move(msg[1], msg[0]);

    if(msg[2] < 680 && msg[3] > 750 && msg[4] > 770) {
      Keyboard.press(0x80);
      Keyboard.press(0x83);
      Keyboard.press(0x6F); //키 누름
    }
    else {
      Keyboard.releaseAll(); //키 해제
    }

    if(msg[4] > 710) {
      while(msg[4] > 710){
        if(msg[2] > 710 && msg[3] > 670) {
          Mouse.move(0, 0, -1);
        }
        if(msg[2] < 600 && msg[3] < 600) {
          Mouse.move(0, 0, 1);
        }
        delay(100);
        
        radio.read(&msg[2], sizeof(msg[2]));
        radio.read(&msg[3], sizeof(msg[3]));
        radio.read(&msg[4], sizeof(msg[4])); 
     }
    }

    if(msg[2] > 710 && msg[3] < 700 && msg[4] < 690) {
      Mouse.press(MOUSE_LEFT);
    }
    else {
      Mouse.release(MOUSE_LEFT);
    }

    if(msg[3] > 710 && msg[2] < 680 && msg[4] < 630) {
      Mouse.press(MOUSE_RIGHT);
    }
    else {
      Mouse.release(MOUSE_RIGHT);
    }
    
  }
}
