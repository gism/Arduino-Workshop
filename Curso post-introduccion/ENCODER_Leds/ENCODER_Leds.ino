#include <FastLED.h>

#define LED_PIN     2
#define SW_PIN     3
#define NUM_LEDS    15
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define PUSH_DELAY 200;
CRGB leds[NUM_LEDS];

int pos = 0;
int dir = 1;

int state = 0;
int last_state = 0;
int push_mils = 0;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT);
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
  
  if(digitalRead(SW_PIN)){
    if(last_state == 0){
      state = 1;
      push_mils = millis();
    }
    last_state = 1;
  }else{
    last_state = 0;
  }
  
  if(millis()-push_mils>300){
   state =0; 
  }
  
  if(state ==1){
    leds[0] = CRGB::White;
  }
  
  if(dir){
    if(pos==NUM_LEDS - 1){
      dir=0;
      pos--;
    }else{
      pos++;
    }
  }else{
    if(pos==0){
      if(!state){
        Serial.println("GAME OVER");
        for( int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB::Red;
        }
        FastLED.show();
        while(1);
      }
      dir=1;
      pos++;
    }else{
      pos--;
    }
  }
  
  FastLED.show();
  delay(40+pos*pos/2);
}


