/*  src/motherboard.h */

#ifndef MOTHERBOARD_H   /* include guards */
#define MOTHERBOARD_H

    #include <Arduino.h>
    #include "PCF8574.h"

    extern PCF8574 pcfMP;

    class Motherboard
        {
        private:

            int _powerSwitchPin;
            int _powerLedPin;
            int _resetSwitchPin;
            int _heartbeatLedPin;

            boolean _powerSwitchState;
            boolean _powerLedState;
            boolean _resetSwitchState;
            boolean _heartbeatLedState;
            boolean _autoRestartFlag;

            int _smosSrrUdpPort;
            int _smosSrrTimeout;
            int _smosSrrSerial;
            int _smosSrrSlot;

            boolean _heartbeatFlag;
    
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
            boolean getheartbeatLedState();
            boolean getAutoRestartFlag();

            int getSmosSrrUdpPort();
            int getSmosSrrTimeout();
            int getSmosSrrSerial();
            int getSmosSrrSlot();

            boolean getHeartbeatFlag();

            // Setters

            void setPowerSwitchPin(int pin);
            void setPowerLedPin(int pin);
            void setResetSwitchPin(int pin);
            void setHeartbeatLedPin(int pin);

            void setPowerSwitchState(boolean state);
            void setResetSwitchState(boolean state);
            void setHeartbeatLedState(boolean state);
            void setAutoRestartFlag(boolean state);

            void setSmosSrrUdpPort(int val);
            void setSmosSrrTimeout(int val);
            void setSmosSrrSerial(int val);
            void setSmosSrrSlot(int val);

            void setHeartbeatFlag(boolean state);

        };

#endif /* MOTHERBOARD_H */