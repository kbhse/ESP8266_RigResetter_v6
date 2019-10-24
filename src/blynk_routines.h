/*  src/blynk_routines.h */

/*
this isn't 'correct' way but blynk header files contain definitions so can't include BlynkSimpleEsp8266.h header in more than 1 file
or get 'multiple definition of blynk' error at compile.
so put BLYNK_WRITE functions in this header file and include it in main.cpp AFTER #include "BlynkSimpleEsp8266.h". this is my solution ...
see https://arduino.stackexchange.com/questions/58358/how-to-avoid-multiple-definition-of-blynk-error for context (but not this solution).
*/

#ifndef BLYNK_ROUTINES_H   /* include guards */
#define BLYNK_ROUTINES_H

#include "motherboard.h"
#include "auxiliary.h"
#include "TimeRoutines.h"
#include "WEMOS_SHT3X.h"                                                                           // Wemos Temperature and Humidity shield library
#include "defines.h"

extern SimpleTimer timer;

Motherboard mb;                                                                                    // only used for smosSrrUdpPort and smosSrrTimeout
Motherboard mbA;
Motherboard mbB;

AuxOut fan1;
AuxOut fan2;
AuxOut auxOutP6;                                                                                   // auxiliary output on PCF8574 P6
AuxIn auxInP7;                                                                                     // auxiliary input on PCF8574 P7 (CONFIG switch)

AuxOut pcbPowerLed;                                                                                // the 5V pcb power led

WidgetLED PowerLedA(V1);                                                                           // indicates state of motherboard A power led
WidgetLED PowerLedB(V5);                                                                           // indicates state of motherboard B power led
WidgetLED ConfigSwitchLed(V22);                                                                    // indicates state of pcb aux input P7 (CONFIG switch)
WidgetLED heartbeatLedA(V3);                                                                             // 
WidgetLED heartbeatLedB(V7);                                                                             // 

WidgetTerminal terminal(V10);
SHT3X sht30(0x44);                                                                                 // create an instance of the SHT3X class (SHT30 sensor shield has two user selectable I2C addresses)

void setPortPins()
	{
	// set the pins of the PCF8574 MP device
	// motherboard A (mbA)
	mbA.setPowerSwitchPin(0);
	mbA.setPowerLedPin(1);
	mbA.setResetSwitchPin(2);
	mbA.setHeartbeatLedPin(3);
	// motherboard B (mbB)
	mbB.setPowerSwitchPin(4);
	mbB.setPowerLedPin(5);
	mbB.setResetSwitchPin(6);
	mbB.setHeartbeatLedPin(7);
	// set the pins of the PCF8574 P device
	fan1.setAuxOutPin(4);                                                                            // Fan 1
	fan1.setActiveHIGH(false);
	fan2.setAuxOutPin(5);                                                                            // Fan 2
	fan2.setActiveHIGH(false);
	auxOutP6.setAuxOutPin(6);                                                                        // auxiliary output on PCF8574 P6
	auxOutP6.setActiveHIGH(true);
	auxInP7.setAuxInPin(7);                                                                          // auxiliary input on PCF8574 P7 (CONFIG switch)
	pcbPowerLed.setAuxOutPin(3);                                                                     // the 5V pcb power led
	pcbPowerLed.setActiveHIGH(true);
	}

// get state of motherboard A power led and send to WidgetLED PowerLedA
// called (via timer) by getPcbInputs()
// if the state has changed print to Blynk Terminal (ie don't print every time timer calls it)
void getPowerLedAState()
	{
	boolean state = mbA.getPowerLedState();
	if(state)
		{
		PowerLedA.setValue(255);
		}
	else
		{
		PowerLedA.setValue(35);  
		}
	if(mbA.getPowerLedLastState() != state)
    	{
    	#ifdef DEBUG_OUT
      		Serial.print(F("Motherboard A Power LED: "));
      		Serial.println(state ? "On" : "Off");
    	#endif
    	#ifdef TERMINAL_OUT
      		terminal.print(dateAndTime());
      		terminal.print(F(" MB A Power Led "));
      		terminal.println(state ? "On" : "Off");
      		terminal.flush();
    	#endif
    	mbA.setPowerLedLastState(state);
    	}
	}

