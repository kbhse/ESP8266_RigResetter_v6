/* SmosSrrUdp.h */

#ifndef SmosSrrUdp_H   /* include guards */
#define SmosSrrUdp_H

    #include <Arduino.h>
    //#include "BlynkSimpleEsp8266.h" 
    #include "SimpleTimer.h"

    //extern BlynkTimer timer;
    extern SimpleTimer timer;

    void wdACallback();

    void listenForSmosUdpPackets();

    void flashHeartbeatLedA();

    void turnLedAOff();

    void flashHeartbeatLedB();

    void turnLedBOff();

    int translateSmosSrrByte(int x);

#endif // SmosSrrUdp_H