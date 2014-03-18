#include "Constants.h"
#include <Brain.h>

// Set up the brain parser, pass it the hardware serial object you want to listen on.
Brain brain(Serial);
int brain_status_color = RED;
int loops_since_good_brain_read = 0;

int mark_counter = 0;
int mark_delay = 0;

void setup() {
   Serial.begin(9600);

   pinMode(RESTART_BUTTON, INPUT); 
   pinMode(MARK_BUTTON, INPUT); 
  
   mark_counter = 0;
   mark_delay = 0;
  
   setLights(RED);

   setupTimer();
  
   setupHeartMonitor();  
  
   pinMode(SOUND_SENSOR, INPUT); 
  
   Serial.print("timestamp, mark, brain connection, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma, heart base, heart rate, GSR, body temp, room temp, temp diff, sound level");
}

void loop() {
   // Reset Button
   if (digitalRead(RESTART_BUTTON) == HIGH) {
      Serial.println("Restarting...");
      delay(2000);
      setup();
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
  
   // Keep the status lights up to date
   setLights(brain_status_color,
             getPulseStatusColor(),
             getTempStatusColor(),
             getGSRStatusColor());

   // Keep the time and update the display
   updateTimer();

   // Read these each loop and we get the average when
   // we call the get methods.
   readGSR();
   readBodyTemp();

   // We go off the base timing of the brain sensors which are ready every
   // aprox 1 second.  We then pull all our data and output the CSV
   if (brain.update()) {
      brain_status_color = GREEN;
      loops_since_good_brain_read = 0;
      
      String timestamp = getStringTS();
      float body_temp = getBodyTemp();
      float room_temp = getRoomTemp();
      float temp_diff = body_temp - room_temp;
      int soundSensorValue = analogRead(SOUND_SENSOR);
      
      //Serial.println(brain.readErrors());
      Serial.print(timestamp);
      Serial.print(", ");
      Serial.print(mark_counter);
      Serial.print(", ");
      Serial.print(brain.readCSV());
      Serial.print(", ");
      Serial.print(getBaseHeartReading());
      Serial.print(", ");
      Serial.print(getHeartRate());
      Serial.print(", ");
      Serial.print(getGSR());
      Serial.print(", ");
      Serial.print(body_temp);
      Serial.print(", ");
      Serial.print(room_temp);
      Serial.print(", ");
      Serial.print(temp_diff);
      Serial.print(", ");
      Serial.print(soundSensorValue);
      Serial.println();
   } else {
      if (++loops_since_good_brain_read > 100) {
         brain_status_color = RED;
      }
   }
}
