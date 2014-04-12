//Keystatus.h
#ifndef KEYSTATUS_H
#define KEYSTATUS_H

#include <windows.h>

//function prototypes
bool keyDown(int key);

//constants for game state transition events
const int GO_NO_WHERE = -1;
const int GO_INTRO = 0;
const int GO_MAIN = 1;
const int GO_HELP = 2;
const int GO_SCORE = 3;
const int GO_PAUSE = 4;
const int GO_PLAY = 5;
const int GO_QUIT = 100;

#endif
