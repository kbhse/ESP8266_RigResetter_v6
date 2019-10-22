/* SmosSrrUdp.h */

#ifndef SmosSrrUdp_H   /* include guards */
#define SmosSrrUdp_H

    #include <Arduino.h>
    //#include "BlynkSimpleEsp8266.h" 
    #include "SimpleTimer.h"                                                                       // use SimpleTimer (instead of BlynkTimer in BlynkSimpleEsp8266.h) so I can include header in multiple files
    #include <ESP8266WiFi.h>
    
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