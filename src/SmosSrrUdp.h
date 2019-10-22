/* SmosSrrUdp.h */

#ifndef SmosSrrUdp_H   /* include guards */
#define SmosSrrUdp_H

    #include <Arduino.h>
    //#include "BlynkSimpleEsp8266.h" 
    #include "SimpleTimer.h"                                                                       // use SimpleTimer (instead of BlynkTimer in BlynkSimpleEsp8266.h) so I can include header in multiple files
    #include <ESP8266WiFi.h>
    #include "motherboard.h"

    //extern BlynkTimer timer;
    extern SimpleTimer timer;
    extern Motherboard mbA;
    extern Motherboard mbB;

    void wdACallback();

    void listenForSmosUdpPackets();

    void flashHeartbeatLedA();

    void turnLedAOff();

    void flashHeartbeatLedB();

    void turnLedBOff();

    int translateSmosSrrByte(int x);

#endif // SmosSrrUdp_H