#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

OneWire oneWire(BODY_TEMP);
DallasTemperature sensors(&oneWire);

void setupTemp() {
  sensors.begin();
  sensors.setWaitForConversion(false);
}

float last_temp_reading = 0;

float convertTempToF(float temperatureC) {
  return (temperatureC * 9.0 / 5.0) + 32.0;
}

void readTempSensors() {
  //sensors.requestTemperatures();
}

float getBodyTemp() {
   //float t = 38;
   float t = sensors.getTempCByIndex(0);
   last_temp_reading = convertTempToF(t);
   return last_temp_reading;
}

int getTempStatusColor() {
   if (last_temp_reading > 40 && last_temp_reading < 120) {
      return GREEN;
   }
   return RED;
}

float getRoomTemp() {
   int B=3975;                  //B value of the thermistor
   int a = analogRead(ROOM_TEMP);
   float resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
   float temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;
   return convertTempToF(temperature);
}
