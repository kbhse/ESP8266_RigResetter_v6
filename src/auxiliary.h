/*  src/auxiliary.h */

#ifndef AUXILIARY_H   /* include guards */
#define AUXILIARY_H

    #include <Arduino.h>
    #include "PCF8574.h"                                                                           // https://github.com/RobTillaart/Arduino/tree/master/libraries/PCF8574

    extern PCF8574 pcfP;                                       // NB  the definition in auxiliary.cpp is 'PCF8574 pcfP(0x38)' but the extern declaration here shouldn't include (0x38) !!

    class AuxOut
        {
        private:

            int _auxOutPin;
            boolean _auxOutState;
            boolean _activeHIGH;
            
        public:

            // Getters

            boolean getAuxOutState();
            boolean getActiveHIGH();

            // Setters

            void setAuxOutPin(int pin);

            void setAuxOutState(boolean state);
            void setActiveHIGH(boolean state);

        };

    class AuxIn
        {
        private:

            int _auxInPin;
            boolean _auxInState;
            
        public:

            // Getters

            boolean getAuxInState();

            // Setters

            void setAuxInPin(int pin);

        };

#endif /* AUXILIARY_H */