#include "logic.h"
#include "graphics.h"
#include "gba.h"
#include <stdlib.h>
#include "images/sprites.h"
#include "tracks/testTrack.h"
//extern volatile OamEntry* shadow;
void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    GameArrowQueue *gaQueue = malloc(sizeof(GameArrowQueue));
    gaQueue->head = NULL;
    gaQueue->tail = NULL;
    gaQueue->size = 0;

    GameArrowQueue *undrawnQueue = malloc(sizeof(GameArrowQueue));
    undrawnQueue->head = NULL;
    undrawnQueue->tail = NULL;
    undrawnQueue->size = 0;

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
    appState->toBeUndrawn = undrawnQueue;
    appState->curTime = 0;
    appState->track = &testTrack;
}


static void enqueueArrow(GameArrowQueue *queue, GameArrow *arrow) {
    queue->tail->next = arrow;
    queue->tail = arrow;
    if (!queue->size) {
        queue->head = arrow;
    }
    queue->size++;
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
static GameArrow* removeArrow(GameArrowQueue *queue, int index) {
    if (index >= queue->size) {return NULL;}
    if (index == 0) {
        return dequeueArrow(queue);

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
    arrow->next = NULL;
    queue->size--;
    return arrow;
}
static void instantiateNewArrow(ArrowType type, GameArrow *arrows[], GameArrowQueue *queue) {
    for (int i = 0; i < 128; i++) {
        int index = (queue->tail->id + i) % 128;
        if (!arrows[index]->inUse) {
            arrows[index]->inUse = 1;
            arrows[index]->type = type;
            arrows[index]->ypos = 128;
            arrows[index]->next = NULL;
            enqueueArrow(queue, arrows[index]);
            return;
        }
    }
}
static void updateArrows(AppState *appState) {
    GameArrow *cur = appState->arrowQueue->head;
    for (int i = 0; i < appState->arrowQueue->size; i++) {
        cur->ypos-= 3;
        if (cur->ypos < 0) {
            appState->score -= MAXWINDOW;
            cur->inUse = 0;
            //dequeueArrow(appState->arrowQueue);
            
            enqueueArrow(appState->toBeUndrawn, removeArrow(appState->arrowQueue, i));
        }
        cur = cur->next;
    }
}
static void parseTrackFrame(const char frame, AppState *appState) {
    if (!frame) {return;}
    if (frame & 8) {
        instantiateNewArrow(A_PRESS, appState->arrows, appState->arrowQueue);
    }
    if (frame & 4) {
        instantiateNewArrow(B_PRESS, appState->arrows, appState->arrowQueue);
    }
    if (frame & 2) {
        instantiateNewArrow(DOWN_ARROW, appState->arrows, appState->arrowQueue);
    }
    if (frame & 1) {
        instantiateNewArrow(RIGHT_ARROW, appState->arrows, appState->arrowQueue);
    }
}
static void parseKeyPress(ArrowType pressedKey, AppState *state) {
    GameArrow *cur = state->arrowQueue->head;
    if (!cur) {return;}
    for (int i = 0; i < 5; i++) {
        if (!cur) {
            state->score -= MAXWINDOW;
            return;
        }
        if (cur->type == pressedKey) {
            if (cur->ypos < MAXWINDOW) {
                cur->inUse = 0;
                removeArrow(state->arrowQueue, i);
                enqueueArrow(state->toBeUndrawn, cur);
                state->score += (MAXWINDOW - cur->ypos);
                return;
            } else {
                break;
            }
        }
        cur = cur->next;
    }
    if (state->arrowQueue->head->ypos < 5) {
        enqueueArrow(state->toBeUndrawn, dequeueArrow(state->arrowQueue));
    }
    state->score -= MAXWINDOW;
}
// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        parseKeyPress(A_PRESS, &nextAppState);
    }
    if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore)) {
        parseKeyPress(B_PRESS, &nextAppState);
    }
    if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore)) {
        parseKeyPress(DOWN_ARROW, &nextAppState);
    }
    if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore)) {
        parseKeyPress(RIGHT_ARROW, &nextAppState);
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