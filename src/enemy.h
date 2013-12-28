#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "globals.h"

enum ENEMY_TYPE
{
    SKELETON,
    ENEMY_NONE
};

// Function prototypes that allow outside classes to get access
// to the Player classes data.
void          NME_Init(void);
void          NME_SetCoordinate(COORDINATE objNewCoord);
COORDINATE    NME_GetCoordinate(void);
/*unsigned char NME_IsCarrying(unsigned int uiItem);
unsigned char NME_TakeHealth(unsigned char ucHealth);
unsigned char NME_GetHealth(void);
unsigned char NME_GetTotalHealth(void);
unsigned char NME_GetVelocity(void);
void          NME_SetVelocity(unsigned char ucVel);
unsigned char NME_GetDirection(void);
void          NME_SetDirection(unsigned char ucDir);
*/
#endif
