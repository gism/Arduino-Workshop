// Arduin Game of Life V0.2 by MarcG
// https://importhack.wordpress.com/
// --------------------------------------------------
// Code based on ADAFRUIT project:
// http://learn.adafruit.com/trinket-usb-volume-knob
 
#include "TimerOne.h"
 
// Game settings:
#define LIFE_RATIO 3
#define HEIGHT 8
#define WIDTH 8
 
// Encoder Hardware settings:
#define PIN_ENCODER_A 0
#define PIN_ENCODER_B 1
#define PORT_ENCODER PINC
#define PIN_BUTTON 0
 
// Numbers definition
#define UNO { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1} \
}
 
#define DOS { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 1, 0, 1, 1, 1}, \
 {1, 1, 1, 0, 1, 1, 1, 1}, \
 {1, 1, 0, 0, 0, 0, 0, 1} \
}
 
#define TRES { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1}, \
 {1, 1, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1} \
}
 
#define CUATRO { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 1, 0, 0, 1, 1}, \
 {1, 1, 1, 0, 1, 0, 1, 1}, \
 {1, 1, 0, 1, 1, 0, 1, 1}, \
 {1, 1, 0, 0, 0, 0, 0, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1} \
}
 
#define CINCO { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 0, 0, 0, 0, 0, 1}, \
 {1, 1, 0, 1, 1, 1, 1, 1}, \
 {1, 1, 0, 1, 1, 1, 1, 1}, \
 {1, 1, 0, 0, 0, 0, 1, 1}, \
 {1, 1, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 0, 0, 0, 1, 1} \
}
 
#define SEIS { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 1, 1, 1, 1, 1}, \
 {1, 1, 0, 0, 0, 0, 1, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1} \
}
 
#define SIETE { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 0, 0, 0, 0, 0, 1}, \
 {1, 1, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 1, 1, 0, 1, 1}, \
 {1, 1, 1, 1, 0, 1, 1, 1}, \
 {1, 1, 1, 1, 0, 1, 1, 1}, \
 {1, 1, 1, 1, 0, 1, 1, 1} \
}
 
#define OCHO { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1} \
}
 
#define NUEVE { \
 {1, 1, 1, 1, 1, 1, 1, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 0, 0, 0, 0, 1}, \
 {1, 1, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 1, 1, 1, 0, 1}, \
 {1, 1, 1, 0, 0, 0, 1, 1} \
}
 
const int numPatterns = 9;
byte numeros[numPatterns][8][8] = {
 UNO, DOS, TRES, CUATRO, CINCO, SEIS, SIETE, OCHO, NUEVE
};
 
// Initital patern NO RANDOM
int no_random[8][8]= { \
 {1, 1, 0, 0, 0, 0, 1, 1}, \
 {1, 0, 1, 1, 1, 1, 0, 1}, \
 {0, 1, 0, 1, 1, 0, 1, 0}, \
 {0, 1, 1, 1, 1, 1, 1, 0}, \
 {0, 1, 0, 1, 1, 0, 1, 0}, \
 {0, 1, 1, 0, 0, 1, 1, 0}, \
 {1, 0, 1, 1, 1, 1, 0, 1}, \
 {1, 1, 0, 0, 0, 0, 1, 1} \
};
 
// Encoder initialization:
static uint8_t enc_prev_pos = 0;
static uint8_t enc_flags = 0;
int buttonState; // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin
int buttonTime = 0;
 
// Display settings:
// 2-dimensional array of row pin numbers:
const int row[8] = {2,7,19,5,13,18,12,16};
 
// 2-dimensional array of column pin numbers:
const int col[8] = {6,11,10,3,17,4,8,9};
 
// Generate univers:
// Pixels is display address
int pixels[8][8];
// Univers is game status
int univers[HEIGHT][WIDTH];
int letra=0;
 
// cursor position:
int x = 0;
int y = 0;
int ratio = LIFE_RATIO;
 
int state = 0;

long last_milis;
long autostart_delay=5000;

int count = 0;
int life = 1;

