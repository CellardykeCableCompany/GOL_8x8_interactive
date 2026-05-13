#include "GOL_8x8.h"
int joyButtonState = 0;
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

    joyButtonState = 0;
    is_paused = false;

    switch2 = 0;
    oldSwitch2 = 0;

    is_blink = false;
    is_move = false;
    // midPointJoyX = ((MAX_JX - MIN_JX) / 2) + MIN_JX + OFF_JX;
    // midPointJoyY = ((MAX_JY - MIN_JY) / 2) + MIN_JY + OFF_JY;
    midPointJoyX = 500;
    midPointJoyY = 500;

    mX = 4;
    mY = 4;
}

// public
void GOL_8x8::setup()
{
    Serial.begin(9600);
    matrix.begin(0x70); // pass in the address 0x70 for led backpack...

    // initialise grid
    randomSeed(analogRead(0));
    setGrid(GRID_SELECTION);
    writeGrid();
    // delay(1000);
    is_update = true;
    previousTime = millis();

    // joystick
    // switch
    pinMode(S1, INPUT_PULLUP); // joystick switch
    pinMode(S2, INPUT_PULLUP); // 2nd switch

    // AceButton for the joystick
    button.init(S1);

    // Configure the ButtonConfig with the event handler, and enable all higher
    // level events.
    ace_button::ButtonConfig *buttonConfig = button.getButtonConfig();
    buttonConfig->setEventHandler(handleEvent);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureClick);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureDoubleClick);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureLongPress);
    buttonConfig->setFeature(ace_button::ButtonConfig::kFeatureRepeatPress);

    // Check if the button was pressed while booting
    if (button.isPressedRaw())
    {
        Serial.println(F("setup(): button was pressed while booting"));
    }

    Serial.println(F("setup(): ready"));
}

void GOL_8x8::update()
{
    // reverse value for switch (better way to to do this i am sure.. )
    int val;
    switch2 = digitalRead(S2);
   // Serial.println(switch2); 
    // reverse switch value

    // need to have switch2 in here (separate switch)
    // use logic/timing rather than AceButton
    read(mX, mY, JOY_MODE);
    if (switch2 == 0 && oldSwitch2 == 1)
    {
        //Serial.println("pressed"); 
        if (grid[mX][mY] == 0)
            val = 1;
        else
            val = 0;

        draw(mX, mY, val);
    }
    oldSwitch2=switch2; 

    // Serial.println(switch2);

    // AceButton
    button.check();
    // Serial.print(joyButtonState);
    // joyButtonState=0;
    if (joyButtonState == 3)
    {
        reset();
        joyButtonState = 0xFF;
    }
    if (joyButtonState == 4)
    {
        Serial.println("paused! ");
        is_paused = !is_paused;
        joyButtonState = 0xFF;
    }

    // current time for GOL -> need to change to currentTimeGOL
    currentTime = millis();

    if (is_paused == 0)
    {

        if (currentTime - previousTime >= RUN_SPEED)
            is_update = true;

        if (is_update)
        {
            play();
        }
    }

    writeGrid();
    blink(mX, mY);
    matrix.writeDisplay();

    // Serial.print(mX); Serial.print("  "); Serial.println(mY);
}

void GOL_8x8::play()
{

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
    // writeGrid();
    previousTime = currentTime;
    is_update = false;
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

void GOL_8x8::setGrid(int mode)
{

    if (mode == 0)
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
                grid[x][y] = sgrid[mode - 1][y][x]; // -1 as 0 is random // note swapped y, x in sGrid to align with the led matrix positioning... not sure why... fix!
            }
        }
    }
}

void GOL_8x8::writeGrid()
{
    // write contents of the grid to the LED matrix

    matrix.clear();

    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            if (grid[x][y] == 1)
                matrix.drawPixel(x, y, LED_ON);
            else
                matrix.drawPixel(x, y, LED_OFF);
        }
    }
    // matrix.writePixel(7,0, LED_ON);
}

void GOL_8x8::reset()
{
    Serial.println("Reset");
    randomSeed(analogRead(0));
    setGrid(GRID_SELECTION);
    writeGrid();
    return;
}

// Draw to matrix (via grid)

