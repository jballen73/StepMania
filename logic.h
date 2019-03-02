#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"



#define POS1 (53)
#define POS2 (87)
#define POS3 (121)
#define POS4 (155)
#define MAXWINDOW (18)
#define PENALTY (10)
#define TIMESTEP (3)
#define NOTEPROB (16)
typedef struct GameArrow GameArrow;
typedef struct Line Line;
typedef enum {
    RIGHT_ARROW,
    DOWN_ARROW,
    A_PRESS,
    B_PRESS,
} ArrowType;
struct GameArrow {
    int inUse;
    int id;
    ArrowType type;
    int ypos;
    GameArrow *next;
};
struct Line {
    int xpos;
    int ypos;
    int id;
    Line *next;
};
typedef struct{
    GameArrow *head;
    GameArrow *tail;
    int size;
} GameArrowQueue;
typedef struct {
    Line *head;
    Line *tail;
    int size;
} LineQueue;
typedef struct {
    int baseAttr0;
    int baseAttr1;
    int baseAttr2;
} ArrowData;
typedef struct {
    const char track[8192];
    int length;
} Track;
typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    int score;
    int useTrack;
    int fpn;
    int len;
    GameArrowQueue *arrows;
    GameArrowQueue *aQueue;
    GameArrowQueue *bQueue;
    GameArrowQueue *downQueue;
    GameArrowQueue *rightQueue;
    GameArrowQueue *toBeUndrawn;
    LineQueue *lines;
    LineQueue *inUseLines;
    LineQueue *toBeUndrawnLines;
    Track *track;
    int curTime;
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
GameArrow* dequeueArrow(GameArrowQueue *queue);
void enqueueArrow(GameArrowQueue *queue, GameArrow *arrow);
void enqueueLine(LineQueue *queue, Line *line);
Line* dequeueLine(LineQueue *queue);
#endif