void setup(){
 
// Uart interface, only dev purpose
Serial.begin(9600);
   
  // Set pins as input with internal pull-up resistors enabled
  pinMode(PIN_ENCODER_A, INPUT);
  pinMode(PIN_ENCODER_B, INPUT);
  digitalWrite(PIN_ENCODER_A, HIGH);
  digitalWrite(PIN_ENCODER_B, HIGH);
   
  // Initialize the pushbutton pin as an input:
  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, HIGH);
   
  // get an initial reading on the encoder pins
  if (digitalRead(PIN_ENCODER_A) == LOW) {
     enc_prev_pos |= (1 << 0);
  }
  if (digitalRead(PIN_ENCODER_B) == LOW) {
     enc_prev_pos |= (1 << 1);
  }
   
  // LED DISPLAY
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
  // initialize the output pins:
  pinMode(col[thisPin], OUTPUT);
  pinMode(row[thisPin], OUTPUT);
  // take the col pins (i.e. the cathodes) high to ensure that
  // the LEDS are off:
  digitalWrite(col[thisPin], HIGH);
  }
   
  Timer1.initialize(2000000);
  showSetRatio();
   
  last_milis = millis();
  
  Serial.println("Arduino - Conway's game of life - by Marc G");
  Serial.println("Visit https://importhack.wordpress.com");
}
 
void loop(){
  int8_t enc_action = 0; // 1 or -1 if moved, sign is direction
   
  uint8_t enc_cur_pos = 0;
  // read in the encoder state first
  if (bit_is_clear(PORT_ENCODER, PIN_ENCODER_A)) {
    enc_cur_pos |= (1 << 0);
  }
  if (bit_is_clear(PORT_ENCODER, PIN_ENCODER_B)) {
    enc_cur_pos |= (1 << 1);
  }
   
  // if any rotation at all
  if (enc_cur_pos != enc_prev_pos){
    if (enc_prev_pos == 0x00){
      // this is the first edge
      if (enc_cur_pos == 0x01) {
        enc_flags |= (1 << 0);
      }else if (enc_cur_pos == 0x02) {
        enc_flags |= (1 << 1);
      }
    }
   
    if (enc_cur_pos == 0x03){
      // this is when the encoder is in the middle of a "step"
      enc_flags |= (1 << 4);
    }else if (enc_cur_pos == 0x00){
      // this is the final edge
      if (enc_prev_pos == 0x02) {
        enc_flags |= (1 << 2);
      }else if (enc_prev_pos == 0x01) {
        enc_flags |= (1 << 3);
      }
   
      // check the first and last edge
      // or maybe one edge is missing, if missing then require the middle state
      // this will reject bounces and false movements
      if (bit_is_set(enc_flags, 0) && (bit_is_set(enc_flags, 2) || bit_is_set(enc_flags, 4))) {
        enc_action = 1;
      }else if (bit_is_set(enc_flags, 2) && (bit_is_set(enc_flags, 0) || bit_is_set(enc_flags, 4))) {
        enc_action = 1;
      }else if (bit_is_set(enc_flags, 1) && (bit_is_set(enc_flags, 3) || bit_is_set(enc_flags, 4))) {
        enc_action = -1;
      }else if (bit_is_set(enc_flags, 3) && (bit_is_set(enc_flags, 1) || bit_is_set(enc_flags, 4))) {
        enc_action = -1;
      }
      enc_flags = 0; // reset for next time
    }
  }
  enc_prev_pos = enc_cur_pos;
   
  if (enc_action > 0) {
    Serial.println("+");
    last_milis = millis();
    if((ratio < 8)&&(!state)){
      ratio++;
      showSetRatio();
    }
    if(state==1){
      nextGeneration();
    }
   
  }
  else if (enc_action < 0) {
    Serial.println("-");
    last_milis = millis();
    if((ratio > 0)&&(!state)){
      ratio--;
      showSetRatio();
    }
  }
  
  // Auto start
  if(millis() - last_milis > autostart_delay && state == 0){
    Serial.println("Auto");
    Timer1.attachInterrupt(nextGeneration);
    generateUnivers(ratio);
    printUnivers();
    state = 2;
  }
  
  // Auto Start when universe is dead
  if(life==64 && state == 2){
    count++;
    if(count==255){
      Serial.println("Death");
      Timer1.detachInterrupt();
      showSetRatio();
      state = 0;
      count = 0;
      last_milis = millis();
    }
  }
   
   
  // Check button status:
  buttonState = digitalRead(PIN_BUTTON);
  if (buttonState == 0) {
    delay(2);
    if (!lastButtonState && !buttonState){
    Serial.println("X");
   
    switch (state){
      case 0:
        generateUnivers(ratio);
        printUnivers();
        state = 1;
        break;
   
      case 1:
        Timer1.attachInterrupt(nextGeneration);
        state = 2;
        break;
       
      case 2:
        Timer1.detachInterrupt();
        state = 1;
        break;
      }
    }
   
    // Long push:
    if (buttonTime > 1000){
      Timer1.detachInterrupt();
      showSetRatio();
      state = 0;
      last_milis = millis();
      Serial.println(">X<");
    }
    buttonTime++;
    lastButtonState = 1;
  }else{
    lastButtonState = 0;
    buttonTime = 0;
  }
   
  refreshScreen();
}
 
