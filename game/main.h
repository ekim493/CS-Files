#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

#define PRESSED_SELECT_START (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons) || KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons))
#define UP (1<<0)
#define DOWN (1<<1)
#define LEFT (1<<2)
#define RIGHT (1<<3)

enum gba_state {
    START,
    INIT_PLAY,
    PLAY,
    GAME_OVER,
    WIN,
    MENU,

};

struct state {
    enum gba_state currState;
    enum gba_state prevState;
};

/**
 * A struct rectangle to hold player data and movement.
 *
 * row, col represent the current top-left location of the player
 * size represents the size of the rectangle, set by difficulty
 * color represents the color of the rectangle. Default red.
 * rd, cd, delay represent the change in row, col, and delay set by the difficulty
 * rDelay, cDelay keeps track of the number of delays in each direction
 * rdA, cdA, dA represent actual change in row, col, and delay if boost or slow is applied
 * prevRow, prevCol represent the previous location of the player for display
 */
struct rect {
    int row;
    int col;
    int size;
    int color;
    int rd;
    int cd;
    int rDelay;
    int cDelay;
    int delay;
    int rdA;
    int cdA;
    int dA;
    int prevRow;
    int prevCol;
};

/**
 * A struct player to hold the player's data.
 *
 * timeDisp allocates space for the string representation of time. Updated in game.
 * timeSec keeps track of game time in sec, timeMSec keeps track of ms portion of time
 * gameTime keeps track of # of frames.
 * size and sizeStr represent the size of the player
 * speed and speedStr represent the player's speed
 * color and colorStr represent the player's color
 * cheat and cheatStr represent whether the player has cheats on or not
 * rect player hols the current player struct.
 * menuPos and menuPosPrev help the player navigate the menu
 */
struct player {
    char timeDisp[10];
    int timeSec;
    int timeMSec;
    u32 gameTime;
    int size;
    char *sizeStr;
    int speed;
    char *speedStr;
    int color;
    char *colorStr;
    int cheat;
    char *cheatStr;
    struct rect player;
    int menuPos;
    int menuPosPrev;
};

struct sprite {
    int col;
    int row;
    int cd;
};


#endif
