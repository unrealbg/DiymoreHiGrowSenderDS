#include <Arduino.h>

#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 30ULL
uint64_t sleepus = 0;
const int Dry = 2950;
const int Wet = 1650;
const int SensorPin = 32;
int soilMoistureValue = 0;
float soilmoisturepercent = 0;
float Temperature;
float Humidity;