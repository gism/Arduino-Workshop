//------------------------------------------------------------------------------
// 8x8 single color LED Tetris for Arduino UNO.
// https://importhack.wordpress.com/
//------------------------------------------------------------------------------
// The below code has been done form the not finished code:
// http://www.github.com/MarginallyClever/8x8LEDtetris

/*
 HARDWARE
 ---
 The LED grid is a red 8x8 model 1088BS.
 The Arduino is an UNO.
 
 WIRING
 ----
 Place the grid so the lights are facing you and the nub on one edge is on the
 bottom.  On the back should be two horizontal rows of pins.  the top row, from
 left to right, connects to arduino digital pins 9-2.  The bottom row of pins,
 from left to right, connect to digital pins 13-10 and analog pins 2-5.

PLAYING
----
Commands sent through Serial (9600):
a: move left
d: move right
s: move down
w: rotate

*/

//--------------------------------------------------------------------------------
// CONSTANTS
//--------------------------------------------------------------------------------

#define LETTER_W         (6)
#define MESSAGE_LEN      (4)
#define MESSAGE_W        (MESSAGE_LEN * LETTER_W)
#define LETTER_H         (8)
#define GRID_W           (8)
#define GRID_H           (8)
#define PIECE_W          (4)
#define PIECE_H          (4)

#define ON               LOW
#define OFF              HIGH 

#define NUM_PIECE_TYPES  (7)

// a list of cathode pins, sorted by top to bottom of the grid
const int cathode[8] = { 8,13,7,11,0,6,1,4 };

// a list of anode pins, sorted by left to right of the grid
const int anode[8] = { 12,2,3,9,5,10,14,15 };


// translate the pins on the LED panel to pins on the Arduino
const int arduino_to_grid[16] = { 13,12,11,10, 16,17,18,19, 2,3,4,5, 6,7,8,9 };

// Usual tetris pieces
// each piece is max 4 wide, 4 tall, and 4 rotations
const char piece_I[] = {
  0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0,

  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
  
  0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0,

  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
};

const char piece_L1[] = {
  0,1,0,0,
  0,1,0,0,
  0,1,1,0,
  0,0,0,0,

  0,0,0,0,
  1,1,1,0,
  1,0,0,0,
  0,0,0,0,
  
  1,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,0,1,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0,
};

const char piece_L2[] = {
  0,1,0,0,
  0,1,0,0,
  1,1,0,0,
  0,0,0,0,

  1,0,0,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0,
  
  0,1,1,0,
  0,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,0,0,0,
  1,1,1,0,
  0,0,1,0,
  0,0,0,0,
};

const char piece_T[] = {
  0,0,0,0,
  1,1,1,0,
  0,1,0,0,
  0,0,0,0,

  0,1,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,1,0,0,
  1,1,1,0,
  0,0,0,0,
  0,0,0,0,

  0,1,0,0,
  0,1,1,0,
  0,1,0,0,
  0,0,0,0,
};

const char piece_S1[] = {
  1,0,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,1,1,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,

  1,0,0,0,
  1,1,0,0,
  0,1,0,0,
  0,0,0,0,

  0,1,1,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
};

const char piece_S2[] = {
  0,1,0,0,
  1,1,0,0,
  1,0,0,0,
  0,0,0,0,

  1,1,0,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0,
  
  0,1,0,0,
  1,1,0,0,
  1,0,0,0,
  0,0,0,0,

  1,1,0,0,
  0,1,1,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
};

const char piece_O[] = {
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
  
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
  
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
  
  1,1,0,0,
  1,1,0,0,
  0,0,0,0,
  0,0,0,0,
};

const char *pieces[] = {
  piece_S1,
  piece_S2,
  piece_L1,
  piece_L2,
  piece_O,
  piece_T,
  piece_I,
};

#define MAX_CHANCES (13)

const char chances[MAX_CHANCES] = {
  0,0,
  1,1,
  2,2,
  3,3,
  4,4,
  5,5,
  6,  // 1 in 13 chance of an I piece
};

