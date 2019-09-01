
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "ht16k33.h"
#include "asciifont.h"
#include "OBD2UART.h"

boolean faster = true;
int speed = 0;
int rate = 1;
int msDelay = 100;

HT16K33 ht;
COBD obd;

extern SoftwareSerial softSerial(A2, A3);


void setup() {
  softSerial.begin(38400);
//  while (!softSerial);
  softSerial.println("Serial ready");
  obd.begin();
  softSerial.println("OBD begin");
  while (!obd.init(PROTO_CAN_11B_500K));
  softSerial.println("OBD ready");

  // initialize everything, 0x00 is the i2c address for the first one (0x70 is added in the class).
  ht.begin(0x01);
  ht.define16segFont((uint16_t*) &fontTable);
  ht.setBrightness(10);

  ht.set16Seg(0, 'k');
  ht.set16Seg(1, 'm');
  ht.set16Seg(2, '/');
  ht.set16Seg(3, 'h');
  ht.sendLed();
  delay(10000);
  ht.clearAll();
}

void loop() {
  char c;
  int  s;
  if (obd.readPID(PID_RPM, s)) {
    softSerial.print("Speed: ");
    softSerial.println(s);
    //  ht.clearAll();
    if (s >= 1000) {
      c = int(s / 1000);
      ht.set16Seg(0, c + '0');
      softSerial.print("0:");
      softSerial.println(c, DEC);
      s = int(s % 1000);
    }
    else {
      ht.set16Seg(0, ' ');
    }
    if (s >= 100) {
      c = int(s / 100);
      ht.set16Seg(1, c + '0');
      softSerial.print("1:");
      softSerial.println(c, DEC);
      s = int(s % 100);
    }
    else if (speed >= 100) {
      ht.set16Seg(1, '0');
    }
    else {
      ht.set16Seg(1, ' ');
    }
    if (s >= 10) {
      c = int(s / 10);
      ht.set16Seg(2, c + '0');
      softSerial.print("2:");
      softSerial.println(c, DEC);
      s = int(s % 10);
    }
    else if (speed >= 10) {
      ht.set16Seg(2, '0');
    }
    else {
      ht.set16Seg(2, ' ');
    }
    c = s;
    ht.set16Seg(3, c + '0');
    softSerial.print("3:");
    softSerial.println(c, DEC);
    ht.sendLed();
  }

//  if (faster) {
//    speed += rate;
//    if (speed > 110) {
//      faster = false;
//    }
//  }
//  else {
//    speed -= rate;
//    if (speed < 15) {
//      faster = true;
//    }
//  }
//
//  msDelay = speed * 2;
//
//  delay(msDelay);
}
