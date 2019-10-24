


#include <Arduino.h>
#include "SimpleTimer.h"
#include "test.h"

    extern SimpleTimer timer;

    void testTimerCallback()
        {
        Serial.println("timer fired");
        }


void testJmm()
    {
        Serial.println("starting test timer");
        timer.setTimeout(10000L, testTimerCallback);
    }

