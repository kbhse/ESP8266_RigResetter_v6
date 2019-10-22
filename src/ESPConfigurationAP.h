/* ESPConfigurationAP.h */

#ifndef ESPConfigurationAP_H   /* include guards */
#define ESPConfigurationAP_H

    #include <Arduino.h>
    #include "auxiliary.h"

    extern char blynk_token[34];
    extern boolean shouldSaveConfig;

    void readParamsFromFSJson(boolean configSwitchFlag);

    void saveConfigCallback();

    void connectToLastUsedAccessPoint();

    void statusLED(int flashes, String message);

#endif /* ESPConfigurationAP_H */
