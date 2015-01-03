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
   
   // Setup and splash
   setupOLED();
   testLights(250);
   clearDisplay();
   setCaseLights(0, 0, 255);
  
   setupTemp();   
   setupTimer();
   setupHeartMonitor();  
  
   // Display the header
   Serial.println();
   Serial.println("timestamp, mark, brain signal, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma, heart MS, heart BPM, GSR, body temp, brain bypass");
}

void loop() {
   time = millis();

   // Clear Display
   if (digitalRead(DISPLAY_BUTTON) == HIGH) {
     splash();
     delay(1000);
     clearDisplay();
   }
  
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
   int heart_bpm = getHeartBPM();
   int heart_ms = getHeartFreq();
   if (heart_bpm > 0) {
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
   
   setStatusBrightness(10 + analogRead(BRIGHTNESS_CONTROL) / 4); 
   
   // We go off the base timing of the brain sensors which are ready every
   // aprox 1 second.  We then pull all our data and output the CSV
   int brain_signal = brain.readSignalQuality();
   int brain_attention = brain.readAttention();
   int brain_meditation = brain.readMeditation();
   int brain_data_bypass = analogRead(BRAIN_BYPASS) + 50; 
   if (brain.update() | brain_bypass_counter >= brain_data_bypass) {
      String brain_data = brain.readCSV();
      if (brain_bypass_counter >= brain_data_bypass) {
         // Fake the data
         brain_data = "250,0,0,0,0,0,0,0,0,0,0";
         brain_signal = 250;
         brain_attention = 0;
         brain_meditation = 0;
      } else {
         // We got data!
         loops_since_good_brain_read = 0;
      }
      brain_bypass_counter = 0;
      
      // Calculate the brain signal light
      if (brain_signal == 0) {
        brain_status_color = GREEN;
      } else if (brain_signal < 200) {
        brain_status_color = YELLOW;
      } else {
        brain_status_color = RED;
      }

      String timestamp = getStringTS();
      float body_temp = getBodyTemp();
      float gsr = getGSR();
      
      // Dump the data out the serial connection
      serialOut(timestamp, mark_counter, brain_data, brain_data_bypass, heart_ms, heart_bpm, gsr, body_temp);
      
      // Display the data on the pretty little OLED
      displayStats(gsr, body_temp, brain_signal, brain_attention, brain_meditation, brain_data_bypass, heart_ms, heart_bpm, mark_counter);      
   } else {
      brain_bypass_counter++;
      if (++loops_since_good_brain_read > 100) {
         brain_status_color = RED;
      }
   }
}

void serialOut(String timestamp, int mark_counter, String brain_data, int brain_data_bypass, int heart_ms, int heart_bpm, float gsr, float body_temp) {
      Serial.print(timestamp);
      Serial.print(",");
      Serial.print(mark_counter);
      Serial.print(",");
      Serial.print(brain_data);
      Serial.print(",");
      Serial.print(heart_ms);
      Serial.print(",");
      Serial.print(heart_bpm);
      Serial.print(",");
      Serial.print(gsr);
      Serial.print(",");
      Serial.print(body_temp);
      Serial.print(",");
      Serial.print(brain_data_bypass);
      Serial.println();
 }
