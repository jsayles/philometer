#include <Wire.h>
#include <SeeedOLED.h>

void setupOLED() {
  Wire.begin();	
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;        
  PORTB |= 0x21;
  
  splash();
}

void splash() {
  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0, 5);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("The New"); 
  SeeedOled.setTextXY(1, 2);  
  SeeedOled.putString("Philographers");
  SeeedOled.setTextXY(3, 7);  
  SeeedOled.putString("by");
  SeeedOled.setTextXY(5, 2);  
  SeeedOled.putString("Jacob & Katie");
  SeeedOled.setTextXY(7, 7);  
  SeeedOled.putString("2014"); 
}

void clearDisplay() {
  SeeedOled.clearDisplay();
}

void displayStats(float gsr, float body_temp, int brain_signal, int brain_attention, int brain_meditation, int brain_bypass, int heart_interval, int heart_bpm, int mark) {
  SeeedOled.setTextXY(0, 1);          
  SeeedOled.putString("Mark: "); 
  SeeedOled.putNumber(mark); 
  SeeedOled.setTextXY(1, 1);          
  SeeedOled.putString("GSR: "); 
  SeeedOled.putFloat(gsr); 
  SeeedOled.setTextXY(2, 1);          
  SeeedOled.putString("Temp: "); 
  SeeedOled.putFloat(body_temp); 
  SeeedOled.setTextXY(3, 1);          
  SeeedOled.putString("Heart: "); 
  SeeedOled.putNumber(heart_interval); 
  SeeedOled.putString("  "); 
  SeeedOled.setTextXY(4, 2);          
  SeeedOled.putString("BPM: "); 
  SeeedOled.putNumber(heart_bpm); 
  SeeedOled.putString("  "); 
  SeeedOled.setTextXY(5, 1);          
  SeeedOled.putString("Brain: "); 
  SeeedOled.putNumber(brain_signal);
  SeeedOled.putString("   ");  
  SeeedOled.setTextXY(6, 2);          
  SeeedOled.putString("A: "); 
  SeeedOled.putNumber(brain_attention); 
  SeeedOled.putString(" M: "); 
  SeeedOled.putNumber(brain_meditation); 
  SeeedOled.putString(" ");  
  SeeedOled.setTextXY(7, 2);          
  SeeedOled.putString("Bypass: "); 
  SeeedOled.putNumber(brain_bypass); 
  SeeedOled.putString("  "); 
 
}

void writeMessage(int m) { 
    float body_temp = 89.6;
  switch(m) {
    case 1:
  SeeedOled.setTextXY(7, 0);          
  SeeedOled.putString("Temp: "); 
  //SeeedOled.setTextXY(3, 6);          
  SeeedOled.putFloat(body_temp); 
  break;

    case 2:
  SeeedOled.setTextXY(3, 0);          
  SeeedOled.putString("GSR:  "); 
  SeeedOled.setTextXY(4, 0);          
  SeeedOled.putString("318");
  break;

    default:
  SeeedOled.setTextXY(3, 0);          
  SeeedOled.putString("Will You Marry"); 
  SeeedOled.setTextXY(4, 0);          
  SeeedOled.putString("Me Katie?!?"); 
  }

}

