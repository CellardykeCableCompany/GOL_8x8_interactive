/*
  Conway Game of Life on Adafruit 8x8 LED Matrix (single colour)

  great starting point 
 https://github.com/tobyoxborrow/gameoflife-arduino/blob/master/GameOfLife/GameOfLife.ino

 adapted 
 jon rogers
 01.05.2026
 
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

 we'll treat edges as connected - they can see the other edge - so it wraps
 other options https://www.chaos.org.uk/~eddy/craft/weblife.html

*/ 
#include "GOL_8x8.h"; 
 int joystickLimits[4] = {512, 512, 512, 512};

GOL_8x8 myGOL; 

void setup() {

  myGOL.setup();  // should do something like setup(START_GRID)

}

void loop() {

 
 myGOL.update(); 
  


  
 
}







