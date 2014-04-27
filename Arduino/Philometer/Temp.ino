//#include "DHT.h"
#include <math.h>

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

//DHT dht(ROOM_TEMP, DHTTYPE);

void setupTemp() {
//    dht.begin();
}

float room_humidity = 0;
float room_temp = 0;
void readRoomTemp()  {
    //room_humidity = dht.readHumidity();
    //room_temp = dht.readTemperature();
   int B=3975;                  //B value of the thermistor
   int a = analogRead(ROOM_TEMP);
   float resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
   float temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;
   room_temp = (temperature * 9.0 / 5.0) + 32.0;;

}

const float calibration = 6.9;

float last_temp_reading = 0;

int temp_cnt = 0;
float totalTemp = 0;
void readBodyTemp() {
   temp_cnt++;
   int reading = analogRead(BODY_TEMP); 
   float temperatureF = calcTemp(reading);
   totalTemp += temperatureF;
}

float getBodyTemp() {
   float t = (totalTemp / temp_cnt) - calibration;
   temp_cnt = 0;
   totalTemp = 0;
   last_temp_reading = t;
   return t;
}

int getTempStatusColor() {
   if (last_temp_reading > 0 && last_temp_reading < 100) {
      return GREEN;
   }
   return RED;
}

float calcTemp(int reading) {
   float voltage = reading * 5.0;
   voltage /= 1024.0; 
   float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
   float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
   return temperatureF;
}

float convertTempToF(float temperatureC) {
  return (temperatureC * 9.0 / 5.0) + 32.0;
}

/*
float getRoomTemp() {
  return convertTempToF(room_temp);
}

float getHumidity() {
  return room_humidity;
}
*/

float getRoomTemp() {
  return room_temp;
}

