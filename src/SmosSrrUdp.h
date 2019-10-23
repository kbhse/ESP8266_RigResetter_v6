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

    extern Motherboard mb;
    extern Motherboard mbA;
    extern Motherboard mbB;

    extern int wd_timer_A_id;                                                                                   // ids of watchdog timers
    extern int wd_timer_B_id;

    void wdACallback();

    void listenForSmosUdpPackets();

    void flashHeartbeatLedA();

    void turnLedAOff();

    void flashHeartbeatLedB();

    void turnLedBOff();

    int translateSmosSrrByte(int x);

#endif // SmosSrrUdp_H