#include <SPI.h>
#include "printf.h"
#include "RF24.h"

int k = 30;
int speed = 200;
int povorot = 50;
int servo_speed = 100;
RF24 radio(9, 10);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

uint8_t address[][6] = { "1Node", "2Node" };

bool radioNumber = 1;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit
bool role = false;  // true = TX role, false = RX role
int take_massage;

void stop(){
  analogWrite(3, 0);
  analogWrite(2, 0);
  analogWrite(4, 0);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(7, 0);  
}
void pramo(){
  analogWrite(4, 255); // лев направление 
  analogWrite(5, speed - k); //лев мотор
  analogWrite(6, speed);  // прав мотор
  analogWrite(7, 0); // прав направление
}
void left(){
  analogWrite(4, 255);
  analogWrite(5, speed - povorot);
  analogWrite(6, speed - povorot);
  analogWrite(7, 255);

}
void right(){
  analogWrite(4, 0);
  analogWrite(5, speed - povorot);
  analogWrite(6, speed - povorot);
  analogWrite(7, 0);
}
void nazad(){
  analogWrite(4, 0);
  analogWrite(5, speed  - k);
  analogWrite(6, speed);
  analogWrite(7, 255);

}
void zaxvat(){
  analogWrite(2, 0);
  analogWrite(3, servo_speed);
}
void otpusk(){
  analogWrite(2, 255);
  analogWrite(3, servo_speed);
}


void setup() {

  Serial.begin(9600);
  while (!Serial) {
  }

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  Serial.println(F("RF24/examples/GettingStarted"));

  char input = Serial.parseInt();
  radioNumber = 0 == 1;
  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  radio.setPALevel(RF24_PA_MAX);  // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(take_massage));  // float datatype occupies 4 bytes
  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0
  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1
  radio.startListening();  // put radio in RX mode
  
}  // setup

void loop() {

    uint8_t pipe;
    if (radio.available(&pipe)) {              // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize();  // get the size of the payload
      radio.read(&take_massage, bytes);             // fetch payload from FIFO
      Serial.println(take_massage);  // print the payload's value
      if (take_massage == 1) pramo();
      if (take_massage == 2) nazad();
      if (take_massage == 3) left();
      if (take_massage == 4) right();
      if (take_massage == 5) zaxvat();
      if (take_massage == 6) otpusk();
      if (take_massage == 0) stop();


    }

}
