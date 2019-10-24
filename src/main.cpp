/*  src/main.cpp */

#define PROGNAM "ESP8266_RigResetter"                                                              // program name
#define VERSION "v6.010"                                                                           // program version (nb lowercase 'version' is keyword)
#define PGMFUNCT "Remotely power-cycle a crypto mining rig"                                        // what the program does
#define HARDWARE "Rig Resetter, ESP8266 Breakout, v6.0"                                            // hardware version
#define AUTHOR "J Manson"                                                                          // created by
#define CREATION_DATE "October 2019"                                                               // date

#include <Arduino.h>
#include "defines.h"
#include <ESP8266WiFi.h>
#include "BlynkSimpleEsp8266.h"                                                                    // can only put this header in 1 place 'cause it includes definitions !
#include "blynk_routines.h"                                                                        // NB this must come AFTER #include "BlynkSimpleEsp8266.h" !
#include "ESPConfigurationAP.h"
#include "SimpleTimer.h"                                                                           // use SimpleTimer (instead of BlynkTimer in BlynkSimpleEsp8266.h) so I can include header in multiple files
#include "SmosSrrUdp.h"
#include "TimeLib.h"                                                                               // https://github.com/PaulStoffregen/Time
#include <WidgetRTC.h>


// NB remove credentials when WiFi Manager enabled !!
/*
const char* ssid = "***********";                                                                  // WiFi credentials, SSID
const char* password = "********************";                                                     // WiFi credentials, password
char auth[] = "********************************";                                                  // Blynk app Auth Token (get from Project Settings (nut icon))
*/

//BlynkTimer timer;
SimpleTimer timer;                                                                                 // use SimpleTimer (instead of BlynkTimer in BlynkSimpleEsp8266.h) so I can include header in multiple files

WidgetRTC rtc;

int wd_timer_A_id;                                                                                 // ids of watchdog timers
int wd_timer_B_id;

BLYNK_CONNECTED()
    {
    // Synchronize time on connection
    rtc.begin();
    }



void setup()
    {

    Serial.begin(115200);
    Serial.printf("\n\n%s_%s, %s, %s\n\n", PROGNAM, VERSION, AUTHOR, CREATION_DATE);

    mbA.pcf8574_init();                                                                            // start the PCF8574 devices and I2C
                                                                                                   // NB only needs called for 1 PCF8574 object to call Wire.begin (I2C) in PCF8574.cpp

    setPortPins();                                                                                 // set the PCF8574 port pins

    auxOutP6.setAuxOutState(LOW);                                                                  // turn auxOutP6 (and associated pcb led) OFF
    pcbPowerLed.setAuxOutState(LOW);                                                               // turn OFF pcb 5V power led until system connected TO wIfI. active HIGH

    boolean configSwitch = auxInP7.getAuxInState();
    #ifdef DEBUG_OUT
        Serial.print(F("configSwitch: "));
        Serial.println(configSwitch ? "On" : "Off");
    #endif
    readParamsFromFSJson(configSwitch);

    if(configSwitch)                                                                               // if configSwitch is ON (nb replace with pcfP.readButton(7) later)
        {
        wiFiManagerDo();                                                                           // run WiFi Manager on-demand config portal
        }
    else
        {
        connectToLastUsedAccessPoint();                                                            // connect to the last used WiFi access point using configuration saved in flash memory by SDK
        }

    pcbPowerLed.setAuxOutState(HIGH);                                                              // connected to WiFi network, turn ON pcb 5V power led

    //Blynk.begin(auth, ssid, password);
    //Blynk.connect();

    Serial.println(F("connecting to Blynk server"));
    Blynk.config(blynk_token);
    while(Blynk.connect() != true) {};
    rtc.begin();                                                                                   // after changing from SSL to basic I need this here to get first dateAndTime for terminal ! (it's also in BLYNK_CONNECTED() above)

    timer.setInterval(60000L, readSHT30Sensor);                                                    // every 60 seconds, send temp and humidity to blync server
    timer.setInterval(500L, getPcbInputs);                                                         // timer to poll the pcb inputs states
    timer.setInterval(5000L, flashHeartbeats);                                                     // if heartbeat flags have been reset by SMOS SRR Watchdog UPD packets, flash the led in Blynk app



    setSyncInterval(10 * 60);                                                                      // Time Sync interval in seconds (10 minutes)

    Blynk.syncVirtual(V1, V5, V30, V31);                                                                // kludge - but syncing a Vpin here means restart date/time is printed to terminal, else 01/01/1970
                                                                                                   // just calling the function (with an unallocated Vpin) works
                                                                                                   // but I need to sync V0 and V1 for logMBPowerLedStates(), so do it here
                                                                                                   // and I need to sync V31 so smosSrrTimeout is updated in time for wd_timer_A
                                                                                                   // and V30 to get smosSrrUdpPort before listenForSmosUdpPackets() called

    logStartup();                                                                                  // log startup info to terminal

    logMBPowerLedStates();                                                                         // log motherboard Power LED states to terminal on startup
  
    Blynk.syncAll();                                                                               // Blynk will sync virtual pins when changed in app but need this to initially sync values on startup
                                                                                                   // The Blynk.syncAll() command restores all the Widget’s values based on the last saved values on the server. 
                                                                                                   // All analog and digital pin states will be restored. Every Virtual Pin will perform BLYNK_WRITE event.
                                                                                                   // You can also update a single Virtual Pin value by calling Blynk.syncVirtual(Vpin)
                                                                                                   // or you can update several pins with Blynk.syncVirtual(V0, V1, V2, ...).
                                                                                                   // the calls to BLYNK_WRITE from Blynk.syncAll() will log state of Fans and Aux Output to Terminal on startup

    long timeout = mb.getSmosSrrTimeout() * 60000;
    wd_timer_A_id = timer.setInterval(timeout, wdACallback);                                       // start the watchdog timer for motherboard A
    wd_timer_B_id = timer.setInterval(timeout, wdACallback);                                       // start the watchdog timer for motherboard B
    
    listenForSmosUdpPackets();                                                                     // start callback function that listens for UPD broadcast packets from SMOS SRR watchdogs

    }// end of setup()


void loop()
    {
    Blynk.run();
    timer.run();
    } // end of loop()