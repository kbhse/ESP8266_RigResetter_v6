/* ESPConfigurationAP.h */

#ifndef ESPConfigurationAP_H   /* include guards */
#define ESPConfigurationAP_H

    #include <Arduino.h>

    extern char blynk_token[34];

    void readParamsFromFSJson(boolean configSwitchFlag);
    void saveConfigCallback();
    void wiFiManagerDo();
    void connectToLastUsedAccessPoint();
    void statusLED(int flashes, String message);

#endif /* ESPConfigurationAP_H */