// Put current ratio on display array:
void showSetRatio(){
  for(int y=0; y < HEIGHT; y++){
    for(int x=0; x < WIDTH; x++){
      pixels[y][x] = numeros[ratio][y][x];
    }
  }
}
 
// Print pixels array to LED display:
void refreshScreen() {
  // iterate over the rows (anodes):
  for (int thisRow = 0; thisRow < 8; thisRow++) {
    // take the row pin (anode) high:
    digitalWrite(row[thisRow], HIGH);
    // iterate over the cols (cathodes):
    for (int thisCol = 0; thisCol < 8; thisCol++) {
      // get the state of the current pixel;
      int thisPixel = pixels[thisRow][thisCol];
      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      // turn the pixel off:
      if (thisPixel == LOW) {
        digitalWrite(col[thisCol], HIGH);
      }
    }
    // take the row pin low to turn off the whole row:
    digitalWrite(row[thisRow], LOW);
  }
}
 
void nextGeneration(){
  evolve();
  printUnivers();
}
 
// Print univers in pixels array
void printUnivers(){
  life = 0;
  for(int y=0; y < HEIGHT; y++){
    for(int x=0; x < WIDTH; x++){
      pixels[y][x] = univers[y][x];
      life = life + univers[y][x];
    }
  }
}
 
void evolve(){
  unsigned new_univers[HEIGHT][WIDTH];
   
  invertirUniverso();
   
  for (int y = 0; y < HEIGHT; y++){
    for (int x = 0; x < WIDTH; x++){
      int n = 0;
      for (int y1 = y - 1; y1 <= y + 1; y1++){
        for (int x1 = x - 1; x1 <= x + 1; x1++){
          if((y1 >= 0) && (x1 >= 0) && (y1 < HEIGHT) && (x1 < WIDTH)){
            if (univers[y1][x1]){
              n++;
            }
          }
        }
      }  
      if (univers[y][x]){
        n--;
      }
       
      new_univers[y][x] = (n == 3 || (n == 2 && univers[y][x]));
    }
  }
   
  for(int y=0; y < HEIGHT; y++){
    for(int x=0; x < WIDTH; x++){
      univers[y][x] = new_univers[y][x];
    }
  } 
  invertirUniverso();
}
 
void invertirUniverso(){
  for(int y=0; y < HEIGHT; y++){
    for(int x=0; x < WIDTH; x++){
      if (univers[y][x] == 1){
        univers[y][x] = 0;
      }else{
        univers[y][x] = 1;
      }
    }
  }
}
 
void generateUnivers(int initialRatio){
 
  Serial.print("Life ratio: ");
  Serial.println(initialRatio);
   
  int randNumber;
  Serial.println("Initial Univers:");
  for(int y=0; y < HEIGHT; y++){
    for(int x=0; x < WIDTH; x++){
      randNumber = random(10);
      if(randNumber > ratio){
        univers[y][x] = 1;
      }else{
        univers[y][x] = 0;
      }
       
      Serial.print(univers[y][x]);
      Serial.print("[");
      Serial.print(randNumber);
      Serial.print("]");
      Serial.print("\t");
    }
    Serial.print("\n");
  }
 
  // Change to true if you want to start with custom patern
  // Edit no_random array
  if(false){
    for(int y=0; y < HEIGHT; y++){
      for(int x=0; x < WIDTH; x++){
        univers[y][x] = no_random[y][x];
      }
    }
  }
}
