/* blynk_helpers.h */

#ifndef blynk_helpers_H   /* include guards */
#define blynk_helpers_H

#include <Arduino.h>

// blynk_routines helper functions
// -------------------------------

// set the pins of the PCF8574 MP device
// and the PCF8574 P device
// why is this in blynk_routines ???
void setPortPins();

// called from: main.cpp
// read the state of the pcb inputs
// called by timer (timer.setInterval(500L, getPcbInputs)) in setup()
// calls: getPowerLedAState(), getPowerLedBState(), getAuxInputState()
void getPcbInputs();

// called from: blynk_routines.h
// get state of motherboard A power led and send to WidgetLED PowerLedA
// called (via timer) by getPcbInputs()
// if the state has changed print to Blynk Terminal (ie don't print every time timer calls it)
void getPowerLedAState();

// called from: blynk_routines.h
// get state of motherboard B power led and send to WidgetLED PowerLedAConfigSwitchLed
// called (via timer) by getPcbInputs()
// if the state has changed print to Blynk Terminal (ie don't print every time timer calls it)
void getPowerLedBState();

// called from: blynk_routines.h
// get state of Aux Input P7 and send to WidgetLED ConfigSwitchLed
// called (via timer) by getPcbInputs()
void getAuxInputState();

// called by timer defined in main.cpp
// reads the SHT30 sensor
// called by a timer (timer.setInterval(60000L, readSHT30Sensor)) in the setup() function
// data will be sent to V42 (temperature) and V43 (humidity) widgets in the app
void readSHT30Sensor();

// called by timer defined in main.cpp
// the (global) heartbeatFlags are set by the SMOS_SRR_UPD routine when valid keep-alive messages are received (approx every 2 seconds).
// called by a timer (timer.setInterval(5000L, flashHeartbeats)) in the setup() function.
// it sends a message to the LED widget to turn ON and starts a timer to turn it OFF again.
// it then resets the heartbeatFlag.
// uses 2 instances of the WidgetLED object, WidgetLED heartbeatLedA(V3) and WidgetLED heartbeatLedB(V7) and their setValue() methods.
void flashHeartbeats();

// called from: blynk_routines.h
// turn heartbeat LED widget A OFF 
void turnHeartbeatBlynkLedAOff();

// called from: blynk_routines.h
// turn heartbeat LED widget B OFF   
void turnHeartbeatBlynkLedBOff();

// called from: main.cpp
// log startup info to terminal
void logStartup();

// called from: main.cpp
// log the states of the Motherboard Power LEDs on startup
void logMBPowerLedStates();

// called by timer defined in main.cpp
// Watchdog callback for motherboard A
// alert user or perform action to restore
// program state (e.g. reset the microprocessor)
void wdACallback();

// called by timer defined in main.cpp
// Watchdog callback for motherboard B
// alert user or perform action to restore
// program state (e.g. reset the microprocessor)
void wdBCallback();

// called from SmosSrrUdp.cpp
// turn motherboard A heartbeat pcb LED ON
// timer Callback to turn LED back off after x mSeconds
// restart the watchdog timer for motherboard A
// set heartbeatFlag for motherboard A
void flashHeartbeatLedA();

// called by timer defined in blynk_routines.h
// turn motherboard A heartbeat pcb LED OFF
void turnLedAOff();

// called from SmosSrrUdp.cpp
// turn motherboard B heartbeat pcb LED ON
// timer Callback to turn LED back off after x mSeconds
// restart the watchdog timer for motherboard B
// set heartbeatFlag for motherboard B
void flashHeartbeatLedB();

// called by timer defined in blynk_routines.h
// turn motherboard B heartbeat pcb LED OFF
void turnLedBOff();

#endif // blynk_helpers_H