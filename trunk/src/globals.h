#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "object.h"


// Droppable items that the player can pick up to provide 
// temporary effects.
#define HEALTH_POTION           1U
#define INVINCIBILITY_POTION    2U
#define INVISIBILITY_POTION     3U
#define GOLD                    4U
#define BOMB                    5U

// Player states that they can be in. These might need to be in logicmanager.c though...
#define PLAYER_NORMAL			0U
#define PLAYER_ATTACKING		1U
#define PLAYER_HIT				2U
#define PLAYER_DEAD				3U

// Plyaer inventory related defines, a max of 16 in game items.
#define CLOTH_ARMOR       0x0001
#define LEATHER_ARMOR     0x0002
#define CHAIN_ARMOR       0x0004
#define PLATE_ARMOR       0x0008
#define DAGGER            0x0010
#define STEEL_SWORD       0x0020
#define LONGSWORD         0x0040
#define LIGHTSWORD        0x0080
//#define XXX               0x0100
//#define XXX               0x0200
//#define XXX               0x0400
//#define XXX               0x0800

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

// Holds the X/Y coordinates of a player, enemy, or other object that
// need's pixel perfect accuracy
typedef struct
{
    unsigned char ucBigX;
    unsigned char ucBigY;
    signed char scSmallX;
    signed char scSmallY;
} COORDINATE;

// Functions that are used universally
unsigned char   GLB_RandomNum(unsigned char ucMin, unsigned char ucMax);
COORDINATE      GLB_MoveCoordinate(COORDINATE coord, signed char scX, signed char scY);
unsigned char   GLB_CoordinateToObjectCollision(COORDINATE coord, MapObject mapobj);
void            GLB_PrintString(unsigned char x, unsigned char y, const char *string);
void            GLB_PrintNumber(unsigned char x, unsigned char y, unsigned char ucNum);

#endif