// get state of motherboard B power led and send to WidgetLED PowerLedAConfigSwitchLed
// called (via timer) by getPcbInputs()
// if the state has changed print to Blynk Terminal (ie don't print every time timer calls it)
void getPowerLedBState()
	{
	boolean state = mbB.getPowerLedState();
	if(state)
		{
		PowerLedB.setValue(255);
		}
	else
		{
		PowerLedB.setValue(35);  
		}
	if(mbB.getPowerLedLastState() != state)
    	{
    	#ifdef DEBUG_OUT
      		Serial.print(F("Motherboard B Power LED: "));
      		Serial.println(state ? "On" : "Off");
    	#endif
    	#ifdef TERMINAL_OUT
      		terminal.print(dateAndTime());
      		terminal.print(F(" MB B Power Led "));
      		terminal.println(state ? "On" : "Off");
      		terminal.flush();
    	#endif
    	mbB.setPowerLedLastState(state);
    	}
	}

// get state of Aux Input P7 and send to WidgetLED ConfigSwitchLed
// called (via timer) by getPcbInputs()
void getAuxInputState()
	{
	boolean state = auxInP7.getAuxInState();
	if(state)
		{
		ConfigSwitchLed.setValue(255);
		}
	else
		{
		ConfigSwitchLed.setValue(35);  
		}
	}

void getPcbInputs()
// read the state of the pcb inputs
// called by timer (timer.setInterval(500L, getPcbInputs)) in setup()
	{
	getPowerLedAState();
	getPowerLedBState();
	getAuxInputState();
	}

// this is used to get the last state of the motherboard A power LED from Blynk on startup when Blynk.syncAll() is called
// used to only print Terminal message when state has changed (ie not on each timer call)
BLYNK_WRITE(V1)
  {
  if(param.asInt() == 255)
    {
    mbA.setPowerLedLastState(true);
    }
  else
    {
    mbA.setPowerLedLastState(false);
    }
  //mbA.setPowerLedLastState((param.asInt() == 255) ? true : false);
  }

// this is used to get the last state of the motherboard B power LED from Blynk on startup when Blynk.syncAll() is called
// used to only print Terminal message when state has changed (ie not on each timer call)
BLYNK_WRITE(V5)
  {
  if(param.asInt() == 255)
    {
    mbB.setPowerLedLastState(true);
    }
  else
    {
    mbB.setPowerLedLastState(false);
    }
  //mbB.setPowerLedLastState((param.asInt() == 255) ? true : false);
  }

// a button widget in the app on (V0) calls this function when its state changes
// it controls motherboard A's soft power switch
BLYNK_WRITE(V0)
	{
	int state = param.asInt();
	mbA.setPowerSwitchState(state);
	#ifdef DEBUG_OUT
		Serial.print("V0 state: ");
		Serial.println(state);
		Serial.print("mbA power switch state: ");
		Serial.println(mbA.getPowerSwitchState());
	#endif
	}

// a button widget in the app on (V2) calls this function when its state changes
// it controls motherboard A's reset switch
BLYNK_WRITE(V2)
	{
	int state = param.asInt();
	mbA.setResetSwitchState(state);
	#ifdef DEBUG_OUT
		Serial.print("V2 state: ");
		Serial.println(state);
		Serial.print("mbA reset switch state: ");
		Serial.println(mbA.getResetSwitchState());
	#endif
	}

// a button widget in the app on (V4) calls this function when its state changes
// it controls motherboard B's soft power switch
BLYNK_WRITE(V4)
	{
	int state = param.asInt();
	mbB.setPowerSwitchState(state);
	#ifdef DEBUG_OUT
		Serial.print("V4 state: ");
		Serial.println(state);
		Serial.print("mbB power switch state: ");
		Serial.println(mbB.getPowerSwitchState());
	#endif
	}

// a button widget in the app on (V6) calls this function when its state changes
// it controls motherboard B's reset switch
BLYNK_WRITE(V6)
	{
	int state = param.asInt();
	mbB.setResetSwitchState(state);
	#ifdef DEBUG_OUT
		Serial.print("V6 state: ");
		Serial.println(state);
		Serial.print("mbB reset switch state: ");
		Serial.println(mbB.getResetSwitchState());
	#endif
	}

// a button widget in the app on (V15) calls this function when its state changes
// it controls fan1 (relay 1)
BLYNK_WRITE(V15)
	{
	int state = param.asInt();
	fan1.setAuxOutState(state);
	#ifdef DEBUG_OUT
    	Serial.print(F("relay 1: "));
    	Serial.println(fan1.getAuxOutState());
  	#endif
  	#ifdef TERMINAL_OUT
    	terminal.print(dateAndTime());
    	terminal.print(F(" Aux Fan 1  "));
    	terminal.println(fan1.getAuxOutState() ? "On" : "Off");
    	terminal.flush();
  	#endif
	}

