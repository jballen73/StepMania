/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites --bpp=4 --for_bitmap --transparent=FFFFFF sprites AButton.png BButton.png DownArrow.png RightArrow.png line.png 
 * Time-stamp: Saturday 03/02/2019, 07:15:42
 * 
 * Image Information
 * -----------------
 * AButton.png 32@32
 * BButton.png 32@32
 * DownArrow.png 32@32
 * RightArrow.png 32@32
 * line.png 64@32
 * Transparent color: (255, 255, 255)
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

extern const unsigned short sprites[1536];
#define SPRITES_SIZE 3072
#define SPRITES_LENGTH 1536

#define BBUTTON_PALETTE_ID (0 << 12)
#define BBUTTON_SPRITE_SHAPE (0 << 14)
#define BBUTTON_SPRITE_SIZE (2 << 14)
#define BBUTTON_ID 512

#define DOWNARROW_PALETTE_ID (1 << 12)
#define DOWNARROW_SPRITE_SHAPE (0 << 14)
#define DOWNARROW_SPRITE_SIZE (2 << 14)
#define DOWNARROW_ID 528

#define RIGHTARROW_PALETTE_ID (1 << 12)
#define RIGHTARROW_SPRITE_SHAPE (0 << 14)
#define RIGHTARROW_SPRITE_SIZE (2 << 14)
#define RIGHTARROW_ID 544

#define ABUTTON_PALETTE_ID (1 << 12)
#define ABUTTON_SPRITE_SHAPE (0 << 14)
#define ABUTTON_SPRITE_SIZE (2 << 14)
#define ABUTTON_ID 560

#define LINE_PALETTE_ID (1 << 12)
#define LINE_SPRITE_SHAPE (1 << 14)
#define LINE_SPRITE_SIZE (3 << 14)
#define LINE_ID 576

#endif

