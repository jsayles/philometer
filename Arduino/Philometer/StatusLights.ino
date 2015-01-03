#include "RGBdriver.h"
#include <Adafruit_NeoPixel.h>

RGBdriver LED_Driver(LEDS_CLOCK, LEDS_DATA);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, CASE_LIGHTS, NEO_RGB + NEO_KHZ800);

int BRIGHTNESS = 100;

void setCaseLights(int r, int g, int b) {
  // Turn on the case lights
  strip.begin();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void setStatusBrightness(int b) {
  BRIGHTNESS = b;
}

void updateLights() {
   // Keep the status lights up to date
   setLights(system_light_color,
             brain_status_color,
             getGSRStatusColor(),
             getTempStatusColor(),
             getPulseStatusColor(),
             heart_beat_color);
}       

void setLights(int c) {
  setLights(c, c, c, c, c, c);
}

void testLights(int delayMS) {
  // Test the case lights
  setLights(BLACK);
  setCaseLights(255, 0, 0);
  delay(delayMS * 2);  
  setCaseLights(0, 255, 0);
  delay(delayMS * 2);
  setCaseLights(0, 0, 255);
  delay(delayMS * 2);

  // Test the status lights
  setLights(RED);
  delay(delayMS * 2);
  setLights(GREEN);
  delay(delayMS * 2);
  setLights(BLUE);
  delay(delayMS);
  setLights(GREEN, BLUE, BLUE, BLUE, BLUE, BLUE);
  delay(delayMS);
  setLights(BLUE, GREEN, BLUE, BLUE, BLUE, BLUE);
  delay(delayMS);
  setLights(BLUE, BLUE, GREEN, BLUE, BLUE, BLUE);
  delay(delayMS);
  setLights(BLUE, BLUE, BLUE, GREEN, BLUE, BLUE);
  delay(delayMS);
  setLights(BLUE, BLUE, BLUE, BLUE, GREEN, BLUE);
  delay(delayMS);
  setLights(BLUE, BLUE, BLUE, BLUE, BLUE, GREEN);
  delay(delayMS);
  setLights(BLUE);
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



