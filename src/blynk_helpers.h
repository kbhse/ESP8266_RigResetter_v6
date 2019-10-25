/* blynk_helpers.h */

#ifndef blynk_helpers_H   /* include guards */
#define blynk_helpers_H

// blynk_routines helper functions
// -------------------------------

// set the pins of the PCF8574 MP device
// and the PCF8574 P device
// why is this in blynk_routines ???
void setPortPins();

// read the state of the pcb inputs
// called by timer (timer.setInterval(500L, getPcbInputs)) in setup()
// calls: getPowerLedAState(), getPowerLedBState(), getAuxInputState()
void getPcbInputs();

// reads the SHT30 sensor
// called by a timer (timer.setInterval(60000L, readSHT30Sensor)) in the setup() function
// data will be sent to V42 (temperature) and V43 (humidity) widgets in the app
void readSHT30Sensor();

// the (global) heartbeatFlags are set by the SMOS_SRR_UPD routine when valid keep-alive messages are received (approx every 2 seconds).
// this function will be called by a timer (timer.setInterval(5000L, flashHeartbeats)) in the setup() function.
// it sends a message to the LED widget to turn ON and starts a timer to turn it OFF again.
// it then resets the heartbeatFlag.
// uses 2 instances of the WidgetLED object, WidgetLED heartbeatLedA(V3) and WidgetLED heartbeatLedB(V7) and their setValue() methods.
void flashHeartbeats();

// log startup info to terminal
void logStartup();

// log the states of the Motherboard Power LEDs on startup
void logMBPowerLedStates();

void wdACallback();

void wdBCallback();

// called from SmosSrrUdp.cpp
void flashHeartbeatLedA();

// called from SmosSrrUdp.cpp
void flashHeartbeatLedB();

// 

#endif // blynk_helpers_H