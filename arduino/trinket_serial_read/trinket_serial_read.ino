/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald

  modified 2 Sep 2016
  by Arturo Guadalupi

  modified 8 Sep 2016
  by Colby Newman
*/
//#include <SoftwareSerial.h>

//SoftwareSerial serial1(0, 1);
// the setup function runs once when you press reset or power the board
int incomingByte = 0;
char buffer[12];
String ascii_num;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //  serial1.begin(115200);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  if (Serial.available()) {
    //        incomingByte = Serial.read();
    //        Serial.print((char)incomingByte);
    //        Serial.print(" , ");
    //        itoa (incomingByte, buffer, 10);
    //         // so if I understand it correctly - now in buffer should be a character (like '0' for 48)
    //         // optionaly I can combine those buffers into string
    //         // ascii_num += buffer;
    //          Serial.println (buffer);
    Serial.println(Serial.readStringUntil('\n'));
    //    Serial.println(Serial.readString());

  } // end if
  //  else {
  //    Serial.println("nope");
  //    delay(100);
  //  }
  delay(10);
}