// a button widget in the app on (V16) calls this function when its state changes
// it controls fan2 (relay 2)
BLYNK_WRITE(V16)
	{
	int state = param.asInt();
	fan2.setAuxOutState(state);
	#ifdef DEBUG_OUT
    	Serial.print(F("relay 2: "));
    	Serial.println(fan2.getAuxOutState());
  	#endif
  	#ifdef TERMINAL_OUT
    	terminal.print(dateAndTime());
    	terminal.print(F(" Aux Fan 2  "));
    	terminal.println(fan2.getAuxOutState() ? "On" : "Off");
    	terminal.flush();
  	#endif
	}

// a button widget in the app on (V19) calls this function when its state changes
// it controls auxiliary output on PCF8574 P6
BLYNK_WRITE(V19)
	{
	int state = param.asInt();
	auxOutP6.setAuxOutState(state);
	#ifdef DEBUG_OUT
    	Serial.print(F("Aux Output: "));
    	Serial.println(auxOutP6.getAuxOutState());
  	#endif
  	#ifdef TERMINAL_OUT
    	terminal.print(dateAndTime());
    	terminal.print(F(" Aux Output "));
    	terminal.println(auxOutP6.getAuxOutState() ? "On" : "Off");
    	terminal.flush();
  	#endif
	}

// a button widget in the app on (V11) calls this function when its state changes
// it sets the autoRestartFlg for motherboard A
BLYNK_WRITE(V11)
		{
		boolean state = param.asInt();
		mbA.setAutoRestartFlag(state);
		}

// a button widget in the app on (V12) calls this function when its state changes
// it sets the autoRestartFlg for motherboard B
BLYNK_WRITE(V12)
		{
		boolean state = param.asInt();
		mbB.setAutoRestartFlag(state);
		}

// a Numeric Input widget in the app on (V30) calls this function when its state changes
// the widget will update the smosSrrUdpPort variable
BLYNK_WRITE(V30)
		{
		int val = param.asInt();
		mb.setSmosSrrUdpPort(val);
		}

// a Numeric Input widget in the app on (V31) calls this function when its state changes
// the widget will update the smosSrrTimeout variable
BLYNK_WRITE(V31)
		{
		int val = param.asInt();
		mb.setSmosSrrTimeout(val);
		}

// a Numeric Input widget in the app on (V32) calls this function when its state changes
// the widget will update the smosSrrSerial variable for motherboard A
BLYNK_WRITE(V32)
		{
		int val = param.asInt();
		mbA.setSmosSrrSerial(val);
		}

// a Numeric Input widget in the app on (V33) calls this function when its state changes
// the widget will update the smosSrrSlot variable for motherboard A
BLYNK_WRITE(V33)
		{
		int val = param.asInt();
		mbA.setSmosSrrSlot(val);
		}

// a Numeric Input widget in the app on (V36) calls this function when its state changes
// the widget will update the smosSrrSerial variable for motherboard B
BLYNK_WRITE(V36)
		{
		int val = param.asInt();
		mbB.setSmosSrrSerial(val);
		}

// a Numeric Input widget in the app on (V37) calls this function when its state changes
// the widget will update the smosSrrSlot variable for motherboard B
BLYNK_WRITE(V37)
		{
		int val = param.asInt();
		mbB.setSmosSrrSlot(val);
		}

// this function will be called by a timer (timer.setInterval(60000L, readSHT30Sensor)) in the setup() function
// data will be sent to V42 (temperature) and V43 (humidity) widgets in the app
void readSHT30Sensor()
	{                                                                  // function: reads SHT30 sensor and sends data to blynk app
	sht30.get();                                                       // get data from sensor
  	Blynk.virtualWrite(V42, sht30.cTemp);                              // send temperature to app
  	Blynk.virtualWrite(V43, sht30.humidity);                           // send humidity to app
  	/*
  	terminal.print("Temperature: ");
  	terminal.print(sht30.cTemp);
  	terminal.print("c   Humidity: ");
  	terminal.print(sht30.humidity);
  	terminal.println("%");
  	terminal.flush();
  	*/
  	}

// turn heartbeat LED widget A OFF 
void turnHeartbeatBlynkLedAOff()
  	{
  	heartbeatLedA.setValue(35);                                                			   // send OFF state to Blynk heartbeat A LED (V3)
  	}

// turn heartbeat LED widget B OFF   
void turnHeartbeatBlynkLedBOff()
  	{
  	heartbeatLedB.setValue(35);                                                			   // send OFF state to Blynk heartbeat A LED (V3)
  	}

