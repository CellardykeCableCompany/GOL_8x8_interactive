/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED Matrix backpacks 
  ----> http://www.adafruit.com/products/872
  ----> http://www.adafruit.com/products/871
  ----> http://www.adafruit.com/products/870

  These displays use I2C to communicate, 2 pins are required to 
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "JoystickLib.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// from demo example
static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 },
  mouthOpen_bmp[] =
  { B00000000,
    B01000010,
    B00000000,
    B00011000,
    B00111100,
    B00111100,
    B00011000,
    B00000000 },
    mouthShut_bmp[] =
  { B00000000,
    B01000000,
    B00000000,
    B00000000,
    B00011000,
    B00011000,
    B00000000,
    B00000000 };


  
void face_laugh();
void ball_bounce(); 
int bx = 2, by = 1;
int dx = 1, dy = 1;



// GOL functions
void game_of_life(); 
int countNeighbour(int, int);
void setGrid (); 
void writeGrid ();


#define MAX_X 8 // 0-7
#define MAX_Y 8
#define START_DENSITY 3 // higher number less dense the start (e.g. if 8 it's 1 in 8 are set to 1)
#define LIFE_THRESHOLD 3

int grid[MAX_Y][MAX_X] = {0};
// optional set up grids
#define NUM_GRID 6
#define GRID_SELECTION 0 // 0 = random; 

// inspiration from https://pi.math.cornell.edu/~lipa/mec/lesson6.html
const int sgrid[NUM_GRID][MAX_Y][MAX_X] PROGMEM = {
  {
      // 1 https://github.com/tobyoxborrow/gameoflife-arduino/blob/master/GameOfLife/GameOfLife.ino#L38
        {1, 1, 0, 0, 1, 1, 0, 0},
        {1, 1, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 1, 1},
        {0, 0, 1, 1, 0, 0, 1, 1},
    }, 
    {
       // 2  https://github.com/tobyoxborrow/gameoflife-arduino/blob/master/GameOfLife/GameOfLife.ino#L38
        {1, 0, 1, 0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0, 1, 1, 0},
        {0, 1, 0, 0, 0, 1, 0, 1},
    },
       {
        //  3 https://conwaylife.com/wiki/Mold
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 0},
        {0, 0, 1, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    },
      {
        // 4 slider 
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
        // 5 slider (2)
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
      },
       {
        // 6 slider (3)
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        
      },
  }; 





void setup() {
  Serial.begin(9600);
  Serial.println("8x8 LED Matrix Test");
  
  matrix.begin(0x70);  // pass in the address 0x70 for led backpack... 

  // initiate grid - as random
  // firt initiate random seed value
  randomSeed(analogRead(0));
  setGrid(); 
  writeGrid(); 
  delay (1000);
  

}

void loop() {
 
//face_laugh();
  //ball_bounce();
 game_of_life();
 delay(100);


}

void ball_bounce()
{
  matrix.clear();
  matrix.drawPixel(bx, by, LED_ON);
  matrix.writeDisplay();

  bx += dx;
  by += dy;

  if (bx <= 0 || bx >= 7) dx = -dx;
  if (by <= 0 || by >= 7) dy = -dy;

  delay(150);

}

