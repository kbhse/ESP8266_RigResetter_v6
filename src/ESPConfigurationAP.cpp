/* ESPConfigurationAP.cpp */

// based on WiFiManager https://github.com/tzapu/WiFiManager
// doesn't use autoConnect
// cpnfigSwitch on pcb to startConfigPortal "ESPConfigurationAP"
// browse to 192.168.4.1 to set WiFi credentials and Blynk & SMOS parameters
// wifi credentials saved in flash memory by SDK
// parameters saved to SPIFFS by WiFiManager routine
//
// terminates on errors with statusLED flashing a number of 250ms flashes as follows:
	// statusLED(1, "readParamsFromFSJson() - failed to mount SPIFFS file system - restart in config mode");
	// statusLED(2, "readParamsFromFSJson() - config.json does not exist - restart in config mode");
	// statusLED(3, "readParamsFromFSJson() - the JsonObject was not parsed successfully - restart in config mode");
	// statusLED(4, "wiFiManagerDo() - failed to start the Configuration Portal");
	// statusLED(5, "wiFiManagerDo() - failed to open parameter file for writing");
	// statusLED(6, "wiFiManagerDo() - failed to mount SPIFFS file system");
	// statusLED(7, "wiFiManagerDo() - configuration complete, TURN CONFIG SWITCH OFF then restart system!")
	// continuous, 500ms delay, connecting to WiFi, run from connectToLastUsedAccessPoint()

#include "ESPConfigurationAP.h"
#include <FS.h>                                                                                    // this needs to be first, or it all crashes and burns...
#include <Arduino.h>
#include "WiFiManager.h"                                                                           // https://github.com/tzapu/WiFiManager
#include "ArduinoJson.h"                                                                           // https://github.com/bblanchon/ArduinoJson NB must be v5
#include "auxiliary.h"
#include "defines.h"

char blynk_token[34] = "BLYNK_TOKEN";
boolean shouldSaveConfig = false;

// --------------------------------------------------
void readParamsFromFSJson(boolean configSwitchFlag)                                                // configSwitchFlag used here to determine how to deal with errors when reading parameters from SPIFFS
	{                                                                                                // if the switch is ON then the configuration routine may fix the errors by creating the file
	//boolean success = true;                                                                          // so just report the error and proceed -this variable unused !
	//read parameters from FS json                                                                   // if the switch is OFF then halt the program with statusLED and recommend configuration
	Serial.println(F("mounting SPIFFS file system..."));
	if (SPIFFS.begin())
		{
		Serial.println(F("mounted"));
		if (SPIFFS.exists("/config.json"))
			{
			//file exists, reading and loading
			Serial.println(F("config file found"));
			File configFile = SPIFFS.open("/config.json", "r");
			if (configFile)
				{
				Serial.println(F("opened config file"));
				size_t size = configFile.size();
				// Allocate a buffer to store contents of the file.
				std::unique_ptr<char[]> buf(new char[size]);
				configFile.readBytes(buf.get(), size);
				DynamicJsonBuffer jsonBuffer;
				JsonObject& json = jsonBuffer.parseObject(buf.get());
				json.printTo(Serial);
				if (json.success())
					{
					Serial.println(F("\nparsed json"));
					
					//strcpy(mqtt_server, json["mqtt_server"]);
					//strcpy(mqtt_port, json["mqtt_port"]);
					strcpy(blynk_token, json["blynk_token"]);
					}
				else
					{
					if(!configSwitchFlag)                           
						{
						statusLED(3, "readParamsFromFSJson() - the JsonObject was not parsed successfully - restart in config mode");     // if configSwitch is OFF stop the program with statusLED and recommend configuration
						}
					Serial.println(F("readParamsFromFSJson() - the JsonObject was not parsed successfully"));                              // if configSwitch is ON just report the error and let it proceed (because configuration will may fix it)
					}
				configFile.close();
				}
			}
		else
			{
			if(!configSwitchFlag)                           
				{
				statusLED(2, "readParamsFromFSJson() - config.json does not exist - restart in config mode");                         // if configSwitch is OFF stop the program with statusLED and recommend configuration
				}
			Serial.println(F("readParamsFromFSJson() - config.json does not exist"));                                                  // if configSwitch is ON just report the error and let it proceed (because configuration will may fix it)
			}
		}
	else
		{
		if(!configSwitchFlag)                           
			{
			statusLED(1, "readParamsFromFSJson() - failed to mount SPIFFS file system - restart in config mode");                   // if configSwitch is OFF stop the program with statusLED and recommend configuration
			}
		Serial.println(F("readParamsFromFSJson() - failed to mount SPIFFS file system"));                                            // if configSwitch is ON just report the error and let it proceed (because configuration will may fix it)
		}
	}
	//end read readParamsFromFSJson

