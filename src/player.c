#include "player.h"

// Holds our classes Player data internally.
static PLAYER objPlayer;

///****************************************************************************
/// Simply initializes all of our Player related variables to a sane default
/// state. This is the same as performing a reset.
///****************************************************************************
unsigned char PLY_Init(void)
{
    objPlayer.ucHealth = 3U;
    objPlayer.ucTotalHealth = 3U;
    objPlayer.ucGold = 0U;
    objPlayer.uiInventory = 0U;
    objPlayer.ucScreensPassed = 0U;
    objPlayer.objLocation.ucBigX = 0U;
    objPlayer.objLocation.ucBigY = 0U;
    objPlayer.objLocation.ucSmallX = 0U;
    objPlayer.objLocation.ucSmallY = 0U;
}

///****************************************************************************
/// Checks to see if the player currently has an item in their inventory, by
/// using some tricky math that stores 16 items in a single unsigned int.
///****************************************************************************
unsigned char PLY_IsCarrying(unsigned int uiItem)
{
    if((uiInv & uiItem) && uiItem) == TRUE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


///****************************************************************************
/// Simply adds gold to the player's stash.
///****************************************************************************
unsigned char PLY_GiveGold(unsigned char ucGold)
{
    objPlayer.ucGold += ucGold;

    // FIXME - Add a check here to see if the player has picked up more than X
    //         gold, and if so, increment their total health.
}


///****************************************************************************
/// Adds health to the player's current health, and make's sure we don't 
/// accidentally go over the total.
///****************************************************************************
unsigned char PLY_GiveHealth(unsigned char ucHealth)
{
    if(objPlayer.ucHealth < (objPlayer.ucTotalHealth - ucHealth))
    {
        objPlayer.ucHealth += ucHealth;
    }
    else
    {
        objPlayer.ucHealth = objPlayer.ucTotalHealth;
    }
}

///****************************************************************************
/// Takes away health from the player, being sure to not go into the negative.
///****************************************************************************
unsigned char PLY_TakeHealth(unsigned char ucHealth)
{
    if(ucHealth > objPlayer.ucHealth)
    {
        objPlayer.ucHealth = 0U;
    }
    else
    {
        objPlayer.ucHealth -= ucHealth;
    }
}

///****************************************************************************
/// Increments the number of Screens the player has passed so far.
///****************************************************************************
void PLY_PassedScreen(void)
{
    objPlayer.ucScreensPassed++;
}

///****************************************************************************
/// Simple getter for the number of screens passed.
///****************************************************************************
unsigned char PLY_GetScreensPassed(void)
{
    return objPlayer.ucScreensPassed;
}

///****************************************************************************
/// Simple getter for the player's current location.
///****************************************************************************
COORDINATE PLY_GetCoordinate(void)
{
    return objPlayer.objLocation;
}
