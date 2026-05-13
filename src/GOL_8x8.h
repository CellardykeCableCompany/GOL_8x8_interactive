#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <AceButton.h>


// the more i look at this code - the more it needs restrucruing around 2 clasess - GOL and Joysitck
// but for now like this.. 

// GOL defines
#define MAX_X 8 // 0-7
#define MAX_Y 8
#define START_DENSITY 3 // higher number less dense the start (e.g. if 8 it's 1 in 8 are set to 1)
#define LIFE_THRESHOLD 3
#define NUM_GRID 6
#define GRID_SELECTION 0 // 0 = random;
#define RUN_SPEED 200    // runspeed delay in millis
#define S1 A2            // switch1 on the joystick
#define S2 A7            // separate switch
#define ADC1 A0
#define ADC2 A1


// joystick defines
#define MIN_JX 263 // calibrated joystick values
#define MAX_JX 878
#define MIN_JY 263
#define MAX_JY 853
#define OFF_JX 0
#define OFF_JY 0
#define MAX_MX 8
#define MAX_MY 8
#define JOY_MODE 1        // way the joystick changes x, y values
#define JOY_THRESH 150    // threhold joystick has to move before increment happens (buffer zone in middle)
#define BLINK_ON_TIME 200 // using this as an equal blink on/off time is visually disturbing
#define BLINK_OFF_TIME 1


// untidy - putting as externs here - need to figure out better way to do thi
extern const int sgrid[NUM_GRID][MAX_Y][MAX_X] PROGMEM;
extern int joyButtonState;

class GOL_8x8
{
public:
    // consrtuctor
    GOL_8x8();
    // GOL functions
    // general  -> these need to be public as accessed in main.
    // good practice to put all the other functions protected in private (unless accessed by main)
    void setup();
    void update();
    // AceButton use
    static void handleEvent(ace_button::AceButton *, uint8_t, uint8_t);

private:
    Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
    ace_button::AceButton button;

    //int ADC1, ADC2, S2; // S1 handled by Acebutton - want S2 handled indpendently
    long int midPointJoyX, midPointJoyY;

    int grid[MAX_Y][MAX_X] = {0};
    int nextGrid[MAX_Y][MAX_X] = {0};
    long int currentTime;
    long int previousTime;
    bool is_update;
    bool is_paused;


     unsigned long previousTimeBlink = 0;
  unsigned long currentTimeBlink;
  unsigned int blinkInterval;

  unsigned long currentTimeJoy; // maybe same as currentTimeBlink - can swap out for generic "time" variable...
  unsigned long previousTimeJoy;
  const unsigned int joyInterval = 100; // speed at which the joystick moves...

  int vertical; // variable to store the value read
  int horizontal;

  // separate switch that (for now) doesn't need to use AceButton
    int switch2; // separate switch - not the joystick one. 
  int oldSwitch2; // previous state of switch (to test for press/release etc.)

  // matrix
  bool is_blink;

  // joystick
  bool is_move;
  // cursor position on matrix
  int mX;
  int mY;

  // initiate joysick values as mid (512) min_x, max_x, min_y, max_y
  int joystickLimits[4] = {512, 512, 512, 512};

    // GOL


    void play(); 
    int countNeighbour(int, int);
    void setGrid(int);
    void writeGrid();
    void reset();

    // draw
    void read(int &, int &, int);
    void follow(int, int);
    void draw(int, int, int);
    void blink(int, int);
};