#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    15
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

byte hue = 0;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
}


void loop()
{
  hue = hue + 17;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV( hue + i*17, 255, 255);
  }
  
  FastLED.show();
  delay(75);
}