// --------------------------------------------------
//callback notifying the need to save config
void saveConfigCallback()
	{
	Serial.println(F("Should save config"));
	shouldSaveConfig = true;
	}

// --------------------------------------------------
void wiFiManagerDo()                                                                                // on demand config portal
	{
	// The extra parameters to be configured (can be either global or just in the setup)
	// After connecting, parameter.getValue() will get you the configured value
	// id/name placeholder/prompt default length
	//WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
	//WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
	WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 32);

	WiFiManager wifiManager;                                                                          //Local intialization. Once its business is done, there is no need to keep it around

	wifiManager.setSaveConfigCallback(saveConfigCallback);                                            //set config save notify callback

	//add all your parameters here
	//wifiManager.addParameter(&custom_mqtt_server);
	//wifiManager.addParameter(&custom_mqtt_port);
	wifiManager.addParameter(&custom_blynk_token);

	//reset settings - for testing
	//wifiManager.resetSettings();

	//set minimum quality of signal so it ignores AP's under that quality
	//defaults to 8%
	//wifiManager.setMinimumSignalQuality();
	
	//sets timeout until configuration portal gets turned off
	//useful to make it all retry or go to sleep
	//in seconds
	//wifiManager.setTimeout(120);

	Serial.println(F("Starting configuration Access Point"));
	Serial.println(F("Connect PC or phone to WiFi Access Point ESPConfigurationAP then browse to 192.168.4.1"));
	if (!wifiManager.startConfigPortal("ESPConfigurationAP"))                                                               // start the wifi configuration Access Point to allow user to enter network credentials
		{                                                                                                                     // and Rig Resetter parameters.
		statusLED(4, "wiFiManagerDo() - failed to start the Configuration Portal");
		/*
		Serial.println("failed to connect and hit timeout");
		delay(3000);
		//reset and try again, or maybe put it to deep sleep
		ESP.reset();
		delay(5000);
		*/
		}
		
	Serial.println(F("connected to WiFi network!"));                                                                               // if you get here you have connected to the WiFi network
	Serial.print(F("IP address: "));
	Serial.println(WiFi.localIP());

	//strcpy(mqtt_server, custom_mqtt_server.getValue());                                                                         //read updated parameters
	//strcpy(mqtt_port, custom_mqtt_port.getValue());
	strcpy(blynk_token, custom_blynk_token.getValue());

	if (shouldSaveConfig)                                                                                                       //save the custom parameters to FS
		{
		Serial.println(F("saving parameters: "));
		DynamicJsonBuffer jsonBuffer;
		JsonObject& json = jsonBuffer.createObject();
		//json["mqtt_server"] = mqtt_server;
		//json["mqtt_port"] = mqtt_port;
		json["blynk_token"] = blynk_token;
		File configFile = SPIFFS.open("/config.json", "w");
		if (!configFile)
			{
			statusLED(5, "wiFiManagerDo() - failed to open parameter file for writing");
			}
		json.printTo(Serial);
		Serial.println();
		Serial.println();
		json.printTo(configFile);
		configFile.close();
		} //end save
		
	statusLED(7, "wiFiManagerDo() - configuration complete, TURN CONFIG SWITCH OFF then restart system!");                       // flash LED_BUILTIN (500ms freq) as prompt to turn configSwitch Off and restart system
	
	} // end of wiFiManagerDo

// --------------------------------------------------
void connectToLastUsedAccessPoint()                                                     // connect to the last used access point based on configuration saved in flash memory by SDK
	{
	//WiFi.begin();                                                                       // don't need this. it will auto-connect if 'persistent' enabled
	Serial.println(F("Connecting to WiFi network"));
	while (WiFi.status() != WL_CONNECTED)                                                 // Wait for connection
		{
		delay(500);
		Serial.print(F("."));
		//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		pcfP.write(3, !pcfP.readButton(3));                                                                               // (Power LED on output pcfP pin 3)
		}
	Serial.println("");
	Serial.print(F("Connected to: "));
	Serial.println(WiFi.SSID());
	//Serial.println(ssid);
	Serial.print(F("IP address: "));
	Serial.println(WiFi.localIP());
	}

// --------------------------------------------------
void statusLED(int flashes, String message)                                                                                // flash status LED and print message to serial monitor. BLOCKING !
	{                                                                                                                     // NB CHANGE TO POWER led via pcf8574
	Serial.println(message);
	while(1)                                                                                                              // FOREVER
		{
		for(int i=0; i<flashes; i++)
			{
			pcfP.write(3, HIGH);                                                                                              // On (Power LED on output pcfP pin 3)
			//digitalWrite(LED_BUILTIN, LOW);
			delay(250);
			pcfP.write(3, LOW);                                                                                               // Off (Power LED on output pcfP pin 3)
			//digitalWrite(LED_BUILTIN, HIGH);
			delay(250);
			}
		delay(2000);
		}
	}



	// digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
