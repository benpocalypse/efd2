#include "player.h"


// Player class specific defines
#define MAX_SMALL_LOC   8

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


unsigned char PLY_GetVelocity(void)
{
    return ((objPlayer.ucVelocityAndDirection & 0xF0) >> 4);
}


void PLY_SetVelocity(unsigned char ucVel)
{
    objPlayer.ucVelocityAndDirection &= 0x0F;
    objPlayer.ucVelocityAndDirection |= (ucVel << 4);
}


unsigned char PLY_GetDirection(void)
{
    return objPlayer.ucVelocityAndDirection & 0x0F;
}


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
/// Move the players current location the correct number of pixels, taking care
/// of wrapping the Big/Small coordinates.
///****************************************************************************
void PLY_Move(signed char scX, signed char scY)
{
    signed char scTempX = objPlayer.objLocation.scSmallX;
    signed char scTempY = objPlayer.objLocation.scSmallY;
    
    scTempX += scX;
    scTempY += scY;

    // If we've wrapped into the next tile..
    if(scTempX >= MAX_SMALL_LOC)
    {// then increment our big tile location.
        objPlayer.objLocation.ucBigX += 1U;
        objPlayer.objLocation.scSmallX = scTempX - TILE_SIZE;
        return;
    }
    else
    {    
        if(scTempX < 0)
        {
            objPlayer.objLocation.ucBigX -= 1U;
            objPlayer.objLocation.scSmallX = TILE_SIZE + scTempX;
            return;
        }
        else
        {
            objPlayer.objLocation.scSmallX = scTempX;
        }
    }

    if(scTempY >= MAX_SMALL_LOC)
    {
        objPlayer.objLocation.ucBigY += 1U;
        objPlayer.objLocation.scSmallY = scTempY - TILE_SIZE;
        return;
    }
    else
    {
        if(scTempY < 0)
        {
            objPlayer.objLocation.ucBigY -= 1U;
            objPlayer.objLocation.scSmallY = TILE_SIZE + scTempY;
            return;
        }
        else
        {
            objPlayer.objLocation.scSmallY = scTempY;
        }
    }    
}
