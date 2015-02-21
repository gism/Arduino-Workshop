#include <FastLED.h>

#define LED_PIN     2
#define SW_PIN     3
#define NUM_LEDS    15
#define BRIGHTNESS  50
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define PUSH_DELAY 200;
CRGB leds[NUM_LEDS];

int val; 
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

int pos = 0;
int dir = 1;

int state = 0;
int last_state = 0;
int push_mils = 0;

int change = 0;
int changing = 0;

CRGB color;
int hue = 0;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode (encoder0PinA,INPUT);
  pinMode (encoder0PinB,INPUT);
  
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT);
}


void loop(){
  
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
//      color = leds[encoder0Pos];
//      color.blue = color.blue - 30;
//      leds[encoder0Pos] = color;
       change = 1;
       
       hue = hue - 10;
      //down();
    } else {
      //up();
      
//      color = leds[encoder0Pos];
//      color.blue = color.blue + 30;
//      leds[encoder0Pos] = color;
      hue = hue + 10;
      change = 1;
    }
    Serial.println(encoder0Pos);
  } 
  encoder0PinALast = n;
  
  if(change){  
    for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV( hue + i*17, 255, 255);
    }
    
    FastLED.show();
    change = 0;
  }
}

void up(){
  if(encoder0Pos < NUM_LEDS){
    encoder0Pos++;
    change = 1;
  }    
}

void down(){
  if(encoder0Pos > 0){
    encoder0Pos--;
    change = 1;
  }     
}

