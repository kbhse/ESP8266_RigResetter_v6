/* TimeRoutines.cpp */

#include <Arduino.h>
#include "TimeRoutines.h"

String dateAndTime()                                                                               // WidgetRTC gets date and time from Blynk server and makes it available to device
  {                                                                                                // uses Time keeping library https://github.com/PaulStoffregen/Time
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  //String currentTime = String(hour()) + ":" + minute() + ":" + second();
  //String currentDate = String(day()) + " " + month() + " " + year();
  String Day = getLeading(day());
  String Month = getLeading(month());
  String Year = getLeading(year());
  String Hour = getLeading(hour());
  String Minute = getLeading(minute());
  //String dateTime1 = Day + "/" + Month + "/" + Year + " " + Hour + ":" + Minute;
  String dateTime1 = Day + "/" + Month + " " + Hour + ":" + Minute;
  Serial.print(F("Current date and time: "));
  Serial.println(dateTime1);
  //Serial.print(" ");
  //Serial.print(currentDate);
  //Serial.println();

  // Send time to the App
  //Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  //Blynk.virtualWrite(V2, currentDate);

  return dateTime1;
  }


String getLeading(int digits)                                                                      // adds leading '0'
  {
  String leading = String(digits);
  if(digits < 10)
    {
    leading = "0" + leading;
    }
  return leading;
  }