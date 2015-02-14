#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    15
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int pos = 0;
int dir = 1;
void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
}


void loop()
{
 
  for( int i = 0; i < NUM_LEDS; i++) {
    if(abs(i - pos)<2){
      leds[i] = CHSV( HUE_RED, 255, 100);
    }else{
      leds[i] = CRGB::Black;
    }
  }
  
  leds[pos] = CHSV( HUE_RED, 255, 255);
  
  if(dir){
    if(pos==NUM_LEDS - 1){
      Serial.println("max");
      dir=0;
      pos--;
    }else{
      Serial.println("+");
      pos++;
    }
  }else{
    Serial.println("decrementa");
    if(pos==0){
      Serial.print("min");
      dir=1;
      pos++;
    }else{
      Serial.print("-");
      pos--;
    }
  }
  
  FastLED.show();
  delay(75);
}