int piece_id;
int piece_rotation;
int piece_x;
int piece_y;

long last_move;
long move_delay=100;

long last_drop;
long drop_delay=500;

long last_letter_milis;
long letter_delay=800;

static int grid[8*8];            // Actual game play
static int grid_virtual[8*8];    // Virtual game play, I use it to check if a move is possible or not.

char game_over_str[] = "GAME OVER ";
int strLen = sizeof(game_over_str);
int ptrChar = 0;

// Thanks https://github.com/dhepper/font8x8 for that beautiful font!
static byte font[91][8] = {
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
};


//--------------------------------------------------------------------------------
// METHODS
//--------------------------------------------------------------------------------

// I want to turn on the column N from the left.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int out(int x) {
  return arduino_to_grid[anode[x]];
}

// I want to turn on the row N from the top.
// this figures out which pin on the LED that is,
// then figures out which pin on the arduino matches that LED pin.
// two translations!
int in(int y) {
  return arduino_to_grid[cathode[y]];
}

// I want to turn on point P(x,y), which is X from the left and Y from the top.
// I might also want to hold it on for us microseconds.
void p(int x,int y,int us) {
  // don't try to turn on a light that doesn't exist
  if(x<0 || x>GRID_W) return;
  if(y<0 || y>GRID_H) return;
  
  // now light it
  digitalWrite(out(x),ON);
  digitalWrite(in(y),OFF);
  delayMicroseconds(us);
  digitalWrite(in(y),ON);
  digitalWrite(out(x),OFF);
}

// draw a piece from (px,py) to (px+x,py+y) on the grid
void draw_piece(int px,int py) {
  int x, y;
  
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {
      if( piece[y*PIECE_W+x] == 1 ) {
        p(px+x,py+y,150);
      }
    }
  }
}

// grid contains the arduino's memory of the game board, including the piece that is falling.
void draw_grid() {
  int x, y;
  for(y=0;y<GRID_H;++y) {
    for(x=0;x<GRID_W;++x) {
      if( grid[y*GRID_W+x] != 0 ) {
        p(x,y,150);
      }
    }
  }
}

void choose_new_piece() {
  // make the chances array longer to change the odds of different pieces appearing.
  // This is not realy random. It needs to be corrected...
  piece_id = chances[rand() % MAX_CHANCES];
  
  // always start the piece top center.
  piece_y=-4;  // -4 squares off the top of the screen.
  piece_x=3;
  // always start in the same orientation.
  piece_rotation=0;
}

void erase_piece_from_grid() {
  int x, y;
  
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {
      int nx=piece_x+x;
      int ny=piece_y+y;
      if(ny<0 || ny>GRID_H) continue;
      if(nx<0 || nx>GRID_W) continue;
      if(piece[y*PIECE_W+x]==1) {
        grid[ny*GRID_W+nx]=0;  // zero erases the grid location.
      }
    }
  }
}

void add_piece_to_grid() {
  int x, y;
  
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {
      int nx=piece_x+x;
      int ny=piece_y+y;
      if(ny<0 || ny>GRID_H) continue;
      if(nx<0 || nx>GRID_W) continue;
      if(piece[y*PIECE_W+x]==1) {
        grid[ny*GRID_W+nx]=1;  // zero erases the grid location.
      }
    }
  }
}

// Move everything down 1 space, destroying the old row number y in the process.
void delete_row(int y) {
  int x;
  for(;y>0;--y) {
    for(x=0;x<GRID_W;++x) {
      grid[y*GRID_W+x] = grid[(y-1)*GRID_W+x];
    }
  }
  // everything moved down 1, so the top row must be empty or the game would be over.
  for(x=0;x<GRID_W;++x) {
    grid[x]=0;
  }
}

void check_remove_lines() {
  int x, y, c;
  for(y=0;y<GRID_H;++y) {
    // count the full spaces in this row
    c = 0;
    for(x=0;x<GRID_W;++x) {
      if( grid[y*GRID_W+x] > 0 ) c++;
    }
    if(c==GRID_W) {
      // row full!
      delete_row(y);
    }
  }  
}

