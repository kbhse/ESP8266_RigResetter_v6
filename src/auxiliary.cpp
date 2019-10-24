/* src/auxiliary.cpp */

#include <Arduino.h>
#include "auxiliary.h"

PCF8574 pcfP(0x38);                                                                                // create instance of the PCF8574 class for the P device

// Getters

boolean AuxOut::getAuxOutState()
    {
    return _auxOutState;
    }

boolean AuxOut::getActiveHIGH()
    {
    return _activeHIGH;
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
    if(_activeHIGH)
        {
        pcfP.write(_auxOutPin, _auxOutState);
        }
    else
        {
        pcfP.write(_auxOutPin, !_auxOutState);
        }
    
    }

void AuxOut::setActiveHIGH(boolean state)
    {
    _activeHIGH = state;
    }

void AuxIn::setAuxInPin(int pin)
    {
    _auxInPin = pin;
    }