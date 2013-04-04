#ifndef _ENEMY_H_
#define _ENEMY_H_

typedef struct
{
    unsigned char ucType;
    unsigned char ucHitPoints;
    unsigned int  uiInventory;
    COORDINATE    objLocation;
} ENEMY;

#endif