int try_to_move_piece_sideways(int dir) {
  
  int new_px = piece_x;
  
  if(dir == 100) new_px++;
  if(dir == 97) new_px--;
  
  if(can_piece_fit(new_px,piece_y,piece_rotation)) {
    piece_x=new_px;
  }
}


void try_to_rotate_piece() {

    // figure out what it will look like at that new angle
    int new_pr = ( piece_rotation + 1 ) % 4;
    // if it can fit at that new angle (doesn't bump anything)
    if(can_piece_fit(piece_x,piece_y,new_pr)) {
      // then make the turn.
      piece_rotation = new_pr;
    }
}

int can_piece_drop() {
  int x,y;
  
  // Generates a virtual screen just as acual  
  for(y=0;y<GRID_H;y++) {
    for(x=0;x<GRID_W;x++) {
      grid_virtual[y*GRID_W+x] = grid[y*GRID_W+x];
    }
  }
      
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  // Erease actual piece from screen
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {
      int nx=piece_x+x;
      int ny=piece_y+y;
      if(ny<0 || ny>GRID_H) continue;
      if(nx<0 || nx>GRID_W) continue;
      if(piece[y*PIECE_W+x]==1) {
        grid_virtual[ny*GRID_W+nx]=0;  // zero erases the grid location.
      }
    }
  }
  
  // Drow piece on future possition
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {
      int nx=piece_x+x;
      int ny=piece_y+y+1;
      if(ny<0 || ny>GRID_H) continue;
      if(nx<0 || nx>GRID_W) continue;
      if(piece[y*PIECE_W+x]==1) {
        if(ny==8) { return 0;}
        grid_virtual[ny*GRID_W+nx]= grid_virtual[ny*GRID_W+nx] + 1;  // The magic is done here, if there is some collision value would be 2. 
      }
    }
  }
  
  // Check if there is any collision on virtual screen:
  for(y=0;y<GRID_H;y++) {
    for(x=0;x<GRID_W;x++) {
      if(grid_virtual[y*GRID_W+x] == 2){return 0;}
    }
  }
  
  // Case where no virtual collision
  return 1;  // can drop  
}


// can the piece fit in this new location?
int can_piece_fit(int px,int py,int pr) {
  int x,y;
  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {      
      int ny=py+y;
      int nx=px+x;
      if(piece[y*PIECE_W+x]>0) {
        if(nx<0) return 0;  // no: off left side
        if(nx>=GRID_W ) return 0;  // no: off right side
        if(ny>=GRID_H ) return 0;  // no: goes off bottom of grid      
        if(grid[ny*GRID_W+nx]==1 ) return 0;  // no: grid already full in this space
      }
    }
  }
  
  return 1;  // can fit
}

// Game over screen
void game_over() {
  last_letter_milis = millis();
  
  setupChar();
  while(1) {
    draw_grid();
    if(millis() - last_letter_milis > letter_delay) {
      last_letter_milis = millis();
      setupChar();
    }
  }
}


void try_to_drop_piece() {
  if(can_piece_drop()==1) {
    erase_piece_from_grid();
    piece_y++;  // move piece down
    add_piece_to_grid();
  } else {
    // hit something!
    check_remove_lines();
    if(check_game_over()) {
      game_over();
    }
    choose_new_piece();
  }
}


void try_to_drop_faster() {
    try_to_drop_piece();
}


