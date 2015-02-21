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
      leds[i] = CHSV( hue + i*17, 255, 255);
      memLeds[i] = CHSV( hue + i*17, 255, 255);
  }
}


void loop(){
  
  if(!digitalRead(encoderSW)){
    while(!digitalRead(encoderSW));
    changing = !changing;
    Serial.print("ch ");
     Serial.print(encoder0Pos);    
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
  
  if(change){  
    FastLED.show();
  }
  
  if(millis() - time > 700){
    
  }
  
  
  
}

void moveUp(){
  if(encoder0Pos < NUM_LEDS){
    encoder0Pos++;
  }    
}

void moveDown(){
  if(encoder0Pos > 0){
    encoder0Pos--;
  }     
}


void colorUp(){
   memLeds[encoder0Pos].hue = memLeds[encoder0Pos].hue + 5;
   hsv2rgb_spectrum( memLeds, leds, NUM_LEDS); 
   Serial.println("+"); 
}

void colorDown(){
  memLeds[encoder0Pos].hue = memLeds[encoder0Pos].hue - 5;
  hsv2rgb_spectrum( memLeds, leds, NUM_LEDS); 
  Serial.println("-"); 
}

