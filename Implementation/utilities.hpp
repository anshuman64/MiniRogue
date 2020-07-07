// utilities.h

// Note:  YOU MUST NOT MAKE ANY CHANGE TO THIS FILE!

#ifndef utilities_hpp
#define utilities_hpp

#include <stdio.h>
#include <list>

using namespace std;


const list<char> SYMBOLS = {'B','D','G','S','@','#','>','&',')','?'};

const int NUM_ROWS = 18;
const int NUM_COLS = 70;

const int MAX_STATS = 99;
const int MAX_LEVEL = 4;

const char ARROW_LEFT  = 'a';
const char ARROW_RIGHT = 'd';
const char ARROW_UP    = 'w';
const char ARROW_DOWN  = 's';

char getCharacter();
void clearScreen();
int randInt(int limit);             // random int from 0 to limit-1
bool trueWithProbability(double p); // return true with probability p


#endif /* utilities_hpp */
