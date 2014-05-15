#include "Constants.h"
#include <Brain.h>


// Set up the brain parser, pass it the hardware serial object you wanGt to listen on.
//Brain brain(Serial);
// Leonardo boards use "Serial1"
Brain brain(Serial);

int system_light_color = BLUE;

int brain_status_color = RED;
int loops_since_good_brain_read = 0;
int brain_bypass_counter = 0;

int mark_counter = 0;
int mark_delay = 0;

unsigned long time;
unsigned long heart_beat_on = 0;
int heart_pulse_time = 200;
boolean tracking_hr = false;
int heart_beat_color = BLUE;

void setup() {
   Serial.begin(9600);

   pinMode(DISPLAY_BUTTON, INPUT); 
   pinMode(MARK_BUTTON, INPUT); 
  
   mark_counter = 0;
   mark_delay = 0;
  
   setLights(BLUE);
   delay(1000);

   setupTemp();
   
   setupTimer();
   
   setupOLED();
  
   setupHeartMonitor();  
  
   //pinMode(SOUND_SENSOR, INPUT); 
  
   Serial.print("timestamp, mark, brain signal, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma, heart base, heart rate, GSR, body temp, room temp, temp diff");
}

void loop() {
   time = millis();

   // Clear Display
   /*
   if (digitalRead(DISPLAY_BUTTON) == HIGH) {
     clearDisplay();
   }
   */
  
   // Mark Increment Button
   if (mark_delay == 0) {
      if (digitalRead(MARK_BUTTON) == HIGH) {
         mark_counter++;
         mark_delay = 100;
      }
   } else {
      mark_delay--;
   }
  
   // Update the heart beat light
   int bpm = getHeartBPM();
   int heart_interval = getHeartFreq();
   if (bpm > 0) {
      tracking_hr = true;
      //int ms_per_beat = bpm/60 * 1000;
      int ms_per_beat = getHeartFreq();
      if (heart_beat_on == 0 | heart_beat_on + ms_per_beat <= time) {
         if (heart_beat_color != GREEN) {
            heart_beat_on = time;
            heart_beat_color = GREEN;
         }
      } else {
         if (heart_beat_on + heart_pulse_time <= time) {
            heart_beat_color = BLACK;
         }
      }
   } else {
      if (tracking_hr) {
         heart_beat_color = YELLOW;
      } else {
         heart_beat_color = BLUE;
      }
   }
   
   // Keep the status lights up to date
   updateLights();

   // Keep the time and update the display
   updateTimer();

   // Read these each loop and we get the average when
   // we call the get methods.
   readGSR();
   readBodyTemp();
   
   readRoomTemp();
   
   //int temp_adjust = analogRead(TEMP_ADJ); 
   int brain_data_bypass = analogRead(BRAIN_BYPASS) + 50; 
   int r, g, b = 0; 
   
   // LeOlympia - Set onboard LED
   //analogWrite(9, 100);
   //analogWrite(10, a2%255);
   //analogWrite(11, 255);
   
   // We go off the base timing of the brain sensors which are ready every
   // aprox 1 second.  We then pull all our data and output the CSV
   if (brain.update() | brain_bypass_counter >= brain_data_bypass) {
      String brain_data = brain.readCSV();
      if (brain_bypass_counter >= brain_data_bypass) {
         // Fake the data
         brain_data = "250,0,0,0,0,0,0,0,0,0,0";
      } else {
         // We got data!
         brain_status_color = GREEN;
         loops_since_good_brain_read = 0;
      }
      brain_bypass_counter = 0;
      
      String timestamp = getStringTS();
      float body_temp = getBodyTemp();
      float room_temp = getRoomTemp();
      float temp_diff = body_temp - room_temp;
      //int soundSensorValue = analogRead(SOUND_SENSOR);
      
      float gsr = getGSR();
      
      //Serial.println(brain.readErrors());
      Serial.print(timestamp);
      Serial.print(",");
      Serial.print(mark_counter);
      Serial.print(",");
      Serial.print(brain_data);
      Serial.print(",");
      Serial.print(heart_interval);
      Serial.print(",");
      Serial.print(bpm);
      Serial.print(",");
      Serial.print(gsr);
      Serial.print(",");
      Serial.print(body_temp);
      Serial.print(",");
      Serial.print(room_temp);
      Serial.print(",");
      Serial.print(temp_diff);
      Serial.print(",");
      //Serial.print(soundSensorValue);
      //Serial.print(",");
      //Serial.print(temp_adjust);
      //Serial.print(",");
      Serial.print(brain_data_bypass);
      Serial.println();
 
      int brain_signal = 100;
      int brain_attention = 50;
      int brain_meditation = 50;
      displayStats(gsr, body_temp, brain_signal, brain_attention, brain_meditation, heart_interval, bpm, mark_counter);      
   } else {
      brain_bypass_counter++;
      if (++loops_since_good_brain_read > 100) {
         brain_status_color = RED;
      }
   }
}
