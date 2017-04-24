/*
*  Author: Bill Porter
*    www.billporter.info
*
*   AI player code by: Mofidul Jamal
*
*    Code to run a wicked cool LED Tetris playing neck tie. Details:
*         http://www.billporter.info/2013/06/21/led-tetris-tie/
*/

#include <Arduino.h>
#include <Wire.h>
#include <ArduinoNunchuk.h>
#include <Adafruit_NeoPixel.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

void screenTest();
void play();
void performAI();
int aiCalculateWeight();
int getHighestColumn();
int getHoleCount();
bool getFullLinePossible();
byte getCommand();
bool checkRotate( bool direction );
bool checkShift(short right, short down);
bool checkGround();
bool checkCeiling();
bool checkCollision();
void shift(short right, short down);
void rotate( bool direction );
void moveDown();
void addToWall();
void removeFromWall();
void updateBrickArray();
bool clearLine();
void nextBrick();
void flashLine( int line );
void drawWall();
void drawGame();
void draw(byte color, signed int brightness, byte x, byte y);
void gameOver();
void newGame();
void updateDisplay();
void clearWall();
void check_nunchuck();
void scoreOneUpLine();
void scoreAdjustLevel();
void doScrolling(char [], int);

//Constants on how it's physically wired
#define LEDDATAPIN 2
#define POWEROFFPIN 4
#define BATTERYVOLTAGEPIN 0

//constants and initialization
#define UP  0
#define DOWN  1
#define RIGHT  2
#define LEFT  3
#define NO_MOVE  4
#define CLOCK_WISE 5
#define COUNTER_CLOCK_WISE 6

#define FULL 255
#define WHITE 0xFF
#define OFF 0

//Display Settings
#define FIELD_WIDTH 8
#define FIELD_HEIGHT 16

#define tick_delay 30         //game speed
#define DOWN_DELAY 800
#define ROTATION_DELAY 80

#define GAME_OVER_HEIGHT FIELD_HEIGHT

//weight given to the highest column for ai
#define HIGH_COLUMN_WEIGHT 5
//weight given to the number of holes for ai
#define HOLE_WEIGHT 3

#define BRICK_COUNT 7

int colorLeters = 0;
// char str[] = "01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char str[] = "Hola MAKERS!";
int strLen = sizeof(str);
int ptrChar = 0;
long scrollPosition = 0;
#define SCROLL_DELAY 120

