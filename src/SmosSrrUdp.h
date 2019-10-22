/* SmosSrrUdp.h */

#ifndef SmosSrrUdp_H   /* include guards */
#define SmosSrrUdp_H

    #include <Arduino.h>

    void wdACallback();

    void listenForSmosUdpPackets();

    void flashHeartbeatLedA();

    void turnLedAOff();

    void flashHeartbeatLedB();

    void turnLedBOff();

    int translateSmosSrrByte(int x);

#endif // SmosSrrUdp_H