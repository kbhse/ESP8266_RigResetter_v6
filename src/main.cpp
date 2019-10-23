/*  src/main.cpp */

#define PROGNAM "ESP8266_RigResetter"                                                              // program name
#define VERSION "v6.004"                                                                           // program version (nb lowercase 'version' is keyword)
#define PGMFUNCT "Remotely power-cycle a crypto mining rig"                                        // what the program does
#define HARDWARE "Rig Resetter, ESP8266 Breakout, v6.0"                                            // hardware version
#define AUTHOR "J Manson"                                                                          // created by
#define CREATION_DATE "October 2019"                                                               // date

#define DEBUG_OUT                                                                                  // debug to serial monitor

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "BlynkSimpleEsp8266.h"                                                                    // can only put this header in 1 place 'cause it includes definitions !
#include "blynk_routines.h"                                                                        // NB this must come AFTER #include "BlynkSimpleEsp8266.h" !
#include "ESPConfigurationAP.h"
#include "SimpleTimer.h"                                                                           // use SimpleTimer (instead of BlynkTimer in BlynkSimpleEsp8266.h) so I can include header in multiple files
#include "SmosSrrUdp.h"

// NB remove credentials when WiFi Manager enabled !!
const char* ssid = "BTHub4-5H9P";                                                                  // WiFi credentials, SSID
const char* password = "nB67c3zuRlPrAVcZL5YN";                                                     // WiFi credentials, password
char auth[] = "9lRwox3I34LL9PADxu3CtNyMHdT4apsm";                                                  // Blynk app Auth Token (get from Project Settings (nut icon))

//BlynkTimer timer;
SimpleTimer timer;                                                                                 // use SimpleTimer (instead of BlynkTimer in BlynkSimpleEsp8266.h) so I can include header in multiple files

int wd_timer_A_id;                                                                                 // ids of watchdog timers
int wd_timer_B_id;

void setup()
    {

    Serial.begin(115200);
    Serial.printf("\n\n%s_%s, %s, %s\n\n", PROGNAM, VERSION, AUTHOR, CREATION_DATE);

    mbA.pcf8574_init();                                                                            // start the PCF8574 devices and I2C
                                                                                                   // NB only needs called for 1 PCF8574 object to call Wire.begin (I2C) in PCF8574.cpp

    setPortPins();                                                                                 // set the PCF8574 port pins

    timer.setInterval(500L, getPcbInputs);                                                         // timer to poll the pcb inputs states

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

    listenForSmosUdpPackets();                                                                     // start callback function that listens for UPD broadcast packets from SMOS SRR watchdogs
    
    Blynk.begin(auth, ssid, password);
    //Blynk.connect();



    long timeout = mb.getSmosSrrTimeout() * 60000;
    wd_timer_A_id = timer.setInterval(timeout, wdACallback);                      // start the watchdog timer for motherboard A

    }// end of setup()


void loop()
    {
    Blynk.run();
    timer.run();
    } // end of loop()