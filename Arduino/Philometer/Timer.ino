#include <Time.h>  
#include "TM1637.h"

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};

TM1637 tm1637(DISPLAY_CLOCK, DISPLAY_DATA);

unsigned int seconds_counter;
int seconds_offset;
int minutes_offset;

void setupTimer() {
   //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
   tm1637.set(BRIGHT_TYPICAL);  
  
   tm1637.init();
  
   seconds_counter = 0;
   seconds_offset = second();
   minutes_offset = minute();
}

int updateTimer() {
   int offset_seconds = second() - seconds_offset;
   int offset_minutes = minute() - minutes_offset;

   int s = (minute() * 60) + second();
   if (s != seconds_counter) {
      // Update the display
      TimeDisp[0] = offset_minutes/10%10;
      TimeDisp[1] = offset_minutes%10;
      TimeDisp[2] = offset_seconds/10%10;;
      TimeDisp[3] = offset_seconds%10;
      tm1637.point(1);
      tm1637.display(TimeDisp);
      seconds_counter = s;
   }
  
   return seconds_counter;
}

String getStringTS() {
   int offset_seconds = getOffsetSeconds();
   int offset_minutes = minute() - minutes_offset;
  
   String ts_val = "";
   int minTmp = offset_minutes;
   if (minTmp < 10) {
      ts_val = "0";
   }
   ts_val += String(minTmp);
   ts_val += ":";
   int secTmp = offset_seconds;
   if (secTmp < 10) {
      ts_val += "0";
   }
   ts_val += secTmp; 
   return ts_val;
}

int getOffsetSeconds() {
   int offset_seconds = second() - seconds_offset;
   if (offset_seconds < 0) {
      return 0;
   }
   return offset_seconds;
}
  
void displayInt(int c) {
   tm1637.point(1);
   TimeDisp[3] = c%10;
   TimeDisp[2] = c/10%10;
   TimeDisp[1] = c/100%10;
   TimeDisp[0] = c/1000%10;;
   tm1637.display(TimeDisp);
}
