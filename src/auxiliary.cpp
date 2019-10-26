/* src/auxiliary.cpp */

#include <Arduino.h>
#include "auxiliary.h"
#include "PCF8574.h"
#include "defines.h"

PCF8574 pcfP(0x38);                                                                                // create instance of the PCF8574 class for the P device

AuxOut fan1;
AuxOut fan2;
AuxOut auxOutP6;                                                                                   // auxiliary output on PCF8574 P6
AuxOut pcbPowerLed;                                                                                // the 5V pcb power led
AuxIn auxInP7;                                                                                     // auxiliary input on PCF8574 P7 (CONFIG switch)

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