void GOL_8x8::follow(int x, int y)
{
    matrix.drawPixel(x, y, LED_ON);
    matrix.clear();
    matrix.writeDisplay();
}

void GOL_8x8::draw(int x, int y, int v)
{
    grid[x][y] = v;
    writeGrid();
}

void GOL_8x8::blink(int x, int y)
{
    // reverse the state of the pixel where the current cursor is

    currentTimeBlink = millis();
    if (is_blink)
        blinkInterval = BLINK_ON_TIME;
    else
        blinkInterval = BLINK_OFF_TIME;

    if (currentTimeBlink - previousTimeBlink >= blinkInterval)
    {
        // blink it off where it was previously on
        // Serial.println(is_blink);
        previousTimeBlink = currentTimeBlink;
        is_blink = !is_blink;
    }

    if (is_blink)
    {
        // change pixel on the matrix directly - thereby not changing the draw grid (to avoid ghost/shadow writing)
        if (grid[x][y] == 1)
            matrix.drawPixel(x, y, LED_OFF);
        else
            matrix.drawPixel(x, y, LED_ON);
    }
}

void GOL_8x8::read(int &x, int &y, int mode)
{
    /* using address reference as we want to change the value directly

    mode - 0 maps position of joystick directly to x,y
    mode - 1 moves x,y incrementally at predetermined speed ()

    */

    // Serial.print(x); Serial.print("   "); Serial.print(y); Serial.println();

    currentTimeJoy = millis();

    int a1 = 0;
    int a2 = 0;

    // bit of noise on the ADC - using this to average over 10 samples
    for (int s = 0; s < 10; s++)
    {
        a1 = a1 + analogRead(ADC1);
        a2 = a2 + analogRead(ADC2);
    }
    a1 = a1 / 10;
    a2 = a2 / 10;

    if (currentTimeJoy - previousTimeJoy >= joyInterval)
    {
        // blink it off where it was previously on
        // Serial.println(is_blink);
        previousTimeJoy = currentTimeJoy;
        is_move = true;
    }

    /*
    Serial.print(midPointJoyX);
    Serial.print(" ");
    Serial.print(a1);
    Serial.print(" ");
    Serial.print(midPointJoyY);
    Serial.print(" ");
    Serial.print(a2);
    Serial.println();

  */

    switch (mode)
    {

    // map
    case 0:
        x = map(a1, MIN_JX, MAX_JX, 0, MAX_MX - 1);
        y = map(a2, MIN_JY, MAX_JY, 0, MAX_MY - 1);
        break;

    case 1:

        // increment
        // Serial.println(midPointJoyX);

        // if timer allows
        if (is_move)

        {
            int dx = a1 - midPointJoyX;
            int dy = a2 - midPointJoyY;

            if (abs(dx) > abs(dy))
            {
                // X axis is dominant
                if (dx < -JOY_THRESH && x > 0)
                    x = x - 1;
                else if (dx > JOY_THRESH && x < MAX_MX - 1)
                    x = x + 1;
            }
            else
            {

                // Y axis is dominant
                if (dy > JOY_THRESH && y < MAX_MY - 1)
                    y = y + 1;
                else if (dy < -JOY_THRESH && y > 0)
                    y = y - 1;
            }
        }

        // x = 4;
        // y = 4;

        break;

    default:
        x = 4;
        y = 4;
        break;
    }

    x = constrain(x, 0, 7);
    y = constrain(y, 0, 7);
    is_move = false;
}

// AceButton code
//  https://github.com/bxparks/AceButton/blob/develop/examples/SingleButton/SingleButton.ino

void GOL_8x8::handleEvent(ace_button::AceButton * /* button */, uint8_t eventType,
                          uint8_t buttonState)
{

#ifdef DEBUG
    // Print out a message for all events.
    Serial.print(F("handleEvent(): eventType: "));
    Serial.print(ace_button::AceButton::eventName(eventType));
    Serial.print(F("; buttonState: "));
    Serial.println(buttonState);
#endif

    switch (eventType)
    {
    case ace_button::AceButton::kEventClicked:
    case ace_button::AceButton::kEventLongPressed:
        joyButtonState = eventType;
        break;
    case ace_button::AceButton::kEventDoubleClicked:
        joyButtonState = eventType; // only store events you care about
        break;
        // ignore Pressed and Released
    }
}
