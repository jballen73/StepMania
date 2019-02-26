#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"



#define POS1 (53)
#define POS2 (87)
#define POS3 (121)
#define POS4 (155)


typedef enum {
    RIGHT_ARROW,
    DOWN_ARROW,
    A_PRESS,
    B_PRESS,
} ArrowType;
typedef struct {
    int inUse;
    int id;
    ArrowType type;
    int ypos;
    GameArrow *next;
} GameArrow;
typedef struct{
    GameArrow *head;
    GameArrow *tail;
    int size;
} GameArrowQueue;

typedef struct {
    int baseAttr0;
    int baseAttr1;
    int baseAttr2;
} ArrowData;
typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    int score;
    GameArrow *arrows[128];
    GameArrowQueue *arrowQueue;
} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/
// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);
// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.
void createData(ArrowData *data, ArrowType type);
#endif
