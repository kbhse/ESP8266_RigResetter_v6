/* SmosSrrUdp.h */

#ifndef SmosSrrUdp_H   /* include guards */
#define SmosSrrUdp_H

    #include <Arduino.h>
    #include "SimpleTimer.h"                                                                       // use SimpleTimer (instead of BlynkTimer in BlynkSimpleEsp8266.h) so I can include header in multiple files
    #include <ESP8266WiFi.h>
    #include "motherboard.h"

    //extern BlynkTimer timer;
    extern SimpleTimer timer;

    extern Motherboard mb;
    extern Motherboard mbA;
    extern Motherboard mbB;

    extern PCF8574 pcfMP;                                       // NB  the definition in auxiliary.cpp is 'PCF8574 pcfP(0x38)' but the extern declaration here shouldn't include (0x38) !!

    extern int wd_timer_A_id;                                                                                   // ids of watchdog timers
    extern int wd_timer_B_id;
    //extern WidgetTerminal terminal;

    void wdACallback();
    void listenForSmosUdpPackets();
    void flashHeartbeatLedA();
    void turnLedAOff();
    void flashHeartbeatLedB();
    void turnLedBOff();
    int translateSmosSrrByte(int x);

#endif // SmosSrrUdp_H