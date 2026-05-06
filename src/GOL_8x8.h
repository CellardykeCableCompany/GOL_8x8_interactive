#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#define MAX_X 8 // 0-7
#define MAX_Y 8
#define START_DENSITY 3 // higher number less dense the start (e.g. if 8 it's 1 in 8 are set to 1)
#define LIFE_THRESHOLD 3
#define NUM_GRID 6
#define GRID_SELECTION 4 // 0 = random;

extern const int sgrid[NUM_GRID][MAX_Y][MAX_X] PROGMEM;

class GOL_8x8
{
public:

    // consrtuctor
    GOL_8x8(); 
    // GOL functions

    void setup(); 
    void update();
    int countNeighbour(int, int);
    void setGrid();
    void writeGrid();


private:
    Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

    int grid[MAX_Y][MAX_X] = {0};
    int nextGrid[MAX_Y][MAX_X] = {0};
};