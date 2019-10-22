/*  src/motherboard.h */

#ifndef MOTHERBOARD_H   /* include guards */
#define MOTHERBOARD_H

    #include <Arduino.h>

    class Motherboard
        {
        private:

            int _powerSwitchPin;
            boolean _powerSwitchState;
            int _powerLedPin;
            boolean _powerLedState;
            int _resetSwitchPin;
            boolean _resetSwitchState;
            int _heartbeatLedPin;
            boolean _heartbeatLedState;
            boolean _autoRestartFlg;
    
        public:

            void pcf8574_init();                                                                   // start the PCF8574 MP device (pcfMP.begin())
                                                                                                   // NB only needs called by 1 Motherboard object eg mbA
            /*
            // Constructor
            Motherboard();
            */

            // Getters

            boolean getPowerSwitchState();
            boolean getPowerLedState();
            boolean getResetSwitchState();
            boolean getAutoRestartFlg();

            // Setters

            void setPowerSwitchPin(int pin);
            void setPowerLedPin(int pin);
            void setResetSwitchPin(int pin);
            void setHeartbeatLedPin(int pin);

            void setPowerSwitchState(boolean state);
            void setResetSwitchState(boolean state);
            void setHeartbeatLedState(boolean state);
            void setAutoRestartFlg(boolean state);

        };

#endif /* MOTHERBOARD_H */