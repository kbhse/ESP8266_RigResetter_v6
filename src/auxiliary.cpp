/* src/auxiliary.cpp */

#include <Arduino.h>
#include "auxiliary.h"

PCF8574 pcfP(0x38);                                                                                // create instance of the PCF8574 class for the P device

// Getters

boolean AuxOut::getAuxOutState()
    {
    return _auxOutState;
    }

boolean AuxIn::getAuxInState()
    {
    _auxInState = !pcfP.readButton(_auxInPin);
        return _auxInState;
    }

// Setters

void AuxOut::setAuxOutPin(int pin)
    {
    _auxOutPin = pin;
    }

void AuxOut::setAuxOutState(boolean state)
    {
    _auxOutState = state;
    pcfP.write(_auxOutPin, !_auxOutState);                                                         // active LOW
    }

void AuxIn::setAuxInPin(int pin)
    {
    _auxInPin = pin;
    }