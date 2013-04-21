#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// Bools
#define FALSE 0U
#define TRUE  1U

// Item related defines, a total of 16 in game items.
#define HEALTH_POTION     0x0001
#define HASTE_POTION      0x0002
#define INVINCIBLE_POTION 0x0004
#define STRENGTH_POTION   0x0008
#define LONGSWORD         0x0010
#define LIGHTSWORD        0x0020
#define CLOTH_ARMOR       0x0040
#define LEATHER_ARMOR     0x0080
#define GOLD              0x0100
#define KEY               0x0200


typedef struct
{
    unsigned char ucBigX;
    unsigned char ucBigY;
    unsigned char ucSmallX;
    unsigned char ucSmallY;
} COORDINATE;


///////////////////////////////////////////////////////////////////////////////
/// Generates random numbers starting at and including ucMin up to and 
/// including ucMax, and returns the number.
///////////////////////////////////////////////////////////////////////////////
unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax)
{
	return (unsigned char)(ucMin + rand() % (ucMax - ucMin));
}
unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax);


unsigned char IsCarrying(unsigned int uiInv, unsigned int uiItem)
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

#endif
