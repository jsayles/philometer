// Reads the GSR Sensor
// Every time readGSR() is called it records a reading
// When getGSR() is called the average of all readings
// is returned.

float last_gsr_reading = 0;
float total_gsr;
int gsr_cnt = 0;

void readGSR() {
   gsr_cnt++;
   total_gsr += analogRead(GSR);
}

float getGSR() {
   float avg = total_gsr/gsr_cnt;
   total_gsr = 0;
   gsr_cnt = 0;
   last_gsr_reading = avg;
   return avg;
}

int getGSRStatusColor() {
   if (last_gsr_reading > 0 && last_gsr_reading < 480) {
      return GREEN;
   }
   return RED;
}

