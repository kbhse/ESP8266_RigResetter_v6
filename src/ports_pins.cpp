/* ports_pins.cpp */

#include <Arduino.h>
#include "motherboard.h"
#include "auxiliary.h"

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

/*
Blynk Virtual Pins:

V0  Button          APP -> DEVICE   Motherboard A Power Switch
V1  LED             DEVICE -> APP   Motherboard A Power LED                 WidgetLED PowerLedA
V2  Button          APP -> DEVICE   Motherboard A Reset Switch
V3  LED             DEVICE -> APP   Heartbeat A LED                         WidgetLED heartbeatLedA
V4  Button          APP -> DEVICE   Motherboard B Power Switch
V5  LED             DEVICE -> APP   Motherboard B Power LED                 WidgetLED PowerLedB
V6  Button          APP -> DEVICE   Motherboard B Reset Switch
V7  LED             DEVICE -> APP   Heartbeat B LED                         WidgetLED heartbeatLedB
V10 Terminal                                                WidgetTerminal terminal
V11 Button          APP -> DEVICE   Motherboard A auto-restart
V12 Button          APP -> DEVICE   Motherboard B auto-restart
V15 Button          APP -> DEVICE   Aux Fan 1 (Relay1)
V16 Button          APP -> DEVICE   Aux Fan 2 (Relay2)
V19 Button          APP -> DEVICE   Aux Output
V22 LED             DEVICE -> APP   Aux Input connected to CONFIG Switch    WidgetLED ConfigSwitchLed
V30 Numeric Input   APP -> DEVICE   smosSrrUdpPort
V31 Numeric Input   APP -> DEVICE   smosSrrTimeout
V32 Numeric Input   APP -> DEVICE   smosSrrSerial A
V33 Numeric Input   APP -> DEVICE   smosSrrSlot A
V36 Numeric Input   APP -> DEVICE   smosSrrSerial A
V37 Numeric Input   APP -> DEVICE   smosSrrSlot B
V42 SuperChart      DEVICE -> APP   SHT30 Temperature
V43 SuperChart      DEVICE -> APP   SHT30 Humidity
V50 Labeled Value   DEVICE -> APP   message on screen
*/