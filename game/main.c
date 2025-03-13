#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"

#include "images/start.h"
#include "images/maze.h"
#include "images/gameOver.h"
#include "images/win.h"
#include "images/sprite.h"



int main(void) {
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    // Save current and previous state of button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    // Game state
    struct state myState;
    myState.currState = START;
    myState.prevState = WIN;

    // Player state
    struct player ps;
    struct player *sp = &ps;
    sp->cheat = 1;
    sp->cheatStr = "Off";
    sp->speed = 1;
    sp->speedStr = "Normal";
    sp->size = 2;
    sp->sizeStr = "2";
    sp->color = 0;
    sp->colorStr = "Red";
    sp->menuPosPrev = 0;
    sp->menuPos = 0;
    
    // Start page sprite
    struct sprite sprite1;
    sprite1.col = 0;
    sprite1.cd = 1;
    sprite1.row = 120;

    while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    switch (myState.currState) {
        case START:
            // Switch state on start press
            if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                myState.currState = INIT_PLAY;
                break;
            } else if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
                myState.currState = MENU;
                break;
            }

            // Animate sprite
            if (sprite1.col + SPRITE_WIDTH == WIDTH) {
                sprite1.col = 0;
            }
            sprite1.col+= sprite1.cd;

            break;
        case INIT_PLAY:
            // Load initial player state for maze depending on difficulty.
            switch (sp->speed) {
                case 0:
                    sp->player.delay = 4;
                    sp->player.rd = 1;
                    sp->player.cd = 1;
                    break;
                case 1:
                    sp->player.delay = 2;
                    sp->player.rd = 1;
                    sp->player.cd = 1;
                    break;
                case 2:
                    sp->player.delay = 0;
                    sp->player.rd = 1;
                    sp->player.cd = 1;
                    break;
                case 3:
                    sp->player.delay = 0;
                    sp->player.rd = 2;
                    sp->player.cd = 2;
                    break;
            }
            switch (sp->color) {
                case 0:
                    sp->player.color = RED;
                    break;
                case 1:
                    sp->player.color = GREEN;
                    break;
                case 2:
                    sp->player.color = BLUE;
                    break;
            }
            sp->player.size = sp->size;

            sp->player.row = 10;
            sp->player.col = 10;
            sp->player.rDelay = 0;
            sp->player.cDelay = 0;
            sp->player.prevRow = 10;
            sp->player.prevCol = 10;
            sp->timeSec = 0;
            sp->gameTime = vBlankCounter;

            // Change state
            myState.currState = PLAY;
            break;
        case PLAY:
            // End game in 100 sec
            if (sp->timeSec >= 100) {
                myState.currState = GAME_OVER;
                break;
            }

            // Back to start on press, otherwise move player
            if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
                myState.currState = START;
                break;
            }

            // Collision detection
            // Loop through pixels of player and check if the corresponding maze is black
            for (int r = sp->player.row; r < sp->player.row + sp->player.size; r++) {
                for (int c = sp->player.col; c < sp->player.col + sp->player.size; c++) {
                    if (sp->cheat && maze[OFFSET(r, c, WIDTH)] == BLACK) {
                        myState.currState = GAME_OVER;
                        break;
                    } else if (maze[OFFSET(r, c, WIDTH)] == YELLOW) {
                        myState.currState = WIN;
                        break;
                    }
                }
            }

            /* Movement for player. Delays movement if at minimum speed.
             * Changes speed based on if boost or slow is pressed.
             */
            sp->player.prevRow = sp->player.row;
            sp->player.prevCol = sp->player.col;
            sp->player.rdA = sp->player.rd;
            sp->player.cdA = sp->player.cd;
            sp->player.dA = sp->player.delay;

            if (KEY_DOWN(BUTTON_A, currentButtons)) {
                if (sp->player.delay > 1) {
                    sp->player.dA = sp->player.dA / 2 - 1;
                } else {
                    sp->player.rdA++;
                    sp->player.cdA++;
                }

            } else if (KEY_DOWN(BUTTON_B, currentButtons)) {
                if (sp->player.rd > 1) {
                    sp->player.rdA--;
                    sp->player.cdA--;
                } else {
                    sp->player.dA = (sp->player.dA + 1) * 2;
                }
            }
            if (KEY_DOWN(BUTTON_UP, currentButtons)) {
                sp->player.rDelay--;
                if (abs(sp->player.rDelay) > sp->player.dA) {
                    sp->player.row -= sp->player.rdA;
                    sp->player.rDelay = 0;
                }
            }
            if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
                sp->player.rDelay++;
                if (abs(sp->player.rDelay) > sp->player.dA) {
                    sp->player.row += sp->player.rdA;
                    sp->player.rDelay = 0;
                }
            }
            if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
                sp->player.cDelay--;
                if (abs(sp->player.cDelay) > sp->player.dA) {
                    sp->player.col -= sp->player.cdA;
                    sp->player.cDelay = 0;
                }
            }
            if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
                sp->player.cDelay++;
                if (abs(sp->player.cDelay) > sp->player.dA) {
                    sp->player.col += sp->player.cdA;
                    sp->player.cDelay = 0;
                }
            }

            // Timer
            if (vBlankCounter - sp->gameTime >= 6) {
                sp->timeMSec++;
                sp->gameTime = vBlankCounter;
            }
            if (sp->timeMSec >= 10) {
                sp->timeMSec -= 10;
                sp->timeSec++;
            }
            snprintf(sp->timeDisp, 10, "%d.%d", sp->timeSec, sp->timeMSec);

            break;

        case GAME_OVER:
            if (PRESSED_SELECT_START) {
                myState.currState = START;
            }
            break;
        case WIN:
            if (PRESSED_SELECT_START) {
                myState.currState = START;
            }
            break;
        case MENU:

            if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons) && sp->menuPos > 0) {
                sp->menuPos--;
            } else if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons) && sp->menuPos < 3) {
                sp->menuPos++;
            }

            if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
                switch (sp->menuPos) {
                    case 0:
                        if (sp->size < 5) {
                            sp->size++;
                        }
                        break;
                    case 1:
                        if (sp->speed < 3) {
                            sp->speed++;
                        }
                        break;
                    case 2:
                        if (sp->color < 2) {
                            sp->color++;
                        }
                        break;
                    case 3:
                        sp->cheat = !sp->cheat;
                        break;
                }
            } else if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
                switch (sp->menuPos) {
                    case 0:
                        if (sp->size > 1) {
                            sp->size--;
                        }
                        break;
                    case 1:
                        if (sp->speed > 0) {
                            sp->speed--;
                        }
                        break;
                    case 2:
                        if (sp->color > 0) {
                            sp->color--;
                        }
                        break;
                    case 3:
                        sp->cheat = !sp->cheat;
                        break;
                }
            }

            switch (sp->speed) {
                case 0:
                    sp->speedStr = "Slow";
                    break;
                case 1:
                    sp->speedStr = "Normal";
                    break;
                case 2:
                    sp->speedStr = "Fast";
                    break;
                case 3:
                    sp->speedStr = "Insane";
                    break;
            }
            switch (sp->color) {
                case 0:
                    sp->colorStr = "Red";
                    break;
                case 1:
                    sp->colorStr = "Green";
                    break;
                case 2:
                    sp->colorStr = "Blue";
                    break;
            }

            if (sp->cheat) {
                sp->cheatStr = "Off";
            } else {
                sp->cheatStr = "On";
            }

            switch (sp->size) {
                case 1:
                    sp->sizeStr = "1";
                    break;
                case 2:
                    sp->sizeStr = "2";
                    break;
                case 3:
                    sp->sizeStr = "3";
                    break;
                case 4:
                    sp->sizeStr = "4";
                    break;
                case 5:
                    sp->sizeStr = "5";
                    break;
            }


            if (PRESSED_SELECT_START) {
                myState.currState = START;
            }
            break;
    }

    waitForVBlank();

    switch (myState.currState) {
        case START:
            if (myState.prevState != START) {
                drawFullScreenImageDMA(start);
                drawString(10, 50, "Press A for Settings", BLACK);
            }
            if (sprite1.col == 1) {
                undrawImageDMA(sprite1.row, WIDTH - SPRITE_WIDTH, SPRITE_WIDTH, SPRITE_HEIGHT, start);
            }
            drawImageDMA(sprite1.row, sprite1.col, SPRITE_WIDTH, SPRITE_HEIGHT, sprite);
            break;
        case INIT_PLAY:
            drawFullScreenImageDMA(maze);
            drawString(146, 3, "Time:", BLACK);
            break;
        case PLAY:
            undrawImageDMA(146, 33, 30, 10, maze);
            drawString(146, 33, sp->timeDisp, BLACK);
            undrawImageDMA(sp->player.prevRow, sp->player.prevCol, sp->player.size, sp->player.size, maze);
            drawRectDMA(sp->player.row, sp->player.col, sp->player.size, sp->player.size, sp->player.color);
            break;
        case GAME_OVER:
            if (myState.prevState != GAME_OVER) {
                drawFullScreenImageDMA(gameOver);
                drawString(150, 20, "Press start or select to try again", BLACK);
            }
            break;
        case WIN:
            if (myState.prevState != WIN) {
                drawFullScreenImageDMA(win);
                drawString(146, 3, "Time:", BLACK);
                drawString(146, 33, sp->timeDisp, BLACK);
            }
            break;
        case MENU:
            if (myState.prevState != MENU) {
                fillScreenDMA(WHITE);
                drawString(20, 80, "Settings", BLACK);
                drawString(40, 50, "Size: ", BLACK);
                drawString(60, 50, "Speed: ", BLACK);
                drawString(80, 50, "Color: ", BLACK);
                drawString(100, 50, "Cheats: ", BLACK);
                drawString(40, 100, sp->sizeStr, BLACK);
                drawString(60, 100, sp->speedStr, BLACK);
                drawString(80, 100, sp->colorStr, BLACK);
                drawString(100, 100, sp->cheatStr, BLACK);
            } else {
                if (sp->menuPosPrev != sp->menuPos) {
                    switch (sp->menuPosPrev) {
                        case 0:
                            drawRectDMA(40, 100, 40, 10, WHITE);
                            drawString(40, 100, sp->sizeStr, BLACK);
                            break;
                        case 1:
                            drawRectDMA(60, 100, 40, 10, WHITE);
                            drawString(60, 100, sp->speedStr, BLACK);
                            break;
                        case 2:
                            drawRectDMA(80, 100, 40, 10, WHITE);
                            drawString(80, 100, sp->colorStr, BLACK);
                            break;
                        case 3:
                            drawRectDMA(100, 100, 40, 10, WHITE);
                            drawString(100, 100, sp->cheatStr, BLACK);
                            break;
                    }
                }
            }



            switch (sp->menuPos) {
                case 0:
                    drawRectDMA(40, 100, 40, 10, WHITE);
                    drawString(40, 100, sp->sizeStr, RED);
                    break;
                case 1:
                    drawRectDMA(60, 100, 40, 10, WHITE);
                    drawString(60, 100, sp->speedStr, RED);
                    break;
                case 2:
                    drawRectDMA(80, 100, 40, 10, WHITE);
                    drawString(80, 100, sp->colorStr, RED);
                    break;
                case 3:
                    drawRectDMA(100, 100, 40, 10, WHITE);
                    drawString(100, 100, sp->cheatStr, RED);
                    break;
            }
            sp->menuPosPrev = sp->menuPos;
            break;

    }

    previousButtons = currentButtons; // Store the current state of the buttons
    myState.prevState = myState.currState;

    }

    return 0;
}
