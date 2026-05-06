#include "GOL_8x8.h"

// define the actual data here
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

GOL_8x8::GOL_8x8()
{
    // constructor  - initialise all variables here
}

// public
void GOL_8x8::setup()
{
    Serial.begin(9600);
    matrix.begin(0x70); // pass in the address 0x70 for led backpack...

    // initialise grid
    randomSeed(analogRead(0));
    setGrid();
    writeGrid();
    delay(1000);
}

void GOL_8x8::update()
{
    // scan the grid in different ways
    // simplest left (0) to right (7), bottom (0) to top (7)

    // make sure gridge is initiated according to a rule
    // could be random... or a pattern - read more

    // initiate new grid - the one you write to

    int new_grid[MAX_Y][MAX_X] = {0}; // buffer

    // now scan grid and set values based on GOL rules - above
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            int val = countNeighbour(x, y);
            if (grid[x][y] == 1)
            {
                // cell is alive (1)
                if ((val == (LIFE_THRESHOLD - 1)) || (val == LIFE_THRESHOLD))
                {
                    new_grid[x][y] = 1;
                }
                else
                {
                    // it dies (0)
                    new_grid[x][y] = 0;
                }
            }
            if (grid[x][y] == 0)
            {
                // cell is dead (0)
                if (val == LIFE_THRESHOLD)
                {
                    new_grid[x][y] = 1;
                }
                else
                {
                    new_grid[x][y] = 0; // stays dead
                }
            }
        }
    }

    // now copy new_grid to grid.

    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            grid[x][y] = new_grid[x][y];
        }
    }
    writeGrid();
}

int GOL_8x8::countNeighbour(int x, int y)
{

    // count neighbouring cells in the grid
    // note this counts itself! so an issue there...
    int val = 0;
    int vx, vy;

    int lx = x - 1; // left of x
    int rx = x + 1; // right of x
    int ay = y - 1; // above y
    int by = y + 1; // below y

    // wrap as toroidal
    if (x == 0)
        lx = MAX_X - 1;
    if (x == MAX_X - 1)
        rx = 0;
    if (y == 0)
        ay = MAX_Y - 1;
    if (y == MAX_Y - 1)
        by = 0;

    // scan three rows

    // row above - 1
    val += grid[lx][ay] + grid[x][ay] + grid[rx][ay];

    // row it's on 0
    val += grid[lx][y] + grid[rx][y];

    // below +1
    val += grid[lx][by] + grid[x][by] + grid[rx][by];

    return val;
}

void GOL_8x8::setGrid()
{
    // set all elements in the grid to zero
    // grid[MAX_Y][MAX_X] = {0};

    if (GRID_SELECTION == 0)
    {
        // randomly asign 1 or 0 into an element
        for (int y = 0; y < MAX_Y; y++)
        {
            for (int x = 0; x < MAX_X; x++)
            {
                if (random(0, START_DENSITY) < 1)
                {
                    grid[x][y] = 1;
                }
                else
                {
                    grid[x][y] = 0;
                }
            }
        }
    }
    else
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            for (int x = 0; x < MAX_X; x++)
            {
                grid[x][y] = sgrid[GRID_SELECTION - 1][y][x]; // -1 as 0 is random // note swapped y, x in sGrid to align with the led matrix positioning... not sure why... fix!
            }
        }
    }
}

void GOL_8x8::writeGrid()
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
