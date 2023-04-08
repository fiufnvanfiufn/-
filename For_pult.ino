#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(9, 10);

uint8_t address[][6] = { "1Node", "2Node" };

bool radioNumber = 1;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit
bool role = true;      // true = TX role, false = RX role
int send_message = 0;

int listen() {
  int xleft = analogRead(A0);
  int yleft = analogRead(A1);
  int yright = analogRead(A4);
  int xright = analogRead(A5);

  if (yright < 300) return 1;
  if (yright > 800) return 2;
  if (xright < 300) return 3;
  if (xright > 800) return 4;
  if (yleft < 300) return 5;
  if (yleft > 800) return 6;

  return 0;
}  //listen()


void setup() {

  Serial.begin(9600);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }

  Serial.println(F("RF24/examples/GettingStarted"));

  char input = Serial.parseInt();
  radioNumber = 1 == 1;
  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(sizeof(send_message));
  radio.openWritingPipe(address[radioNumber]);
  radio.openReadingPipe(1, address[!radioNumber]);
  radio.stopListening();  // put radio in TX mode

}  // setup

void loop() {

  unsigned long start_timer = micros();                     // start the timer
  bool report = radio.write(&send_message, sizeof(float));  // transmit & save the report
  unsigned long end_timer = micros();                       // end the timer

  if (report) {
    Serial.print(F("Transmission successful! "));  // payload was delivered
    Serial.print(F("Time to transmit = "));
    Serial.print(end_timer - start_timer);  // print the timer result
    Serial.print(F(" us. Sent: "));
    Serial.println(send_message);  // print payload sent
  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }
  send_message = listen();
  delay(100);

}  // loop
