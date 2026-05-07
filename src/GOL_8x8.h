#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <AceButton.h>

#define MAX_X 8 // 0-7
#define MAX_Y 8
#define START_DENSITY 3 // higher number less dense the start (e.g. if 8 it's 1 in 8 are set to 1)
#define LIFE_THRESHOLD 3
#define NUM_GRID 6
#define GRID_SELECTION 0 // 0 = random;
#define RUN_SPEED 200 // runspeed delay in millis
#define S1 A2 // switch1 on the joystick
#define ADC1 A0
#define ADC2 A1


extern const int sgrid[NUM_GRID][MAX_Y][MAX_X] PROGMEM;
extern int joyButtonState;  

class GOL_8x8
{
public:

    // consrtuctor
    GOL_8x8(); 
    // GOL functions

    void setup(); 
    void update();
    int countNeighbour(int, int);
    void setGrid(int);
    void writeGrid();
    void reset();

    // AceButton use
    static void handleEvent(ace_button::AceButton*, uint8_t, uint8_t);
    

private:
    Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
    ace_button::AceButton button;
   

    int grid[MAX_Y][MAX_X] = {0};
    int nextGrid[MAX_Y][MAX_X] = {0};
    long int currentTime; 
    long int previousTime; 
    


    bool is_update; 
};