/*

Boot response with RFDEBUG=ON; from a real RFLink is:
2017-02-06 15:28:46.657 RFLink: 20;00;Nodo RadioFrequencyLink - RFLink Gateway V1.1 - R46;
2017-02-06 15:28:46.657 RFLink: Controller Initialized!...
2017-02-06 15:28:46.734 RFLink: 20;01;MiLight=ON;
2017-02-06 15:28:46.734 RFLink: 20;02;VER=1.1;REV=46;BUILD=03;

Boot response with RFDEBUG=OFF; from a real RFLink is:
2017-02-06 15:36:18.388 User: initiated a RFLink Device Create command: 10;reboot;
2017-02-06 15:36:18.486 RFLink: Controller Initialized!...
2017-02-06 15:36:18.562 RFLink Detected, Version: 1.1 Revision: 46 Build: 3

RFLink compatible commands supported:
20;00;Nodo RadioFrequencyLink - RFLink Gateway V1.1 - R33;Nano;
20;01;VER=1.1;REV=46;BUILD=03;Nano;
10;version;
20;01;VER=1.1;REV=46;BUILD=03;Nano;
10;ping;
20;PONG;Nano;
10;x10;000046;1;on;
20;06;OK;"10;X10;000046;1;ON;";Nano;
10;x10;000047;2;off;
20;08;OK;"10;X10;000047;2;OFF;";Nano;


VERZENDEN
=========

INPUT SERIAL                  OUTPUT SERIAL                         COMMENT
---------------------------------------------------------------------------------------------------------------------------------------------------------
10;X10;000046;1;ON;           20;00;X10;ID=46;SWITCH=1;CMD=ON;      OK. OUTPUT = reception of transmitted = ECHO. X10 signal recognised by RFLink on RPi
10;X10;46;1;off;              20;11;X10;ID=46;SWITCH=1;CMD=OFF;     OK. OUTPUT = reception of transmitted = ECHO. X10 signal recognised by RFLink on RPi
10;X10;46;1;ON;               20;05;X10;ID=46;SWITCH=1;CMD=ON;      OK. Leading zeroes nicely ignored

ONTVANGEN
=========

SENT BY                       OUTPUT SERIAL                         COMMENT
---------------------------------------------------------------------------------------------------------------------------------------------------------
RFLink X10 F 1 ON             20;00;X10;ID=46;SWITCH=1;CMD=ON;      OK
RFLink X10 F 1 OFF            20;01;X10;ID=46;SWITCH=1;CMD=OFF;     OK
RFLnk PING                    20;PONG;                              OK for Domoticz
RPi x10rfgwr f00f 0           geen                                  Rpi x10rfgwr not OK
RPi x10rf j 1 off             20;03;X10;ID=4a;SWITCH=1;CMD=OFF;     RPi x10rf not OK, always OFF
RPi x10rf j 1 on              20;04;X10;ID=4a;SWITCH=1;CMD=OFF;     Rpi x10rf not OK, always OFF

*/

