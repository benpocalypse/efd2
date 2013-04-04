#ifndef _PLAYER_H_
#define _PLAYER_H_


typdef struct
{
    unsigned char ucHitPoints;
    unsigned char ucGold;
    unsigned int  uiInventory; // BITFIELD...MASKABLE, ORABLE(16 items total)
    unsigned char ucScreensPassed;
    COORDINATE    objLocation;
} PLAYER;

#endif
