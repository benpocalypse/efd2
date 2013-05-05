#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "globals.h"

// All of the data related to the player is stored  here.
typedef struct
{
    unsigned char ucHealth;
    unsigned char ucTotalHealth;
    unsigned char ucGold;
    unsigned int  uiInventory; // BITFIELD...MASKABLE, ORABLE(16 items total)
    unsigned char ucScreensPassed;
    unsigned char ucVelocityAndDirection;
    COORDINATE    objLocation;
} PLAYER;

// Function prototypes that allow outside classes to get access
// to the Player classes data.
unsigned char PLY_Init(void);
unsigned char PLY_IsCarrying(unsigned int uiItem);
unsigned char PLY_GiveGold(unsigned char ucGold);
unsigned char PLY_GiveHealth(unsigned char ucHealth);
unsigned char PLY_TakeHealth(unsigned char ucHealth);
void          PLY_PassedScreen(void);
unsigned char PLY_GetScreensPassed(void);
unsigned char PLY_GetVelocity(void);
void          PLY_SetVelocity(unsigned char ucVel);
unsigned char PLY_GetDirection(void);
void          PLY_SetDirection(unsigned char ucDir);
COORDINATE    PLY_GetCoordinate(void);
void          PLY_SetCoordinate(COORDINATE objNewCoord);

#endif
