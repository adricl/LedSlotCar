#include <Arduino.h>
#include <Wire.h>



//https://www.arduino.cc/en/Tutorial/MasterReader

void setup() { 
  Serial.begin(115200);
  Wire.begin(8);
  Wire.onRequest(requestEvent)
}

void loop(){
    delay(100);
}

void requestEvent() {
  Wire.write()
  Serial.println(x);         // print the integer
}