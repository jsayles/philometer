#include "RGBdriver.h"

RGBdriver LED_Driver(LEDS_CLOCK, LEDS_DATA);

void setLights(int c) {
   setLights(c, c, c, c, c, c);
}

void updateLights() {
   // Keep the status lights up to date
   setLights(heart_beat_color,
             getPulseStatusColor(),
             getGSRStatusColor(),
             getTempStatusColor(),
             brain_status_color,
             system_light_color);
}       

void setLights(int c1, int c2, int c3, int c4, int c5, int c6) {
   LED_Driver.begin();
   setLight(c1);
   setLight(c2);
   setLight(c3);
   setLight(c4);
   setLight(c5);
   setLight(c6);
   LED_Driver.end();
}

void setLight(int color) {
   switch(color) {
      case RED:
         LED_Driver.SetColor(BRIGHTNESS, 0, 0); 
         break;
      case GREEN:
         LED_Driver.SetColor(0, BRIGHTNESS, 0); 
         break;
      case BLUE:
         LED_Driver.SetColor(0, 0, BRIGHTNESS); 
         break;
      case YELLOW:
         LED_Driver.SetColor(BRIGHTNESS, BRIGHTNESS, 0); 
         break;
      case WHITE:
         LED_Driver.SetColor(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS); 
         break;
      default:
         LED_Driver.SetColor(0, 0, 0);
   }
}