const PROGMEM byte font[128][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0000 (nul)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0001
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0002
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0003
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0004
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0005
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0006
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0007
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0008
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0009
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+000F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0010
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0011
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0012
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0013
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0014
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0015
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0016
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0017
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0018
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0019
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001A
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001B
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001C
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001D
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001E
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+001F
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
    { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
    { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
    { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
    { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
    { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
    { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
    { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
    { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
    { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
    { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
    { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
    { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (//)
    { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
    { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
    { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
    { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
    { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
    { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
    { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
    { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
    { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
    { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
    { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
    { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
    { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
    { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
    { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
    { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
    { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
    { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
    { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
    { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
    { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
    { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
    { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
    { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
    { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
    { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
    { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
    { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
    { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
    { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
    { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
    { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
    { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
    { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
    { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
    { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
    { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
    { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
    { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
    { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
    { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
    { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
    { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
    { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
    { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
    { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
    { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
    { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
    { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
    { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
    { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
    { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
    { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
    { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
    { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
    { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
    { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
    { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
    { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
    { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
    { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+007F
};

const PROGMEM uint16_t bricks[ BRICK_COUNT ][4] = {
  {
    0b0100010001000100,      //1x4 cyan
    0b0000000011110000,
    0b0100010001000100,
    0b0000000011110000
  },
  {
    0b0000010011100000,      //T  purple
    0b0000010001100100,
    0b0000000011100100,
    0b0000010011000100
  },
  {
    0b0000011001100000,      //2x2 yellow
    0b0000011001100000,
    0b0000011001100000,
    0b0000011001100000
  },
  {
    0b0000000011100010,      //L orange
    0b0000010001001100,
    0b0000100011100000,
    0b0000011001000100
  },
  {
    0b0000000011101000,      //inverse L blue
    0b0000110001000100,
    0b0000001011100000,
    0b0000010001000110
  },
  {
    0b0000100011000100,      //S green
    0b0000011011000000,
    0b0000100011000100,
    0b0000011011000000
  },
  {
    0b0000010011001000,      //Z red
    0b0000110001100000,
    0b0000010011001000,
    0b0000110001100000
  }
};

//8 bit RGB colors of blocks
//RRRGGGBB
const PROGMEM uint8_t brick_colors[BRICK_COUNT]={
  0b00011111, //cyan
  0b10000010, //purple
  0b11111100, //yellow
  0b11101000, //orange?
  0b00000011, //blue
  0b00011100, //green
  0b11100000 //red
};

//8 bit RGB colors of each word
//RRRGGGBB
const PROGMEM uint8_t words_colors[BRICK_COUNT]={
  0b00011100, //green
  0b11100000, //red
  0b00011111, //cyan
  0b10000010, //purple
  0b11111100, //yellow
  0b11101000, //orange?
  0b00000011 //blue
};

const unsigned short	max_level	= 9;
const unsigned long	level_tresholds[ 10 ]	= {
	0,
	100,
	500,
	1500,
	4000,
	6000,
	8000,
	10000,
	14000,
	20000
};

const unsigned short level_difficulty[ max_level ]  = {
0,
80,
160,
240,
320,
400,
480,
560,
700
};

const unsigned int score_per_level = 10; //per brick in lv 1+
const unsigned int score_per_line  = 300;

byte wall[FIELD_WIDTH][FIELD_HEIGHT];
//The 'wall' is the 2D array that holds all bricks that have already 'fallen' into place

#define AI_MODE 1
#define USER_MODE 2
int gameMode = AI_MODE;

bool aiCalculatedAlready = false;   // Set to true when ai tasks are finished

struct TAiMoveInfo{
  byte rotation;
  int positionX, positionY;
  int weight;
} aiCurrentMove;

struct TBrick{
  byte type; //This is the current brick shape.
  byte rotation; //active brick rotation
  byte color; //active brick color
  int positionX, positionY; //active brick position
  byte pattern[4][4]; //2D array of active brick shape, used for drawing and collosion detection
} currentBrick;

unsigned short  level       = 0;
unsigned long  score        = 0;
unsigned long  score_lines  = 0;

// Define the RGB pixel array and controller functions,
Adafruit_NeoPixel strip = Adafruit_NeoPixel(FIELD_WIDTH*FIELD_HEIGHT, LEDDATAPIN, NEO_GRB + NEO_KHZ800);

ArduinoNunchuk nunchuk = ArduinoNunchuk();

void setup(){
  Serial.begin(115200);
  nunchuk.init();

  Serial.println("\n\r--- Tetris MADE v0.0 ---\n\r");

  pinMode(LEDDATAPIN, OUTPUT); //LED Strip

  char welcome[] = "Welcome to MADE!  ";
  doScrolling(welcome, sizeof(welcome));

  newGame();
}

void loop(){
  //check_nunchuck();
  //screenTest();   // Check LED connection
  play();
}

unsigned long ulLastMoveMillis = 0;
//plays the game!
void play(){

  // Check is game is over
  if(getHighestColumn() >= GAME_OVER_HEIGHT){
    Serial.println("GAME OVER");
    newGame();
  }

  if(currentBrick.positionY < -1){    // When brick is falling but not yet displayed on screen
    moveDown();
    //pulse onbaord LED and delay game
    digitalWrite(13, HIGH);
    delay(tick_delay);
    digitalWrite(13, LOW);
    delay(tick_delay);
  }
  else{
    //this flag gets set after calculating the AI move
    //and reset after we get the nextbrick in the nextBrick call
    if(aiCalculatedAlready == false)
    {
      performAI();
    }
    else
    {
      byte command = getCommand();

      if(command == CLOCK_WISE) {
        if(checkRotate(1) == true) {
          rotate(1);
          delay(ROTATION_DELAY);
        }
      }else if(command == COUNTER_CLOCK_WISE) {
        if(checkRotate(0) == true) {
          rotate(0);
          delay(ROTATION_DELAY);
        }
      }else if(command == LEFT) {
        if(checkShift(-1, 0) == true) {
          shift(-1, 0);
        }
      } else if(command == RIGHT) {
      if( checkShift(1, 0) == true) {
        shift(1, 0);
      }
    }else if(command == DOWN) {
      moveDown();
    }

    //pulse onbaord LED and delay game
    digitalWrite(13, HIGH);
    delay(tick_delay);
    digitalWrite(13, LOW);
    delay(tick_delay);

    }
  }
  drawGame();

  if(millis() > ulLastMoveMillis + DOWN_DELAY - level_difficulty[level]){
    ulLastMoveMillis = millis();
    moveDown();
  }

}

byte getCommand(){

  nunchuk.update();

  if (gameMode == AI_MODE){

    // Check if Z button is pressed to exit AI mode
    if (nunchuk.zButton){
      delay(20);
      nunchuk.update();
      if (nunchuk.zButton){
        Serial.println("NEW GAME");
        gameMode = USER_MODE;
        newGame();
      }
    }

    //gets commands according to ai state
    if(currentBrick.rotation != aiCurrentMove.rotation)
      return CLOCK_WISE;
    if(currentBrick.positionX > aiCurrentMove.positionX)
      return LEFT;
    if(currentBrick.positionX < aiCurrentMove.positionX)
      return RIGHT;
    if(currentBrick.positionX == aiCurrentMove.positionX)
      return NO_MOVE;
    }
    else{

      // MANUAL MODE
      if (nunchuk.cButton) {
        return CLOCK_WISE;
      }else if(nunchuk.zButton) {
        return COUNTER_CLOCK_WISE;
      }else if(nunchuk.analogX < 50) {
        return LEFT;
      }else if(nunchuk.analogX > 200) {
        return RIGHT;
      }else if(nunchuk.analogY < 50){
        return DOWN;
      }else{
        return NO_MOVE;
      }

  }
}

void scoreOneUpLine()
{
	/*
	 * adds score points... called once per line.
	 */
	score += score_per_line;
	score_lines++;
  scoreAdjustLevel();
}

void scoreAdjustLevel()
{
	if( level < max_level )
	{
		if(score >= level_tresholds[level + 1])
		{
			Serial.println("LEVELUP!");
			Serial.println(score);
			Serial.println(level_tresholds[ level + 1 ]);
			level = level + 1;
			Serial.println(level);
		}
	}
}

//performs AI player calculations.
void performAI(){
  struct TBrick initialBrick;
  //save position of the brick in its raw state
  memcpy((void*)&initialBrick, (void*)&currentBrick, sizeof(TBrick));
  //stores our 20 possible AI moves
  struct TAiMoveInfo aiMoves[4 * FIELD_WIDTH];
  //counter keeps track of the current index into our aimoves array
  byte aiMoveCounter = 0;
  //save position of the the brick at the left most rotated position
  struct TBrick aiLeftRotatedBrick;
  //save position of the brick at the rotated position
  struct TBrick aiRotatedBrick;

  //first check the rotations(initial, rotated once, twice, thrice)
  for(int aiRotation = 0; aiRotation < 4; aiRotation++ )
  {
    //rotate if possible
    if(checkRotate(1) == true)
      rotate(1);
    //save the rotated brick
    memcpy((void*)&aiRotatedBrick, (void*)&currentBrick, sizeof(TBrick));
    //shift as far left as possible
    while(checkShift(-1,0) == true)
      shift(-1, 0);
    //save this leftmost rotated position
    memcpy((void*)&aiLeftRotatedBrick, (void*)&currentBrick, sizeof(TBrick));

    //now check each possible position of X
    for(int aiPositionX = 0; aiPositionX < FIELD_WIDTH; aiPositionX++)
    {
      //next move down until we can't
      while(checkGround() == false )
      {
        shift(0,1);
      }
      //calculate ai weight of this particular final position
      int aiMoveWeight = aiCalculateWeight();
      //save the weight, positions and rotations for this ai move
      aiMoves[aiMoveCounter].weight = aiMoveWeight;
      aiMoves[aiMoveCounter].rotation = currentBrick.rotation;
      aiMoves[aiMoveCounter].positionX = currentBrick.positionX;
      aiMoves[aiMoveCounter].positionY = currentBrick.positionY;
      //move our index up for the next position to save to
      aiMoveCounter++;
      //drawGame();
      //Serial.println(aiMoveWeight);
      //delay(500);

      //now restore the previous position and shift it right by the column # we are checking
      memcpy((void*)&currentBrick, (void*)&aiLeftRotatedBrick, sizeof(TBrick));
      if(checkShift(aiPositionX+1,0) == true)
        shift(aiPositionX+1,0);
    }

    //reload rotated start position
    memcpy((void*)&currentBrick, (void*)&aiRotatedBrick, sizeof(TBrick));
  }

  //at this point we have calculated all the weights of every possible position and rotation of the brick

  //find move with lowest weight
  int lowestWeight = aiMoves[0].weight;
  int lowestWeightIndex = 0;
  for(int i = 1; i < aiMoveCounter; i++)
  {
    if(aiMoves[i].weight <= lowestWeight)
    {
      lowestWeight = aiMoves[i].weight;
      lowestWeightIndex = i;
    }
  }
  //save this AI move as the current move
  memcpy((void*)&aiCurrentMove, (void*)&aiMoves[lowestWeightIndex], sizeof(TAiMoveInfo));
  //restore original brick that we started with
  memcpy((void*)&currentBrick, (void*)&initialBrick, sizeof(TBrick));
  //update the brick, set the ai flag so we know that we dont need to recalculate
  updateBrickArray();
  aiCalculatedAlready = true;
}

//calculates the ai weight
//when this function is called, the currentBrick is moved into a final legal position at the bottom of the wall
//which is why we add it to the wall first and then remove it at the end
int aiCalculateWeight(){
  int weights = 0;
  //add to wall first before calculating ai stuffs
  addToWall();
  //get the two weights
  int highestColumn = getHighestColumn();
  int holeCount = getHoleCount();

  //if this position will yield a full completed row then its weight is 0, which is the lowest possible
  //remember the the lowest weight will be the best move to make
  if(getFullLinePossible() == true)
  {
    weights = 0;
  }
  else
  {
      weights = (HIGH_COLUMN_WEIGHT * highestColumn) + (HOLE_WEIGHT * holeCount);
  }
  removeFromWall(); //undo the wall addition when done
  return weights;
}


//returns how high the wall goes
int getHighestColumn(){
  int columnHeight = 0;
  //count
  int maxColumnHeight = 0;
  for(int j = 0; j < FIELD_WIDTH; j++)
  {
    columnHeight = 0;
    for(int k = FIELD_HEIGHT-1; k!=0; k--)
    {
      if(wall[j][k] != 0)
      {
        columnHeight = FIELD_HEIGHT - k;
        //Serial.print(k);
        //Serial.println(" is k");
        //delay(100);
      }
    }
    if(columnHeight > maxColumnHeight)
      maxColumnHeight = columnHeight;
  }
  return maxColumnHeight;
}

//counts the number of given holes for the ai calculation
int getHoleCount(){
  int holeCount = 0;
  for(int j = 0; j < FIELD_WIDTH; j++)
  {
    for(int k = currentBrick.positionY + 2; k < FIELD_HEIGHT; k++)
    {
      if(wall[j][k] == 0)
        holeCount++;
    }
  }
  return holeCount;
}

//determines if a full line is possible given the current wall (for ai)
bool getFullLinePossible()
{
  int lineCheck;
  for(byte i = 0; i < FIELD_HEIGHT; i++)
  {
    lineCheck = 0;
    for(byte k = 0; k < FIELD_WIDTH; k++)
    {
      if( wall[k][i] != 0)
        lineCheck++;
    }

    if(lineCheck == FIELD_WIDTH)
    {
      return true;
    }
  }
  return false;
}

//checks if the next rotation is possible or not.
bool checkRotate( bool direction )
{
  rotate( direction );
  bool result = !checkCollision();
  rotate( !direction );

  return result;
}

//checks if the current block can be moved by comparing it with the wall
bool checkShift(short right, short down)
{
  shift( right, down );
  bool result = !checkCollision();
  shift( -right, -down );

  return result;
}

// checks if the block would crash if it were to move down another step
// i.e. returns true if the eagle has landed.
bool checkGround()
{
  shift( 0, 1 );
  bool result = checkCollision();
  shift( 0, -1 );
  return result;
}

// checks if the block's highest point has hit the ceiling (true)
// this is only useful if we have determined that the block has been
// dropped onto the wall before!
bool checkCeiling()
{
  for( int i = 0; i < 4; i++ )
  {
    for( int k = 0; k < 4; k++ )
    {
      if(currentBrick.pattern[i][k] != 0)
      {
        if( ( currentBrick.positionY + k ) < 0 )
        {
          return true;
        }
      }
    }
  }
  return false;
}

//checks if the proposed movement puts the current block into the wall.
bool checkCollision()
{
  int x = 0;
  int y =0;

  for( byte i = 0; i < 4; i++ )
  {
    for( byte k = 0; k < 4; k++ )
    {
      if( currentBrick.pattern[i][k] != 0 )
      {
        x = currentBrick.positionX + i;
        y = currentBrick.positionY + k;

        if(x >= 0 && y >= 0 && wall[x][y] != 0)
        {
          //this is another brick IN the wall!
          return true;
        }
        else if( x < 0 || x >= FIELD_WIDTH )
        {
          //out to the left or right
          return true;
        }
        else if( y >= FIELD_HEIGHT )
        {
          //below sea level
          return true;
        }
      }
    }
  }
  return false; //since we didn't return true yet, no collision was found
}

//updates the position variable according to the parameters
void shift(short right, short down)
{
  currentBrick.positionX += right;
  currentBrick.positionY += down;
}

// updates the rotation variable, wraps around and calls updateBrickArray().
// direction: 1 for clockwise (default), 0 to revert.
void rotate( bool direction )
{
  if( direction == 1 )
  {
    if(currentBrick.rotation == 0)
    {
      currentBrick.rotation = 3;
    }
    else
    {
      currentBrick.rotation--;
    }
  }
  else
  {
    if(currentBrick.rotation == 3)
    {
      currentBrick.rotation = 0;
    }
    else
    {
      currentBrick.rotation++;
    }
  }
  updateBrickArray();
}

void moveDown(){
  if( checkGround() )
  {
    addToWall();
    drawGame();
    if( checkCeiling() )
    {
      gameOver();
    }
    else
    {
      while(clearLine())
      {
        scoreOneUpLine();
      }
      nextBrick();
      //scoreOneUpBrick();
    }
  }
  else
  {
    //grounding not imminent
    shift( 0, 1 );
  }
  //scoreAdjustLevel();
  //ticks = 0;
}

//put the brick in the wall after the eagle has landed.
void addToWall()
{
  for( byte i = 0; i < 4; i++ )
  {
    for( byte k = 0; k < 4; k++ )
    {
      if(currentBrick.pattern[i][k] != 0){
        wall[currentBrick.positionX + i][currentBrick.positionY + k] = currentBrick.color;

      }
    }
  }
}

//removes brick from wall, used by ai algo
void removeFromWall(){
  for( byte i = 0; i < 4; i++ )
  {
    for( byte k = 0; k < 4; k++ )
    {
      if(currentBrick.pattern[i][k] != 0){
        wall[currentBrick.positionX + i][currentBrick.positionY + k] = 0;

      }
    }
  }
}

//uses the currentBrick_type and rotation variables to render a 4x4 pixel array of the current block
// from the 2-byte binary reprsentation of the block
void updateBrickArray()
{
  unsigned int data = pgm_read_word(&(bricks[ currentBrick.type ][ currentBrick.rotation ]));
  for( byte i = 0; i < 4; i++ )
  {
    for( byte k = 0; k < 4; k++ )
    {
      if(bitRead(data, 4*i+3-k))
      currentBrick.pattern[k][i] = currentBrick.color;
      else
      currentBrick.pattern[k][i] = 0;
    }
  }
}
//clears the wall for a new game
void clearWall()
{
  for( byte i = 0; i < FIELD_WIDTH; i++ )
  {
    for( byte k = 0; k < FIELD_HEIGHT; k++ )
    {
      wall[i][k] = 0;
    }
  }
}

// find the lowest completed line, do the removal animation, add to score.
// returns true if a line was removed and false if there are none.
bool clearLine()
{
  int line_check;
  for( byte i = 0; i < FIELD_HEIGHT; i++ )
  {
    line_check = 0;

    for( byte k = 0; k < FIELD_WIDTH; k++ )
    {
      if( wall[k][i] != 0)
      line_check++;
    }

    if( line_check == FIELD_WIDTH )
    {
      flashLine( i );
      for( int  k = i; k >= 0; k-- )
      {
        for( byte m = 0; m < FIELD_WIDTH; m++ )
        {
          if( k > 0)
          {
            wall[m][k] = wall[m][k-1];
          }
          else
          {
            wall[m][k] = 0;
          }
        }
      }

      return true; //line removed.
    }
  }
  return false; //no complete line found
}

//randomly selects a new brick and resets rotation / position.
void nextBrick(){
  currentBrick.rotation = 0;
  currentBrick.positionX = round(FIELD_WIDTH / 2) - 1;
  currentBrick.positionY = -3;

  currentBrick.type = random( 0, BRICK_COUNT );
  currentBrick.color = pgm_read_byte(&(brick_colors[ currentBrick.type ]));

  aiCalculatedAlready = false;

  updateBrickArray();

  //displayPreview();
}

//effect, flashes the line at the given y position (line) a few times.
void flashLine( int line ){
  bool state = 1;
  for(byte i = 0; i < 6; i++ )
  {
    for(byte k = 0; k < FIELD_WIDTH; k++ )
    {
      if(state)
      wall[k][line] = 0b11111111;
      else
      wall[k][line] = 0;

    }
    state = !state;
    drawWall();
    updateDisplay();
    delay(200);
  }
}

//clean up, reset timers, scores, etc. and start a new round.
void newGame()
{
  level = 0;
  score = 0;
  score_lines = 0;

  clearWall();
  nextBrick();
}

//tests pixels
void screenTest(){
  for( int k = 0; k < FIELD_HEIGHT; k++ )
  {
    for( int i = 0; i < FIELD_WIDTH; i++ )
    {
      clearWall();
      wall[i][k] = 7;
      drawGame();
      delay(1000);

      Serial.print("LIGHT Pixel x: ");
      Serial.print(i);
      Serial.print(" y: ");
      Serial.println(k);

    }
  }
}

//takes a byte color values an draws it to pixel array at screen x,y values.
//new brightness value lets you dim LEDs w/o changing color.
void draw(byte color, signed int brightness, byte x, byte y){

  unsigned short address=0;
  byte r,g,b;

  //calculate address (LED position on strip)
  y = (FIELD_HEIGHT-1) - y;
  if (y % 2 != 0) x = (FIELD_WIDTH - 1) - x;

  address = x + y*8;

  if(color==0 || brightness < 0){
    strip.setPixelColor(address, 0);
  }
  else{
    //calculate colors, map to LED system
    b=color&0b00000011;
    g=(color&0b00011100)>>2;
    r=(color&0b11100000)>>5;

    //make sure brightness value is correct
    brightness=constrain(brightness,0,FULL);

    strip.setPixelColor(address, map(r,0,7,0,brightness), map(g,0,7,0,brightness), map(b,0,3,0,brightness));
  }
}

//draws wall only, does not update display
void drawWall(){
  for(int j=0; j < FIELD_WIDTH; j++){
    for(int k = 0; k < FIELD_HEIGHT; k++ )
    {
      draw(wall[j][k],FULL,j,k);
    }
  }
}

//'Draws' wall and game piece to screen array
void drawGame()
{
  //draw the wall first
  drawWall();

  //now draw current piece in play
  for( int j = 0; j < 4; j++ )
  {
    for( int k = 0; k < 4; k++ )
    {
      if(currentBrick.pattern[j][k] != 0)
      {
        if( currentBrick.positionY + k >= 0 )
        {
          draw(currentBrick.color, FULL, currentBrick.positionX + j, currentBrick.positionY + k);
          //field[ positionX + j ][ p osition_y + k ] = currentBrick_color;
        }
      }
    }
  }
  updateDisplay();
}

//Update LED strips
void updateDisplay(){
  strip.show();
}

void check_nunchuck()
{
  nunchuk.update();

  Serial.print("X: ");
  Serial.print(nunchuk.analogX, DEC);
  Serial.print("\tY: ");
  Serial.print(nunchuk.analogY, DEC);
  Serial.print("\tAccel X: ");
  Serial.print(nunchuk.accelX, DEC);
  Serial.print("\tAccel Y: ");
  Serial.print(nunchuk.accelY, DEC);
  Serial.print("\tAccel Z: ");
  Serial.print(nunchuk.accelZ, DEC);
  Serial.print("\tSW Z: ");
  Serial.print(nunchuk.zButton, DEC);
  Serial.print("\tSW C: ");
  Serial.println(nunchuk.cButton, DEC);

  delay(800);
}

void doScrolling(char strJuas[], int llarg){

  int colorLeters = 0;
  int strLen = llarg;
  int ptrChar = 0;
  long scrollPosition = 0;

  clearWall();
  updateDisplay();

  while(scrollPosition < ((strLen - 1) * 8)){

    for(int k = FIELD_HEIGHT - 1; k > 0; k-- ){
      for(int j=0; j < FIELD_WIDTH; j++){
        wall[j][k] = wall[j][k-1];
      }
    }

    ptrChar = scrollPosition/8;
    byte charLine = 7 - (scrollPosition-(8 * ptrChar));
    char c = strJuas[ptrChar];

    // Serial.print("Scroll:\t");
    // Serial.print(scrollPosition);
    // Serial.print("\tChar pos:\t");
    // Serial.print(ptrChar);
    // Serial.print("\tLine:\t");
    // Serial.print(charLine);
    // Serial.print("\tChar:\t");
    // Serial.print(c);
    // Serial.print('\t');
    // Serial.println(c,HEX);

    byte bitMask = 0x01;
    char f = pgm_read_byte(&font[c][charLine]);

    if((c == ' ') && (charLine == 0)){
      colorLeters++;
    }
    if (colorLeters > BRICK_COUNT - 1){
      colorLeters = 0;
    }

    uint8_t color = pgm_read_byte(&(words_colors[colorLeters]));

    for (int x = 0; x < 8; x++) {
      if (f & bitMask){
        //Serial.print('O');
        wall[x][0] = color;
      }else{
        wall[x][0] = 0;
        //Serial.print('.');
      }
      bitMask = bitMask << 1;
    }
    //Serial.println('.');

    for( byte i = 0; i < FIELD_WIDTH; i++ ){
      for( byte k = 0; k < FIELD_HEIGHT; k++ ){
        draw(wall[i][k],FULL,i,k);
      }
    }

  scrollPosition++;
  updateDisplay();
  delay(SCROLL_DELAY);
  }

  clearWall();
}

//obvious function
void gameOver()
{

 Serial.println("\n\rGame Over.");
 Serial.print( "Level:\t");
 Serial.println(level);
 Serial.print("Lines:\t");
 Serial.println(score_lines);
 Serial.print( "Score:\t");
 Serial.println(score);
 Serial.println();

 char buf [20];
 sprintf (buf, "Game OVER %lu  ", score);

  int length = 1;
  while (score/= 10 )
    length++;

 doScrolling(buf, length + 13);

 gameMode = AI_MODE;
 newGame();
}
