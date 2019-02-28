#include "graphics.h"
#include "gba.h"
#include "images/sprites.h"
#include "images/Background.h"
#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
//#include "images/garbage.h"
static void drawSprites(void);
volatile OamEntry shadow[128];
//volatile OamEntry* playerCharacter = &shadow[0];

void hideSprites(void) {
    for(int i = 0; i < 128; i++) {
        shadow[i].attr0 = ATTR0_HIDE;
    }
    drawSprites();
}
void graphicsInit(void) {
    DMA[3].src = sprites_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = SPRITES_PALETTE_LENGTH | DMA_ON;

    DMA[3].src = sprites;
    DMA[3].dst = &CHARBLOCKBASE[5];
    DMA[3].cnt = SPRITES_LENGTH | DMA_ON;


    for(int i = 0; i < 128; i++) {
        shadow[i].attr0 = ATTR0_HIDE;
    }

    //playerCharacter = &shadow[0];
    // playerCharacter->attr0 = 50 | SPRITES_PALETTE_TYPE | PLAYERCHARACTERSPRITE_SPRITE_SHAPE;
    // playerCharacter->attr1 = 50 | PLAYERCHARACTERSPRITE_SPRITE_SIZE;
    //playerCharacter->attr2 = PLAYERCHARACTERSPRITE_PALETTE_ID | PLAYERCHARACTERSPRITE_ID;

    
}
// TA-TODO: Add any draw/undraw functions for sub-elements of your app here.
// For example, for a snake game, you could have a drawSnake function
// or a drawFood function
//
// e.g.:
// static void drawSnake(Snake* snake);
// static void drawFood(Food* food);

/*
static void drawShot0(int xpos, int ypos, int direction) {
    playerShot0->attr0 = (ypos&0x00FF) | SPRITES_PALETTE_TYPE | PLAYERSHOT_SPRITE_SHAPE;
    if (direction){
        playerShot0->attr1 = (xpos&0x00FF) | PLAYERSHOT_SPRITE_SIZE; 
    } else {
        playerShot0->attr1 = (xpos&0x00FF) | PLAYERSHOT_SPRITE_SIZE | ATTR1_HFLIP; 
    }
}
*/
static void drawArrow(GameArrow *arrow) {
    ArrowData data;
    createData(&data, arrow->type);
    shadow[arrow->id].attr0 = arrow->ypos | data.baseAttr0;
    shadow[arrow->id].attr1 = data.baseAttr1;
    shadow[arrow->id].attr2 = data.baseAttr2;
}
static void hideArrow(GameArrow *arrow) {
    shadow[arrow->id].attr0 = ATTR0_HIDE;
}
static void drawScore(int score) {
    char scoreText[7];
    sprintf(scoreText, "%d", score);
    drawString(218, 3, scoreText, BLACK);
}
static void drawSprites(void) {
    DMA[3].src = shadow;
    DMA[3].dst = OAMMEM;
    DMA[3].cnt = 128*4 | DMA_ON;
}
// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    UNUSED(state);
    drawFullScreenImageDMA(Background);
    drawSprites();
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    UNUSED(state);
    
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    //if (state->levelChange > 0) {

       // state->levelChange--;
    //}
    GameArrow *cur = state->aQueue->head;
    while (cur) {
        drawArrow(cur);
        cur = cur->next;
    }
    cur = state->bQueue->head;
    while (cur) {
        drawArrow(cur);
        cur = cur->next;
    }
    cur = state->downQueue->head;
    while (cur) {
        drawArrow(cur);
        cur = cur->next;
    }
    cur = state->rightQueue->head;
    while (cur) {
        drawArrow(cur);
        cur = cur->next;
    }
    while (state->toBeUndrawn->size) {
        cur = dequeueArrow(state->toBeUndrawn);
        hideArrow(cur);
        enqueueArrow(state->arrows, cur);
    }
    drawRectDMA(218, 3, 20, 10, WHITE);
    drawScore(state->score);
    drawSprites();
}

