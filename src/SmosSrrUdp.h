/* SmosSrrUdp.h */

#ifndef SmosSrrUdp_H   /* include guards */
#define SmosSrrUdp_H

    // The only time you should include a header within another .h file is if you need to access a type definition in that header
    #include <Arduino.h>
    
    void listenForSmosUdpPackets();
    int translateSmosSrrByte(int x);

#endif // SmosSrrUdp_H