#include "player.h"

// All of the data related to the player is stored  here.
typedef struct
{
    unsigned char ucHealthAndState;
    unsigned char ucTotalHealth;
    unsigned char ucGold;
    unsigned int  uiInventory; // BITFIELD...MASKABLE, ORABLE(16 items total)
    unsigned char ucScreensPassed;
    unsigned char ucVelocityAndDirection;
    COORDINATE    objLocation;
} PLAYER;

// Holds our classes Player data internally.
static PLAYER objPlayer;

///****************************************************************************
/// Simply initializes all of our Player related variables to a sane default
/// state. This is the same as performing a reset.
///****************************************************************************
void PLY_Init(void)
{
    objPlayer.ucHealthAndState = 0x30;
    objPlayer.ucTotalHealth = 3U;
    objPlayer.ucGold = 0U;
    objPlayer.uiInventory = 0U;
    objPlayer.ucScreensPassed = 0U;
    objPlayer.ucVelocityAndDirection = NO_DIR;
    objPlayer.objLocation.ucBigX = 0U;
    objPlayer.objLocation.ucBigY = 0U;
    objPlayer.objLocation.scSmallX = 0;
    objPlayer.objLocation.scSmallY = 0;
}


///****************************************************************************
/// Checks to see if the player currently has an item in their inventory, by
/// using some tricky math that stores 16 items in a single unsigned int.
///****************************************************************************
unsigned char PLY_IsCarrying(unsigned int uiItem)
{
    return ((objPlayer.uiInventory & uiItem) && uiItem);
}


///****************************************************************************
/// Simply adds gold to the player's stash.
///****************************************************************************
unsigned char PLY_GiveGold(unsigned char ucGold)
{
    objPlayer.ucGold += ucGold;

    // FIXME - Add a check here to see if the player has picked up more than X
    //         gold, and if so, increment their total health.
	
	return 0;
}


///****************************************************************************
/// Adds health to the player's current health, and make's sure we don't 
/// accidentally go over the total.
///****************************************************************************
unsigned char PLY_GiveHealth(unsigned char ucHealth)
{
	/*
	unsigned char ucTempHealth = (objPlayer.ucHealthAndState & 0xF0) >> 4;
	
    if(ucTempHealth < (objPlayer.ucTotalHealth - ucHealth))
    {
        objPlayer.ucHealthAndState += (ucHealth << 4);
    }
    else
    {
        objPlayer.ucHealthAndState = (objPlayer.ucHealthAndState & 0x0F) + (objPlayer.ucTotalHealth << 4);
    }*/
	
	return 0;
}

///****************************************************************************
/// Takes away health from the player, being sure to not go into the negative.
///****************************************************************************
unsigned char PLY_TakeHealth(unsigned char ucHealth)
{
	/*
	unsigned char ucTempHealth = (objPlayer.ucHealthAndState & 0xF0) >> 4;
	
    if(ucHealth > ucTempHealth)
    {
        objPlayer.ucHealthAndState = (objPlayer.ucHealthAndState & 0x0F);
    }
    else
    {
        objPlayer.ucHealthAndState -= (ucHealth << 4);
    }
	 */
	return 0;
}

///****************************************************************************
/// Increments the number of Screens the player has passed so far.
///****************************************************************************
void PLY_PassedScreen(void)
{
    objPlayer.ucScreensPassed++;
}


///****************************************************************************
/// Simple getter for the current Player Health.
///****************************************************************************
unsigned char PLY_GetHealth(void)
{
    return (objPlayer.ucHealthAndState & 0xF0) >> 4;
}

///****************************************************************************
/// Simple getter for the Total Player Health.
///****************************************************************************
unsigned char PLY_GetTotalHealth(void)
{
    return objPlayer.ucTotalHealth;
}


///****************************************************************************
/// Simple getter for the number of screens passed.
///****************************************************************************
unsigned char PLY_GetScreensPassed(void)
{
    return objPlayer.ucScreensPassed;
}


///****************************************************************************
/// Getter for the Player's velocity that gets the value from a single 8 bit
/// variable along with the players direction.
///****************************************************************************
unsigned char PLY_GetVelocity(void)
{
    return ((objPlayer.ucVelocityAndDirection & 0xF0) >> 4);
}


///****************************************************************************
/// Setter for the Player's velocity that stores the value in a single 8 bit
/// variable along with the players direction.
///****************************************************************************
void PLY_SetVelocity(unsigned char ucVel)
{
    objPlayer.ucVelocityAndDirection &= 0x0F;
    objPlayer.ucVelocityAndDirection |= (ucVel << 4);
}


///****************************************************************************
/// Getter for the Player's direction that gets the value from a single 8 bit
/// variable along with the players velocity.
///****************************************************************************
unsigned char PLY_GetDirection(void)
{
    return objPlayer.ucVelocityAndDirection & 0x0F;
}


///****************************************************************************
/// Setter for the Player's direction that stores the value in a single 8 bit
/// variable along with the players velocity.
///****************************************************************************
void PLY_SetDirection(unsigned char ucDir)
{
    objPlayer.ucVelocityAndDirection &= 0xF0;
    objPlayer.ucVelocityAndDirection |= ucDir;
}


///****************************************************************************
/// Simple getter for the player's current location.
///****************************************************************************
COORDINATE PLY_GetCoordinate(void)
{
    return objPlayer.objLocation;
}


///****************************************************************************
/// Simple setter for the players current location.
///****************************************************************************
void PLY_SetCoordinate(COORDINATE objNewCoord)
{
    objPlayer.objLocation.ucBigX = objNewCoord.ucBigX;
    objPlayer.objLocation.ucBigY = objNewCoord.ucBigY;
    objPlayer.objLocation.scSmallX = objNewCoord.scSmallX;
    objPlayer.objLocation.scSmallY = objNewCoord.scSmallY;
}

///****************************************************************************
/// Simple getter for the player's current 'state'.
///****************************************************************************
unsigned char PLY_GetState(void)
{
	return (objPlayer.ucHealthAndState & 0x0F);
}

///****************************************************************************
/// Simple setter for the player's current 'state'.
///****************************************************************************
void PLY_SetState(unsigned char ucState)
{
	objPlayer.ucHealthAndState = (objPlayer.ucHealthAndState & 0xF0) + ucState;
}
