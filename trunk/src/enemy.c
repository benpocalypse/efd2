#include "enemy.h"

// All of the data related to ANY enemy is stored here.
typedef struct
{
    //ENEMY_TYPE eType;
    unsigned char ucHitpoints;
    unsigned int  uiInventory;
    COORDINATE    objLocation;
} ENEMY;

// Holds our classes Enemy data internally.
static ENEMY objEnemy;

void NME_Init()
{
    //objEnemy.eType = NONE;
    objEnemy.ucHitpoints = 3U;
    objEnemy.uiInventory = 0U;
    objEnemy.objLocation.ucBigX = 0U;
    objEnemy.objLocation.ucBigY = 0U;
    objEnemy.objLocation.scSmallX = 0;
    objEnemy.objLocation.scSmallY = 0;
}

void NME_SetCoordinate(COORDINATE objNewCoord)
{
    objEnemy.objLocation.ucBigX = objNewCoord.ucBigX;
    objEnemy.objLocation.ucBigY = objNewCoord.ucBigY;
    objEnemy.objLocation.scSmallX = objNewCoord.scSmallX;
    objEnemy.objLocation.scSmallY = objNewCoord.scSmallY;
}

COORDINATE NME_GetCoordinate(void)
{
    return objEnemy.objLocation;
}