// the (global) heartbeatFlags are set by the SMOS_SRR_UPD routine when valid keep-alive messages are received (approx every 2 seconds).
// this function will be called by a timer (timer.setInterval(5000L, flashHeartbeats)) in the setup() function.
// it sends a message to the LED widget to turn ON and starts a timer to turn it OFF again.
// it then resets the heartbeatFlag.
// uses 2 instances of the WidgetLED object, WidgetLED heartbeatLedA(V3) and WidgetLED heartbeatLedB(V7) and their setValue() methods.
void flashHeartbeats()                                                            				   // if heartbeat flags have been reset by SMOS SRR Watchdog UPD packets, flash the leds in Blynk app
  	{
  	if(mbA.getHeartbeatFlag() == true)															   // if the heartbeat flag for motherboard A is set
    	{
    	heartbeatLedA.setValue(255);                                             		   	   	   // send ON state to Blynk heartbeat A LED (V3)
    	timer.setTimeout(500, turnHeartbeatBlynkLedAOff);                             			   // Callback to turn LED back off after x mSeconds
    	mbA.setHeartbeatFlag(false);                                                       		   // set flag Off 
    	}
  	if(mbB.getHeartbeatFlag() == true)															   // if the heartbeat flag for motherboard B is set
    	{
    	heartbeatLedB.setValue(255);                                             			   	   // send ON state to Blynk heartbeat B LED (V7)
    	timer.setTimeout(500, turnHeartbeatBlynkLedBOff);                             			   // Callback to turn LED back off after x mSeconds
    	mbB.setHeartbeatFlag(false);                                                       		   // set flag Off 
    	}
  	}

// Terminal Commands 
void terminalCommands()
  {
  terminal.println(F("Commands:"));
  terminal.println(F("clr - clear the Log"));
  terminal.println(F("env - log temperature and humidity"));
  terminal.println(F("net - log SSID and IP"));
  terminal.println(F("rst - Restart the ESP8266"));
  terminal.println(F("?   - print this list"));
  }

// Blynk Terminal
BLYNK_WRITE(V10)
  {
  //terminal.println();
  if(String("?") == param.asStr())
    {
    terminalCommands();
    }
  else if(String("env") == param.asStr())
    {
    terminal.println(dateAndTime());
    sht30.get();
    terminal.print(F("Temperature: "));
    terminal.print(sht30.cTemp);
    terminal.print(F("c   Humidity: "));
    terminal.print(sht30.humidity);
    terminal.println(F("%"));
    terminal.flush();
    }
  else if(String("net") == param.asStr())
    {
    terminal.println(dateAndTime());
    terminal.print(F("Connected to: "));
    terminal.println(WiFi.SSID());
    terminal.print(F("IP address: "));
    terminal.println(WiFi.localIP());
    }
    else if(String("clr") == param.asStr())
    {
    terminal.clear();                                                         					   // Clear the terminal content
    }
  else if(String("rst") == param.asStr())
    {
    terminal.println();
    terminal.println(dateAndTime());
    terminal.println(F("Restarting the ESP8266..."));
    terminal.flush();
    delay(500);                                           										   // delay to allow terminal message to send
    ESP.restart();                                       										   // restart the ESP8266 https://techtutorialsx.com/2017/12/29/esp8266-arduino-software-restart/
    }
  else
    {
    //terminalCommands(); 
    /*
    // Send it back
    terminal.println(dateAndTime());
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
    */
    }

  // Ensure everything is sent
  terminal.flush();
  }
  
// log startup info to terminal
void logStartup()                                     
  {
  terminal.println();
  terminal.println(dateAndTime());
  terminal.print(F("Rig Resetter "));
  terminal.print(VERSION);
  terminal.println(F(" started"));
  terminal.print(F("Connected to: "));
  terminal.println(WiFi.SSID());
  terminal.print(F("IP address: "));
  terminal.println(WiFi.localIP());
  terminal.println(F("type '?' for list of commands"));
  //terminalCommands();                                                                 		   // print list of Terminal commands
  terminal.println();
  terminal.flush();                                                                     		   // Ensure everything is sent
  }

// log the states of the Motherboard Power LEDs on startup
void logMBPowerLedStates()
  {
  bool stateA = mbA.getPowerLedState();
  bool stateB = mbB.getPowerLedState();
  #ifdef TERMINAL_OUT
    terminal.print(dateAndTime());
    terminal.print(F(" MB A Power Led "));
    terminal.println(stateA ? "On" : "Off");
    terminal.print(dateAndTime());
    terminal.print(F(" MB B Power Led "));
    terminal.println(stateB ? "On" : "Off");
    terminal.flush();
  #endif
  }

#endif /* BLYNK_ROUTINES_H */