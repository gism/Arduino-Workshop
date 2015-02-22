#include <FastLED.h>

#define LED_PIN     2
#define SW_PIN     3
#define NUM_LEDS    15
#define BRIGHTNESS  50
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define PUSH_DELAY 200;
CRGB leds[NUM_LEDS];
CHSV memLeds[NUM_LEDS];


int val; 
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoderSW= 5;
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
int flash = 0;

unsigned long time =0;

CHSV color;
int hue = 0;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  
  pinMode (encoder0PinA,INPUT);
  pinMode (encoder0PinB,INPUT);
  
  Serial.begin(9600);
  pinMode(SW_PIN, INPUT);
  pinMode(encoderSW, INPUT);
  
  
  for( int i = 0; i < NUM_LEDS; i++) {
//      leds[i] = CHSV( hue + i*17, 255, 255);
//      memLeds[i] = CHSV( hue + i*17, 255, 255);
    leds[i] = CRGB::Black;
    memLeds[i] = CHSV( 0, 255, 0);
  }
}


void loop(){
  
  if(!digitalRead(encoderSW)){
    while(!digitalRead(encoderSW));
    changing = !changing;   
  }
  
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      if (changing){
        colorDown();
        change = 1;
      }else{
        moveDown();
        change = 1;
      }
    } else {
      if (changing){
        colorUp();
        change = 1;
      }else{
        moveUp();
        change = 1;
      }
    }
  } 
  encoder0PinALast = n;
  
  if(millis() - time > 800){
    flash = !flash;
    time = millis();
    change = 1;
  }
  
  if(flash){
    if(!changing){
      leds[encoder0Pos] = CRGB::White;
    }
  }else{
    //hsv2rgb_spectrum(memLeds[encoder0Pos], leds[encoder0Pos]);
    hsv2rgb_spectrum( memLeds, leds, NUM_LEDS); 
  }

  if(change){   
    FastLED.show();
    change = 0;
  }
 
}

void moveUp(){
  if(encoder0Pos < NUM_LEDS){
    encoder0Pos++;
    
    time = millis();
    flash =1;
  }
  hsv2rgb_spectrum( memLeds, leds, NUM_LEDS);
}

void moveDown(){
  if(encoder0Pos > 0){
    encoder0Pos--;
    
    time = millis();
    flash = 1;
  }
  hsv2rgb_spectrum( memLeds, leds, NUM_LEDS); 
}


void colorUp(){
   memLeds[encoder0Pos].hue = memLeds[encoder0Pos].hue + 5;
   memLeds[encoder0Pos].val = 255;
   hsv2rgb_spectrum( memLeds, leds, NUM_LEDS);  
}

void colorDown(){
  memLeds[encoder0Pos].hue = memLeds[encoder0Pos].hue - 5;
  memLeds[encoder0Pos].val = 255;
  hsv2rgb_spectrum( memLeds, leds, NUM_LEDS); 
}

