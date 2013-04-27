#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// Item related defines, a total of 16 in game items.
#define HEALTH_POTION     0x0001
#define HASTE_POTION      0x0002
#define INVINCIBLE_POTION 0x0004
#define STRENGTH_POTION   0x0008
#define LONGSWORD         0x0010
#define LIGHTSWORD        0x0020
#define CLOTH_ARMOR       0x0040
#define LEATHER_ARMOR     0x0080
#define GOLD              0x0100
#define KEY               0x0200

// Defines for our tile types.
#define EMPTY	    0U
#define FLOOR1      1U
#define FLOOR2      2U
#define FLOOR3      3U
#define DOOR	    4U
#define STAIRS      5U
#define TABLE       6U
#define SHELF       7U
#define BARREL      8U
#define WALL_UP     9U
#define WALL_DOWN   10U
#define WALL_LEFT   11U
#define WALL_TOP    12U
#define WALL_MIDDLE 13U
#define WALL_RIGHT  14U
#define WALL_SINGLE 15U

#define HUD_CORNER  40U
#define HUD_HORIZ   41U
#define HUD_VERT    42U

// Collision detection defines
#define TILE_SIZE           8

// Video related defines
#define SCREEN_W        30U
#define SCREEN_H        28U
#define HUD_H           6U
#define MAP_X_OFFSET    3U
#define MAP_Y_OFFSET    7U

// Directional defines
#define UP		0U
#define DOWN	1U
#define LEFT	2U
#define RIGHT	3U
#define NO_DIR  4U

// Functions that are used universally
unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax);


// Holds the X/Y coordinates of a player, enemy, or other object that
// need's pixel perfect accuracy
typedef struct
{
    unsigned char ucBigX;
    unsigned char ucBigY;
    signed char scSmallX;
    signed char scSmallY;
} COORDINATE;

#endif
