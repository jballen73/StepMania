#include "logic.h"
#include "graphics.h"
#include "gba.h"
#include <stdlib.h>
#include "images/sprites.h"
#include "tracks/marioTrack.h"
#include "tracks/metronomeTrack.h"
//extern volatile OamEntry* shadow;
void enqueueArrow(GameArrowQueue *queue, GameArrow *arrow) {
    queue->tail->next = arrow;
    queue->tail = arrow;
    if (!queue->size) {
        queue->head = arrow;
    }
    queue->size++;
}
void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    GameArrowQueue *newAQueue = malloc(sizeof(GameArrowQueue));
    newAQueue->head = NULL;
    newAQueue->tail = NULL;
    newAQueue->size = 0;

    GameArrowQueue *newBQueue = malloc(sizeof(GameArrowQueue));
    newBQueue->head = NULL;
    newBQueue->tail = NULL;
    newBQueue->size = 0;

    GameArrowQueue *newDownQueue = malloc(sizeof(GameArrowQueue));
    newDownQueue->head = NULL;
    newDownQueue->tail = NULL;
    newDownQueue->size = 0;

    GameArrowQueue *newRightQueue = malloc(sizeof(GameArrowQueue));
    newRightQueue->head = NULL;
    newRightQueue->tail = NULL;
    newRightQueue->size = 0;

    GameArrowQueue *undrawnQueue = malloc(sizeof(GameArrowQueue));
    undrawnQueue->head = NULL;
    undrawnQueue->tail = NULL;
    undrawnQueue->size = 0;

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
        enqueueArrow(gaQueue, newArrow);
    }


    appState->gameOver = 0;
    appState->score = 0;
    appState->arrows = gaQueue;
    appState->aQueue = newAQueue;
    appState->bQueue = newBQueue;
    appState->downQueue = newDownQueue;
    appState->rightQueue = newRightQueue;
    appState->toBeUndrawn = undrawnQueue;
    appState->curTime = 0;
    appState->track = &marioTrack;
}



GameArrow* dequeueArrow(GameArrowQueue *queue) {
    GameArrow* arrow = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    if (!queue->size) {
        queue->tail = NULL;
    }
    arrow->next = NULL;
    return arrow;
}
// static GameArrow* removeArrow(GameArrowQueue *queue, int index) {
//     if (index >= queue->size) {return NULL;}
//     if (index == 0) {
//         return dequeueArrow(queue);

//     }
//     GameArrow *arrow = queue->head;
//     GameArrow *prev = queue->head;
//     for (int i = 0; i < index; i++) {
//         prev = arrow;
//         arrow = arrow->next;
//     }
//     prev->next = arrow->next;
//     if (index == queue->size - 1) {
//         queue->tail = prev;
//     }
//     arrow->next = NULL;
//     queue->size--;
//     return arrow;
// }
static void instantiateNewArrow(ArrowType type, GameArrowQueue *arrows, GameArrowQueue *queue) {
    GameArrow *arrow = dequeueArrow(arrows);
    arrow->inUse = 1;
    arrow->type = type;
    arrow->ypos = 128;
    arrow->next = NULL;
    enqueueArrow(queue, arrow);
    
}
static void updateArrows(AppState *appState) {
    GameArrow *cur = appState->aQueue->head;
    while (cur) {
        cur->ypos -= TIMESTEP;
        cur = cur->next;
    }
    cur = appState->bQueue->head;
    while (cur) {
        cur->ypos -= TIMESTEP;
        cur = cur->next;
    }
    cur = appState->downQueue->head;
    while (cur) {
        cur->ypos -= TIMESTEP;
        cur = cur->next;
    }
    cur = appState->rightQueue->head;
    while (cur) {
        cur->ypos -= TIMESTEP;
        cur = cur->next;
    }
    while (appState->aQueue->size && appState->aQueue->head->ypos < 0) {
        cur = dequeueArrow(appState->aQueue);
        cur->inUse = 0;
        enqueueArrow(appState->toBeUndrawn, cur);
        appState->score -= PENALTY;
    }
    while (appState->bQueue->size && appState->bQueue->head->ypos < 0) {
        cur = dequeueArrow(appState->bQueue);
        cur->inUse = 0;
        enqueueArrow(appState->toBeUndrawn, cur);
        appState->score -= PENALTY;
    }
    while (appState->downQueue->size && appState->downQueue->head->ypos < 0) {
        cur = dequeueArrow(appState->downQueue);
        cur->inUse = 0;
        enqueueArrow(appState->toBeUndrawn, cur);
        appState->score -= PENALTY;
    }
    while (appState->rightQueue->size && appState->rightQueue->head->ypos < 0) {
        cur = dequeueArrow(appState->rightQueue);
        cur->inUse = 0;
        enqueueArrow(appState->toBeUndrawn, cur);
        appState->score -= PENALTY;
    }
}
static void parseTrackFrame(const char frame, AppState *appState) {
    if (!frame) {return;}
    if (frame & 8) {
        instantiateNewArrow(A_PRESS, appState->arrows, appState->aQueue);
    }
    if (frame & 4) {
        instantiateNewArrow(B_PRESS, appState->arrows, appState->bQueue);
    }
    if (frame & 2) {
        instantiateNewArrow(DOWN_ARROW, appState->arrows, appState->downQueue);
    }
    if (frame & 1) {
        instantiateNewArrow(RIGHT_ARROW, appState->arrows, appState->rightQueue);
    }
}
static void parseKeyPress(GameArrowQueue *queue, AppState *state) {
    if (!queue->head) {
        //state->score -= PENALTY;
        return;
    }
    if (queue->head->ypos < MAXWINDOW) {
        GameArrow *cur = dequeueArrow(queue);
        cur->inUse = 0;
        enqueueArrow(state->toBeUndrawn, cur);
        int scoreIncrement = MAXWINDOW - cur->ypos;
        if (cur->ypos < 5) {
            scoreIncrement -= (5 - cur->ypos);
        }
        state->score += scoreIncrement;
    } else {
        state->score -= PENALTY;
        return;
    }
}
// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        parseKeyPress(currentAppState->aQueue, &nextAppState);
    }
    if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore)) {
        parseKeyPress(currentAppState->bQueue, &nextAppState);
    }
    if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore)) {
        parseKeyPress(currentAppState->downQueue, &nextAppState);
    }
    if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore)) {
        parseKeyPress(currentAppState->rightQueue, &nextAppState);
    }
    updateArrows(&nextAppState);
    parseTrackFrame((nextAppState.track->track)[currentAppState->curTime], &nextAppState);
    nextAppState.curTime+= 1;
    
    if (nextAppState.curTime > nextAppState.track->length) {
        nextAppState.gameOver = 1;
    }
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