/* SmosSrrUdp.cpp */

#include <Arduino.h>
#include "SmosSrrUdp.h"
#include "ESPAsyncUDP.h"                                                                           // https://github.com/me-no-dev/ESPAsyncUDP
#include "defines.h"


AsyncUDP udp;                                                                                      // create instance of AsyncUPD object

//--------------------------------- Watchdog callback for motherboard A ----------------------------------------------------------- --------------------------
void wdACallback()

// alert user or perform action to restore
// program state (e.g. reset the microprocessor)

  {
  #ifdef TERMINAL_OUT
    terminal.print(dateAndTime());
    terminal.print(F(" MB A watchdog timeout!"));
    terminal.flush();
  #endif
  if(mbA.getAutoRestartFlag())                                                                                 // if motherboard A auto-restart is selected
    {
    #ifdef TERMINAL_OUT
      terminal.print(dateAndTime());
      terminal.print(F(" Auto-Reset MB A..."));
      terminal.flush();
    #endif
    // code to reset motherboard A
    }
  }


//-----------------------------------------------------------
// (approx every 2 seconds)

void listenForSmosUdpPackets()
    {
    #ifdef DEBUG_OUT
        Serial.println(F("listening for SMOS SRR watchdog keep alive UDP packets"));
    #endif
    if(udp.listen(mb.getSmosSrrUdpPort()))
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
                if(smosSrrSerial == mbA.getSmosSrrSerial())                                                            // if the serial number in the packet (set in SMOS SRR) matches serialA (as set in Blynk app)
                    {
                    flashHeartbeatLedA();                                                                         // flash the heartbeat LED (pin 3 on MP PCF8574 is heartbeat A Led)
                    timer.restartTimer(wd_timer_A_id);                                                            // restart the watchdog timer for motherboard A
                    }
                if(smosSrrSerial == mbB.getSmosSrrSerial())                                                            // if the serial number in the packet (set in SMOS SRR) matches serialB (as set in Blynk app)
                    {
                    flashHeartbeatLedB();                                                                         // flash the heartbeat LED (pin 7 on MP PCF8574 is heartbeat B Led)
                    }
                }
      
            //Serial.println(packet.data()[8]);
            //Serial.println(translateSmosSrrByte(packet.data()[8]));
      
            }); // end of callback function
        }
    } // end of listenForSmosUdpPackets

//-----------------------------------------------------------
void flashHeartbeatLedA()
    {
    pcfMP.write(3, LOW);                                                                                  // turn motherboard A heartbeat pcb LED ON (LOW is ON)
    mbA.setHeartbeatFlag(true);                                                                           // set heartbeatFlag for motherboard A
    //heartbeatA.setValue(255);                                                                           // send ON state to Blynk heartbeat A LED (V3)
    timer.setTimeout(150, turnLedAOff);                                                                   // Callback to turn LED back off after x mSeconds
    }

//-----------------------------------------------------------
void turnLedAOff()
    {
    pcfMP.write(3, HIGH);                                                                                 // turn motherboard A heartbeat pcb LED ON (HIGH is OFF)
    //heartbeatA.setValue(35);                                                                            // send OFF state to Blynk heartbeat A LED (V3)
    }

//-----------------------------------------------------------
void flashHeartbeatLedB()
    {
    pcfMP.write(7, LOW);                                                                                   // turn motherboard heartbeat LED ON
    mbB.setHeartbeatFlag(true);                                                                            // set heartbeatFlag for motherboard B
    timer.setTimeout(150, turnLedBOff);                                                                    // Callback to turn LED back off after x mSeconds
    }

//-----------------------------------------------------------
void turnLedBOff()
    {
    pcfMP.write(7, HIGH);                                                                                   // HIGH is OFF
    }


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