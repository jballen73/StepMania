#include "logic.h"
#include "graphics.h"
#include "gba.h"
#include <stdlib.h>
#include "sprites.h"
//extern volatile OamEntry* shadow;
void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    GameArrowQueue *gaQueue = malloc(sizeof(GameArrowQueue));
    gaQueue->head = NULL;
    gaQueue->tail = NULL;
    gaQueue->size = 0;

    for (int i = 0; i < 128; i++) {
        GameArrow *newArrow = malloc(sizeof(GameArrow));
        newArrow->inUse = 0;
        newArrow->id = i;
        newArrow->next = NULL;
        newArrow->type = A_PRESS;
        newArrow->ypos = 128;
        appState->arrows[i] = newArrow;
    }


    appState->gameOver = 0;
    appState->score = 0;
    appState->arrowQueue = gaQueue;
}


static void enqueueArrow(GameArrowQueue *queue, GameArrow *arrow) {
    queue->tail->next = arrow;
    queue->tail = arrow;
    if (!queue->size) {
        queue->head = arrow;
    }
    queue->size++;
}
static GameArrow* dequeueArrow(GameArrowQueue *queue) {
    GameArrow* arrow = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    if (!queue->size) {
        queue->tail = NULL;
    }
    return arrow;
}
static void removeArrow(GameArrowQueue *queue, int index) {
    if (index >= queue->size) {return;}
    if (index == 0) {
        dequeueArrow(queue);
        return;
    }
    GameArrow *arrow = queue->head;
    GameArrow *prev = queue->head;
    for (int i = 0; i < index; i++) {
        prev = arrow;
        arrow = arrow->next;
    }
    prev->next = arrow->next;
    if (index == queue->size - 1) {
        queue->tail = prev;
    }
    queue->size--;
}
static void instantiateNewArrow(ArrowType type, GameArrow **arrows, GameArrowQueue *queue) {
    for (int i = 0; i < 128; i++) {
        if (!arrows[i]->inUse) {
            arrows[i]->inUse = 1;
            arrows[i]->type = type;
            arrows[i]->ypos = 128;
            enqueueArrow(queue, arrows[i]);
            return;
        }
    }
}
static void updateArrows(AppState *appState) {
    GameArrow *cur = appState->arrowQueue->head;
    for (int i = 0; i < appState->arrowQueue->size; i++) {
        cur->ypos--;
        if (cur->ypos < 0) {
            appState->score -= 5;
            removeArrow(appState->arrowQueue, i);
        }
    }
}
static void parseTrackFrame(int *frame, AppState *appState) {
    if (frame[0]) {
        instantiateNewArrow(A_PRESS, appState->arrows, appState->arrowQueue);
    }
    if (frame[1]) {
        instantiateNewArrow(B_PRESS, appState->arrows, appState->arrowQueue);
    }
    if (frame[2]) {
        instantiateNewArrow(DOWN_ARROW, appState->arrows, appState->arrowQueue);
    }
    if (frame[3]) {
        instantiateNewArrow(RIGHT_ARROW, appState->arrows, appState->arrowQueue);
    }
}
// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    
    return nextAppState;
}


void createData(ArrowData *data, ArrowType type) {
    switch(type) {
        case A_PRESS:
            data->baseAttr0 = SPRITES_PALETTE_TYPE | ABUTTON_SPRITE_SHAPE;
            data->baseAttr1 = POS1 | ABUTTON_SPRITE_SIZE;
            data->baseAttr2 = ABUTTON_PALETTE_ID | ABUTTON_ID;
            break;
        case B_PRESS:
            data->baseAttr0 = SPRITES_PALETTE_TYPE | BBUTTON_SPRITE_SHAPE;
            data->baseAttr1 = POS2 | BBUTTON_SPRITE_SIZE;
            data->baseAttr2 = BBUTTON_PALETTE_ID | BBUTTON_ID;
            break;
        case DOWN_ARROW:
            data->baseAttr0 = SPRITES_PALETTE_TYPE | DOWNARROW_SPRITE_SHAPE;
            data->baseAttr1 = POS3 | DOWNARROW_SPRITE_SIZE;
            data->baseAttr2 = DOWNARROW_PALETTE_ID | DOWNARROW_ID;
            break;
        case RIGHT_ARROW:
            data->baseAttr0 = SPRITES_PALETTE_TYPE | RIGHTARROW_SPRITE_SHAPE;
            data->baseAttr1 = POS4 | RIGHTARROW_SPRITE_SIZE;
            data->baseAttr2 = RIGHTARROW_PALETTE_ID | RIGHTARROW_ID;
            break;
    }
}