/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites --bpp=4 --for_bitmap sprites playerCharacterSprite.png playerShot.png deathCountText.png checkpointText.png savedText.png sonicSprites.gif --transparent=FFFFFF 
 * Time-stamp: Thursday 11/15/2018, 11:05:46
 * 
 * Image Information
 * -----------------
 * playerCharacterSprite.png 16@16
 * playerShot.png 8@8
 * deathCountText.png 32@8
 * checkpointText.png 32@8
 * savedText.png 32@8
 * sonicSprites.gif (frame 0) 32@64
 * sonicSprites.gif (frame 1) 32@64
 * Transparent color: (255, 255, 255)
 * 
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITES_H
#define SPRITES_H

#define SPRITES_PALETTE_TYPE (0 << 13)
#define SPRITES_DIMENSION_TYPE (1 << 6)

extern const unsigned short sprites_palette[256];
#define SPRITES_PALETTE_SIZE 512
#define SPRITES_PALETTE_LENGTH 256

extern const unsigned short sprites[1296];
#define SPRITES_SIZE 2592
#define SPRITES_LENGTH 1296

#define SONICSPRITES1_PALETTE_ID (0 << 12)
#define SONICSPRITES1_ID 512

#define PLAYERCHARACTERSPRITE_PALETTE_ID (1 << 12)
#define PLAYERCHARACTERSPRITE_SPRITE_SHAPE (0 << 14)
#define PLAYERCHARACTERSPRITE_SPRITE_SIZE (1 << 14)
#define PLAYERCHARACTERSPRITE_ID 544

#define SONICSPRITES0_PALETTE_ID (2 << 12)
#define SONICSPRITES0_ID 548

#define PLAYERSHOT_PALETTE_ID (1 << 12)
#define PLAYERSHOT_SPRITE_SHAPE (0 << 14)
#define PLAYERSHOT_SPRITE_SIZE (0 << 14)
#define PLAYERSHOT_ID 580

#define DEATHCOUNTTEXT_PALETTE_ID (1 << 12)
#define DEATHCOUNTTEXT_SPRITE_SHAPE (1 << 14)
#define DEATHCOUNTTEXT_SPRITE_SIZE (1 << 14)
#define DEATHCOUNTTEXT_ID 581

#define CHECKPOINTTEXT_PALETTE_ID (1 << 12)
#define CHECKPOINTTEXT_SPRITE_SHAPE (1 << 14)
#define CHECKPOINTTEXT_SPRITE_SIZE (1 << 14)
#define CHECKPOINTTEXT_ID 585

#define SAVEDTEXT_PALETTE_ID (1 << 12)
#define SAVEDTEXT_SPRITE_SHAPE (1 << 14)
#define SAVEDTEXT_SPRITE_SIZE (1 << 14)
#define SAVEDTEXT_ID 589

extern const unsigned short sonicSprites_frames[2];
#define SONICSPRITES_FRAMES 2

#define SONICSPRITES_SPRITE_SHAPE (2 << 14)
#define SONICSPRITES_SPRITE_SIZE (3 << 14)

#endif