void face_laugh()
{

 matrix.clear();
  matrix.drawBitmap(0, 0, mouthOpen_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(400);

  matrix.clear();
  matrix.drawBitmap(0, 0, mouthShut_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(200);

}

void game_of_life()
{
// planning here for Conway Game of Life

// first write to matrix - to have an individual point come on
// test this by making a bouncing ball (could read accellerometer)

// example  https://github.com/tobyoxborrow/gameoflife-arduino/blob/master/GameOfLife/GameOfLife.ino

/* ----------------
rules
from here
https://playgameoflife.com/info


FOR EACH CELL THAT IS POPULATED (ALIVE - 1)
Each cell with one or no neighbors dies, as if by solitude.
Each cell with four or more neighbors dies, as if by overpopulation.
Each cell with two or three neighbors survives.

FOR EACH CELL THAT IS UNPOPULATED (DEAD - 0 )
Each cell with exactly three neighbors becomes populated.
Otherwise it dies. 
*/

// we'll treat edges as connected - they can see the other edge - so it wraps
// other options https://www.chaos.org.uk/~eddy/craft/weblife.html

/// scan the grid in different ways
// simplest left (0) to right (7), bottom (0) to top (7)

// make sure gridge is initiated according to a rule
// could be random... or a pattern - read more 

// initiate new grid - the one you write to

  int new_grid[MAX_Y][MAX_X] = {0}; // buffer 

  // now scan grid and set values based on GOL rules - above
  for (int y=0; y<MAX_Y; y++){
    for (int x=0; x<MAX_X; x++){
      int val = countNeighbour(x, y);
      if(grid[x][y]==1){
        // cell is alive (1) 
       if ((val==(LIFE_THRESHOLD-1)) || (val==LIFE_THRESHOLD)){
          new_grid[x][y]=1;
	}
       else{
        // it dies (0) 
        new_grid[x][y]=0; 
       }
      }
      if (grid[x][y]==0){
        // cell is dead (0)
        if (val==LIFE_THRESHOLD) {
          new_grid[x][y]=1; 
        }  
        else {
          new_grid[x][y] = 0; // stays dead
        }
       }
      }
    }

    // now copy new_grid to grid.

    for(int y=0; y<MAX_Y; y++){
      for(int x=0; x<MAX_X; x++){
        grid[x][y] = new_grid[x][y];
    }
  }
  writeGrid(); 


}

int countNeighbour(int x, int y)
{
  // count neighbouring cells in the grid 
  // note this counts itself! so an issue there... 
 int val=0;
 int vx, vy; 

 int lx = x-1;  // left of x
 int rx = x+1;  // right of x
 int ay = y-1;  // above y
 int by = y+1;  // below y
 
 // wrap as toroidal
  if (x==0)
   lx = MAX_X-1; 
  if (x==MAX_X-1)
   rx = 0; 
  if (y==0) 
   ay = MAX_Y-1; 
  if (y==MAX_Y-1)
   by = 0; 


 // scan three rows

 // row above - 1
 val+= grid[lx][ay] + grid[x][ay] + grid [rx][ay];

 // row it's on 0 
 val+= grid[lx][y] + grid [rx][y];

 // below +1
 val+= grid[lx][by] + grid [x][by] + grid [rx][by];

/*
  for (int ys=-1; ys<2; ys++){
    for (int xs=-1; xs<2; xs++){
      
      // wrap around cell edges
      if (x+xs<0){
        vx = MAX_X-1; 
      }
      else if (x+xs>MAX_X-1) 
        vx = 0; 
      else {
        vx = x+xs; 
	}

      if (y+ys<0) {
        vy = MAX_Y-1;  
      }
      else if (y+ys>MAX_Y-1 ){
        vy=0; 
      }
      else{
        vy = y+ys; 
      }
      // do the sum on all these
      val = val + grid[vx][vy];   
    }

  }
*/
    
 return val; 

}

void setGrid ()
{
  // set all elements in the grid to zero
 // grid[MAX_Y][MAX_X] = {0};


  if (GRID_SELECTION==0)
  {
  // randomly asign 1 or 0 into an element
  for(int y=0; y<MAX_Y; y++){
    for(int x=0; x<MAX_X; x++){
       if (random(0, START_DENSITY) < 1) {
            grid[x][y] = 1;
	}
      else {
        grid[x][y]= 0; 
      }
    }
  }
}
 else{
    for(int y=0; y<MAX_Y; y++){
      for(int x=0; x<MAX_X; x++){
        grid [x][y] = sgrid[GRID_SELECTION-1][y][x]; // -1 as 0 is random // note swapped y, x in sGrid to align with the led matrix positioning... not sure why... fix!
      }
    }
   
  }
}

void writeGrid ()
{
 // write contents of the grid to the LED matrix

  matrix.clear();
  
  

  for (int y=0; y<MAX_Y; y++){
    for (int x=0; x<MAX_X; x++){
      if (grid[x][y]==1)
        matrix.drawPixel(x, y, LED_ON);
      else 
       matrix.drawPixel(x, y, LED_OFF);  
	}
  }
  //matrix.writePixel(7,0, LED_ON);

  matrix.writeDisplay();


}







