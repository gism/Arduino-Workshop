#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    15
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

//Adapted from David A. Mellis' code for Ping sensor
const int trigPin = 7;
const int echoPin = 8;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
}

void loop()
{
  long duration, cm;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  
  int pos = map(cm, 5, 40, NUM_LEDS, 0);
  Serial.print("pos ");
  Serial.println(pos);
  
  for( int i = 0; i < NUM_LEDS; i++) {
    if(i<pos){
      leds[i] = CHSV( HUE_BLUE - 30 + 2*pos, 255, 255 - 75 + 5*pos);
    }else{
      leds[i] = CRGB::Black;
    }
  }
  
  FastLED.show();
  
  if(pos==NUM_LEDS){
    Serial.println("ALARM!!");
    byte hue = 0;
    while(1){
        for( int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV( HUE_RED + 10 - hue/5, 255, 200 + hue/5);
        }
        FastLED.show();
        hue++;
        delay(5);
    }
  }
  
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(100);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
