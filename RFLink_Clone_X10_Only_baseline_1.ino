/* 
# RFLink-Clone-Nano-ESP8266

RFlink clone (X10RF only) running on an Arduino Nano or ESP8266

What is RFLink  
Excellent RF controller for a wealth of RF 433Mhz, 868Mhz, 2.4Ghz running on a standard Arduino Mega.
Check out their website here : http://www.nemcon.nl/blog2/easyha

Why a clone  
I created a clone for the following reasons:
- I love Reverse Engineering of 433Mhz RF gadgets
- The RFLink software works great (try it!) but:
  = Requires an Arduino Mega to support the wealth of RF equipment
  = Arduino source code is currently unavailable, with the exception of an older "development" version. Code is supplied as "hex". So changes can be made to the latest code.
  = My favorite platform are Arduino Nano and ESP8266.
- Did I mention that I love Reverse Engineering RF gadgets

Disclaimer  
This is a prototype under development. Works for me, but you may have to change the code for your environment or use case.

 * 07-01-2017. Recognises RF signal on Arduino+RX433
 * 21-01-2017. RFLINK compatible output printed
 * 06-02-2017. Response to "10;PING;" sent ("20;PONG;"). PING is sent reglularly by Domoticz and expects a PONG.
 *             Seems that the sketch is accepted  by Domoticz as a "genuine" RFLink, sends X10RF that is recognised by a real RFLink, and receives X10 thats is sent by a real RFLink.
 *             So for Domoticz this is a working RFLink clone, for X10RF only!
 * 07-02-2017. Compiles OK for Wemos D1 too. Runs OK, X10 from RFlink is detected. Only change is:
                Pinnumbers (lines 17.35) and interrupt pin (lines 57..58)
 * 08-02-2017. port numbers changed to allow RX433 and TX433 direct connect to Nano header
 * 
 */

// RX433 direct connect to Nano header G/D2/D3
#define GND_RX 99   // not used
#define PORT_RX 2   // Arduino UNO = D2
#define VCC_RX 3    // Arduino UNO = D3

// TX433 direct connect to Nano header D5/D6/D7
#define GND_TX 6  // Arduino UNO = D6
#define PORT_TX 7 // Arduino UNO = D7
#define VCC_TX 8  // Arduino UNO = D8

/*
// RX433 GND/DATA/VCC direct connect to Wemos header D1/D2/D3. Werkt 07-02-2017
#define GND_RX D1
#define PORT_RX D2   // Arduino UNO = D2
#define VCC_RX D3    // Arduino UNO = D3

// TX433 direct connect to Wemos header D5/D6/D7
#define GND_TX 99   // Arduino UNO = D7
#define PORT_TX 99   // Arduino UNO = D6
#define VCC_TX 99    // Arduino UNO = D5
*/

#define reps 5        // Number of times that a RF command must be repeated.
#define ledpin 0 //BLUE_LED   // Pin for the led that blinks when a command is send. (0 = no blink). 13 for Arduino, D4 for Wemos D1 Mini.


byte sequenceNumber=2;        // 0 and 1 are used for the RFLink boot messages (see Setup);

void processRfCommand(char house, byte unit, byte command, bool isRepeat){
// ISR called when valid X10 RF received

  if (isRepeat) return;     // is a repeated command, already reported, disregard the repeat

  // output should be like : 20;6D;X10;ID=4a;SWITCH=0;CMD=OFF;
  char out[80]="20;00;CMD UNKNOWN;Nano;";
  if (command == B0010) sprintf(out, "20;%02x;X10;ID=%06x;SWITCH=%02d;CMD=ON;Nano;", sequenceNumber, house, unit); 
  if (command == B0011) sprintf(out, "20;%02x;X10;ID=%06x;SWITCH=%02d;CMD=OFF;Nano;", sequenceNumber, house, unit); 

  Serial.println(out); 
  sequenceNumber++;
}

#include "X10rfrx.h"      // X10 receive library in tab X10rfrx from https://github.com/tmittet/x10/tree/master/src. Name changed from X10rf to X10rfrx.
X10rf myx10rx = X10rf(0, PORT_RX, processRfCommand);  // X10 receive for Arduino
//X10rf myx10rx = X10rf(PORT_RX, 2, processRfCommand);  // X10 receive for WeMos D1 Mini

#include "x10rftx.h"      // X10 send library in tab x10rftx from https://github.com/pyrou/X10RF-Arduino.  Name changed from X10rf to X10rftx.
x10rf myx10tx = x10rf(PORT_TX,ledpin,reps);                // X10 send
  
void setup(){
  Serial.begin(57600);
  myx10tx.begin();
  myx10rx.begin();
  Serial.println("20;00;Nodo RadioFrequencyLink - RFLink Gateway V1.1 - R33;Nano;");
  Serial.println("20;01;VER=1.1;REV=46;BUILD=03;Nano;");

  // setup ports for direct connect RX433
  if (VCC_RX !=99) {pinMode(VCC_RX, OUTPUT);  digitalWrite(VCC_RX, HIGH); }
  if (GND_RX !=99) {pinMode(GND_RX, OUTPUT);  digitalWrite(GND_RX, LOW); }
  // setup ports for direct connect TX433
  if (PORT_TX !=99) {pinMode(PORT_TX, OUTPUT);  digitalWrite(PORT_TX, LOW);}
  if (VCC_TX !=99) {pinMode(VCC_TX, OUTPUT);  digitalWrite(VCC_TX, HIGH); }
  if (GND_TX !=99) {pinMode(GND_TX, OUTPUT);  digitalWrite(GND_TX, LOW); }
}

void loop(){
    {
        if (readCommand()) executeCommand();
    }
}



