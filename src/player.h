#ifndef _PLAYER_H_
#define _PLAYER_H_


// All of the data related to the player is stored  here.
typdef struct
{
    unsigned char ucHealth;
    unsigned char ucTotalHealth;
    unsigned char ucGold;
    unsigned int  uiInventory; // BITFIELD...MASKABLE, ORABLE(16 items total)
    unsigned char ucScreensPassed;
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
COORDINATE    PLY_GetCoordinate(void);
unsigned 

#endif
