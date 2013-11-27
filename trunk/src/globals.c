#include "globals.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <avr/pgmspace.h>

// Internal function prototypes.
void GLBi_SetFont(unsigned char x,unsigned char y,unsigned char c);


///****************************************************************************
/// Generates random numbers starting at and including ucMin up to and 
/// including ucMax, and returns the number.
///****************************************************************************
unsigned char GLB_RandomNum(unsigned char ucMin, unsigned char ucMax)
{
	return (unsigned char)(ucMin + rand() % ((ucMax - ucMin)+1));
}


///****************************************************************************
/// This function takes in a map coordinate, and then moves it the requested
/// X and Y amounts, taking into account wrapping the Big/Small numbers.
///****************************************************************************
COORDINATE GLB_MoveCoordinate(COORDINATE coord, signed char scX, signed char scY)
{
    signed char scTempX = coord.scSmallX;
    signed char scTempY = coord.scSmallY;
    
    scTempX += scX;
    scTempY += scY;

    // If we've wrapped into the next tile..
    if(scTempX >= TILE_SIZE)
    {// then increment our big tile location.
        coord.ucBigX += 1U;
        coord.scSmallX = scTempX - TILE_SIZE;        
    }
    else
    {    
        if(scTempX < 0)
        {
            coord.ucBigX -= 1U;
            coord.scSmallX = TILE_SIZE + scTempX;
        }
        else
        {
            coord.scSmallX = scTempX;
        }
    }

    if(scTempY >= TILE_SIZE)
    {
        coord.ucBigY += 1U;
        coord.scSmallY = scTempY - TILE_SIZE;
    }
    else
    {
        if(scTempY < 0)
        {
            coord.ucBigY -= 1U;
            coord.scSmallY = TILE_SIZE + scTempY;
        }
        else
        {
            coord.scSmallY = scTempY;
        }
    }
    
    return coord;
}


///****************************************************************************
/// This function takes a COORDINATE and a MapObj and tests to see if they are
/// colliding. If they are, it returns true, otherwise, returns false.
///****************************************************************************
unsigned char GLB_CoordinateToObjectCollision(COORDINATE coord, MapObject mapobj)
{
    // First, we check to see if our coordinate and map object are close enough to 
    // merrit check the individual pixels of the coordinate and object.
    if((coord.ucBigX <= (mapobj.ucX+1)) && (coord.ucBigX >= (mapobj.ucX-1)) &&
       (coord.ucBigY <= (mapobj.ucY+1)) && (coord.ucBigY >= (mapobj.ucY-1)))
    {
        // Now, generate actual X and Y coordinates before checking them.
        unsigned int uiTX = (coord.ucBigX * 8) + coord.scSmallX + 4;
        unsigned int uiTY = (coord.ucBigY * 8) + coord.scSmallY + 4;
        unsigned int uiDX = (mapobj.ucX * 8) + 4;
        unsigned int uiDY = (mapobj.ucY * 8) + 4;
        
        // If the coordinate and the object are within 9-ish pixels, they're
        // colliding.
        if(((uiTX + 9) >= uiDX) && (uiTX <= (uiDX + 9)) && 
           ((uiTY + 9) >= uiDY) && (uiTY <= (uiDY + 9)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

///****************************************************************************
/// This is a custom print function that uses just tiles to print to the screen
/// using a reduced font set so we can save flash space for other things.
///****************************************************************************
void GLB_PrintString(unsigned char x,unsigned char y,const char *string)
{
    unsigned char c;
    unsigned char i = 0U;

    while(1)
    {
        c = pgm_read_byte(&(string[i++]));

        if(c == 0)
            break;

        GLBi_SetFont(x++,y,c);
    }
}


///****************************************************************************
/// This is a custom print function that uses just tiles to print a number to
/// the screen using a reduced font set.
///****************************************************************************
void GLB_PrintNumber(unsigned char x, unsigned char y, unsigned char ucNum)
{
    // Maximum of 3 digits in an unsigned char
    char cTemp[3] = {0, 0, 0};
    unsigned char i = 0;


    // Loop through our digits and convert them to ASCII.
    while (i <= 2)
    {
        if (ucNum > 0U)
        {
            cTemp[2 - i] = 48U + (ucNum % 10U);
            ucNum /= 10U;
        }
        else
        {
            cTemp[2 - i] = 0; // Pad with spaces if needed
        }
        
        i++;
    }

    //  Now print out our number, one digit at a time.
    GLBi_SetFont(x,  y, cTemp[2]);
    GLBi_SetFont(x+1,y, cTemp[1]);
    GLBi_SetFont(x+2,y, cTemp[0]);
}


///****************************************************************************
/// This function translates the ASCII char passed in to our specific internal
/// font tile table. It relies on the font being loaded after the tiles, but
/// before the sprites.
///****************************************************************************
void GLBi_SetFont(unsigned char x,unsigned char y,unsigned char c)
{
    // If we aren't requesting a space, then convert our ASCII char
    // to something our internal font table understands.
    if(c != 32)
    {
    
    // NOTE - This weird nested if/else group is to prevent our - reassigning
    //        of c to affect the further font assignments. Weird, but deal with
    //        it.
    
        // Numbers
        // 0 = 62
        // 9 = 71
        if((c >= 48) && (c <= 58))
        {
            c = c + 16;
            SetTile(x,y,c);
        }
        else
        {
            // Punctuation
            // colon and semicolon = 72, 73
            if((c == 58) || (c == 59))
            {
                c = c + 16;
            }
            else
            {
                // comma, dash, and period = 74, 75, and 76
                if((c >= 44) && (c <= 46))
                {
                    c = c + 32;
                }
                else
                {
                    // exclamation point = 77
                    if(c == 33)
                    {
                        c = c + 46;
                    }
                    else
                    {
                        // Uppercase letters
                        // A = 78
                        // B = 103
                        if((c >= 65) && (c <= 90))
                        {
                            c = c + 15;
                        }
                        else
                        {
                            // Lowercase letters
                            // a = 104
                            // z = 129
                            if((c >= 97) && (c <= 122))
                            {
                                c = c + 9;                             
                            }
                        }
                    }
                }
            }
        }
        
        // Finally, set our tile as requested.
        SetTile(x,y,c);
    }
}

