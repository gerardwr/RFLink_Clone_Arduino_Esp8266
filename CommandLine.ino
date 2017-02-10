// declare command as global to keep between calls to readCommand
String command="";

boolean readCommand() {
    char c;
    if(Serial.available() > 0)
    {
        c = Serial.read();
        if( (c != '\n') && (c != '\r') )
        {       
            command += c;
            return false;
        }
        else
            return true;
    }
}


void reboot(){
  // using the WDT, clears all SW and HW settings
  // !!!! does not seem to work OK !!!!!
#include <avr/wdt.h>

  wdt_enable(1000);
  while(1) {};
}


void executeCommand(){
  int x10Id, x10Switch;
  char x10Command[5];
  char buf[40];
  boolean unknown=false;
  int onOff;

  // command is like :
  //  10;MiLightv1;F746;00;3c00;ON;
  //  10;X10;000041;1;OFF;"
  
  //command="10;X10;000041;1;OFF;"
  // command="10;PING;"
  
  command.toUpperCase();
  //Serial.println(command);
  //command.toCharArray(buf, 40);
  if (command == "10;PING;") Serial.println("20;PONG;Nano;");
  else if (command == "10;VERSION;")   Serial.println("20;01;VER=1.1;REV=46;BUILD=03;Nano;");
  // else if (command == "10;REBOOT;") {Serial.println("Rebooting in 1 sec"); reboot(); } // does not work OK!!!!
  else if (command.startsWith("10;X10;")){
    // NOTE sscanf available for Arduino/AVR but NOT supported for ESP8266
    //   if (3 ==sscanf( buf, "10;X10;%x;%x;%[^;];", &x10Id, &x10Switch,x10Command )){                 // works OK for Arduino/AVR but NOT for ESP8266    x10Id=command.substring(7,13).toInt();
    x10Id = command.substring(11,13).toInt();
    x10Switch=  command.substring(14,15).toInt();
    command.substring(16,19).toCharArray(x10Command,4);   // 3 position from command (ON or OFF), 3+1=4 for toCharArray
    command.substring(11,13).toCharArray(buf,3);
    x10Id=strtol(buf,NULL, 16);
    //Serial.println(command.substring(16,19));
    sprintf(buf,"Debug : 10;X10/%x/%x/%s", x10Id, x10Switch, x10Command );
    Serial.println(buf);
    if(command.substring(16,18) == "ON") onOff=ON;
    else if(command.substring(16,19) == "OFF") onOff=OFF;

    digitalWrite(VCC_RX,LOW);      // RX433 power off to prevent local echo
    myx10tx.x10Switch(char(x10Id),x10Switch,onOff);
    digitalWrite(VCC_RX,HIGH);      // RX433 power on again
    sprintf(buf,"20;%02x;OK;", sequenceNumber); Serial.print(buf); Serial.print("\"");  Serial.print(command); Serial.println("\";Nano;" );
  }
  else unknown=true;
  
  if (unknown == true) { sprintf(buf,"20;%02x;CMD UNKNOWN;", sequenceNumber);  Serial.print(buf); Serial.print("\"");  Serial.print(command); Serial.println("\";Nano;" ); }

  sequenceNumber++;

  command="";
}
