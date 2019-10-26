/* SmosSrrUdp.cpp */

#include <Arduino.h>
#include "SmosSrrUdp.h"                                                                            // include header so I can put functions here in any order
#include "ESPAsyncUDP.h"                                                                           // https://github.com/me-no-dev/ESPAsyncUDP
#include <ESP8266WiFi.h>
#include "PCF8574.h"
#include "motherboard.h"
#include "defines.h"
#include "blynk_helpers.h"

AsyncUDP udp;                                                                                      // create instance of AsyncUPD object

extern Motherboard mb;
extern Motherboard mbA;
extern Motherboard mbB;

extern PCF8574 pcfMP;                                       // NB  the definition in auxiliary.cpp is 'PCF8574 pcfP(0x38)' but the extern declaration here shouldn't include (0x38) !!

//-----------------------------------------------------------
// (approx every 2 seconds)

void listenForSmosUdpPackets()
    {
    #ifdef DEBUG_OUT
        Serial.println(F("listening for SMOS SRR watchdog keep alive UDP packets"));
    #endif
    if(udp.listen(mb.getSmosSrrUdpPort()))
    //if(udp.listen(1051))
        {
        Serial.printf("UDP Listening on IP: %s, Port: %d\n", WiFi.localIP().toString().c_str(), mb.getSmosSrrUdpPort());
        // CALLBACK FUNCTION
        udp.onPacket([](AsyncUDPPacket packet)                      
            {
        
            #ifdef DEBUG_OUT_UDP
                Serial.print(millis()/1000);                                                                            // print packet to serial monitor
                Serial.print(F(" "));
                Serial.print(F("UDP Packet Type: "));
                Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
                Serial.print(F(", From: "));
                Serial.print(packet.remoteIP());
                Serial.print(F(":"));
                Serial.print(packet.remotePort());
                Serial.print(F(", To: "));
                Serial.print(packet.localIP());
                Serial.print(F(":"));
                Serial.print(packet.localPort());
                Serial.print(F(", Length: "));
                Serial.print(packet.length());
                Serial.print(F(", Data: "));
                for (int i=0; i<packet.length(); i++)
                    {
                    Serial.printf(" %X", packet.data()[i]);                                                                // print data bytes in Hexidecimal
                    }
                Serial.println();
            #endif

            if(packet.length() == 12)
                {
                // get SMOS SRR serial number from UPD packet
                long smosSrrSerial;
                smosSrrSerial = translateSmosSrrByte(packet.data()[7]) * 10000;
                smosSrrSerial += translateSmosSrrByte(packet.data()[8]) * 100;
                smosSrrSerial += translateSmosSrrByte(packet.data()[9]);
                //Serial.println(smosSrrSerial);
                //Serial.println(smosSrrSerial_A);
                //Serial.println(smosSrrSerial_B);
                if(smosSrrSerial == mbA.getSmosSrrSerial())                                        // if the serial number in the packet (set in SMOS SRR) matches serialA (as set in Blynk app)
                    {
                    flashHeartbeatLedA();                                                          // flash the PCB heartbeat LED A and restart the watchdog timer (function in blynk_routines.h)
                    }
                if(smosSrrSerial == mbB.getSmosSrrSerial())                                        // if the serial number in the packet (set in SMOS SRR) matches serialB (as set in Blynk app)
                    {
                    flashHeartbeatLedB();                                                          // flash the PCB heartbeat LED B and restart the watchdog timer (function in blynk_routines.h)
                    }
                }
      
            //Serial.println(packet.data()[8]);
            //Serial.println(translateSmosSrrByte(packet.data()[8]));
      
            }); // end of callback function
        }
    } // end of listenForSmosUdpPackets

//-----------------------------------------------------------
int translateSmosSrrByte(int x)
    {
    int y;
    y = int(x / 16) * 10;
    y = y + (x % 16);
    return y;
    }

// take a byte from udp packet and convert hex to dec such that eg 0x45 will give 45 decimal (not 69!) - SMOS SSR encoding !!?*!
// method:
// convert hex to decimal (0x45 => 69)
// x = 69
// int(x/16) => 4
// x%16 => 5  modulus
// (4 * 10) + 5 = 45

//-----------------------------------------------------------