void react_to_player() {
  int rot = 0;
  int mov = 0;
  int dow = 0;
  int incomingByte = 0;
  
  // Joystick interface changed to UART interface. Sorry no Joystick available now, maybe in future.
  while (Serial.available()) {
    incomingByte = Serial.read();
    
    // If "a" or "d" send then move.
    if(incomingByte == 97 || incomingByte == 100){
      if(!mov){
        erase_piece_from_grid();
        try_to_move_piece_sideways(incomingByte);
        add_piece_to_grid();
        mov = 1;
      }
    }
    
    // If "w" is send then rotate
    if(incomingByte == 119){
      if(!rot){
        erase_piece_from_grid();
        try_to_rotate_piece();
        add_piece_to_grid();
        rot = 1;
      }
    }
    
    // if "s" is send then move down
    if(incomingByte == 115){
      if(!dow){
        erase_piece_from_grid();
        try_to_drop_faster();
        add_piece_to_grid();
        dow = 1;
      }
    }
    
  }
}


int check_game_over() {
  // can the piece fit in this new location
  int x,y;
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);
  
  for(y=0;y<PIECE_H;++y) {
    for(x=0;x<PIECE_W;++x) {      
      int ny=piece_y+y;
      int nx=piece_x+x;
      if(piece[y*PIECE_W+x]>0) {
        if(ny<0) return 1;  // yes: off the top!
      }
    }
  }
  
  return 0;  // not over yet...
}


// called once when arduino reboots
void setup() {
  Serial.begin(9600);
  
  int i;
  // set all the pins to output.
  // 16 could be defined as NUM_INPUTS+NUM_OUTPUTS
  for(i=0;i<16;++i) {
    pinMode(arduino_to_grid[i],OUTPUT);
  }

  // turn every light off.
  for(i=0;i<8;++i) {
    digitalWrite(out(i),OFF);
  }
  for(i=0;i<8;++i) {
    digitalWrite(in(i),ON);
  }
  
  // set up joystick click
  pinMode(1,INPUT);
  digitalWrite(1,HIGH);
  
  // clear the grid
  for(i=0;i<8*8;++i) {
    grid[i]=0;
  }
  
  // make the game a bit more random - pull a number from space and use it to 'seed' a crop of random numbers.
  randomSeed(analogRead(1));
  
  // get ready to start the game.
  choose_new_piece();
  
  // start the game clock after everything else is ready.
  last_move = millis();
  last_drop = last_move;
  
  Serial.println("I'm a 8x8 LED tetris, use 'a', 'd', 'w' and 's' to play.");
  Serial.println("More information: https://importhack.wordpress.com/");
}

// called over and over after setup()
void loop() {
  // the game plays at one speed,
  if(millis() - last_move > move_delay ) {
    last_move = millis();
    react_to_player();
  }
  
  // ...and drops the falling block at a different speed.
  if(millis() - last_drop > drop_delay ) {
    last_drop = millis();
    try_to_drop_piece();
  }
  
  // when it isn't doing those two things, it's redrawing the grid.
  draw_grid();
}

// Print a fancy char on LED matrix.
void setupChar(){
  char c = game_over_str[ptrChar];

  for (int x = 0; x < 8; x++) {
    byte bitMask = 0x01;
    byte f = font[c-32][x];
    for (int y = 0; y < 8; y++) {
      if (f & bitMask){
        grid[x*GRID_W+y] = HIGH;
      }else{
        grid[x*GRID_W+y] = LOW;
      }
      bitMask = bitMask << 1;
    }
  }

  ptrChar++;
  if(ptrChar>=strLen-1){
    ptrChar = 0;
  }

}

// Just for debug.
void PrintGrid(){
  int x,y;
  
  for(y=0;y<GRID_H;y++) {
    for(x=0;x<GRID_W;x++) {
      Serial.print(grid[y*GRID_W+x]);
      Serial.print("|");
    }
    Serial.println();
  }
  Serial.println();
}

void PrintGridVirtual(){
  int x,y;
  
  for(y=0;y<GRID_H;y++) {
    for(x=0;x<GRID_W;x++) {
      Serial.print(grid_virtual[y*GRID_W+x]);
      Serial.print("|");
    }
    Serial.println();
  }
  Serial.println();
}

/**
* This file is part of LED8x8tetris.
*
* LED8x8tetris is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* LED8x8tetris is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Arduino Timer Interrupt. If not, see <http://www.gnu.org/licenses/>.
*/
