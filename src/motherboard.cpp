/*  src/motherboard.cpp */

#include <Arduino.h>
#include "motherboard.h"
#include <PCF8574.h>                                                                               // https://github.com/RobTillaart/Arduino/tree/master/libraries/PCF8574

/*
    bool _powerLedState;
    int _powerLedPin;
    bool _powerSwitchState;
    int _powerSwitchPin;
    bool _resetSwitchState;
    int _resetSwitchPin;
    bool _heartbeatLedState;
    int _heartbeatLedPin;
    bool _autoRestartFlg;
*/

PCF8574 pcfMP(0x39);                                                                               // create instance of the PCF8574 class for the MP device

void Motherboard::pcf8574_init()
    {
    pcfMP.begin();                                                                                 // start the PCF8574 devices and I2C
    }                                                                                              // NB only needs called for 1 PCF8574 object to call Wire.begin (I2C) in PCF8574.cpp

// constructor
/*
Motherboard::Motherboard()
    {
    pcfMP.begin();       
    }
*/

// Getters

boolean Motherboard::getPowerSwitchState()
    {
    return _powerSwitchState;
    }

boolean Motherboard::getPowerLedState()
    {
    _powerLedState = !pcfMP.readButton(_powerLedPin);
    return _powerLedState;
    }

boolean Motherboard::getResetSwitchState()
    {
     return _resetSwitchState;
    }

boolean Motherboard::getAutoRestartFlg()
    {
    return _autoRestartFlg;
    }

// Setters

void Motherboard::setPowerSwitchPin(int pin)
    {
    _powerSwitchPin = pin;
    }

void Motherboard::setPowerLedPin(int pin) {_powerLedPin = pin;}

void Motherboard::setResetSwitchPin(int pin) {_resetSwitchPin = pin;}

void Motherboard::setHeartbeatLedPin(int pin) {_heartbeatLedPin = pin;}


void Motherboard::setPowerSwitchState(boolean state)
    {
    _powerSwitchState = state;
    pcfMP.write(_powerSwitchPin, !_powerSwitchState);                                              // active LOW
    }

void Motherboard::setResetSwitchState(boolean state)
    {
    _resetSwitchState = state;
    pcfMP.write(_resetSwitchPin, !_resetSwitchState);                                              // active LOW
    }

void Motherboard::setHeartbeatLedState(boolean state)
    {
    _heartbeatLedState = state;
    }
void Motherboard::setAutoRestartFlg(boolean state)
    {
    _autoRestartFlg = state;
    }
