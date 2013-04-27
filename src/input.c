#include "input.h"
#include <uzebox.h>


///****************************************************************************
/// This function is just a helper function that let's us abstract the reading
/// of the controllers a bit. In other words, it just makes things easier
/// elsewhere in our code.
///****************************************************************************
unsigned char INPUT_GetButton(C_INPUT eButton)
{
	unsigned int uiJoy = ReadJoypad(0);
    
    if(eButton == (C_INPUT)IN_UP)
    {
        return ((uiJoy&BTN_UP) == BTN_UP);
    }
    if(eButton == (C_INPUT)IN_LEFT)
    {
        return ((uiJoy&BTN_LEFT) == BTN_LEFT);
    }
    if(eButton == (C_INPUT)IN_RIGHT)
    {
        return ((uiJoy&BTN_RIGHT) == BTN_RIGHT);
    } 
    if(eButton == (C_INPUT)IN_DOWN)
    {
        return ((uiJoy&BTN_DOWN) == BTN_DOWN);
    }
    if(eButton == (C_INPUT)IN_A)
    {
        return ((uiJoy&BTN_A) == BTN_A);
    }
    if(eButton == (C_INPUT)IN_B)
    {
        return ((uiJoy&BTN_B) == BTN_B);
    }
    if(eButton == (C_INPUT)IN_X)
    {
        return ((uiJoy&BTN_X) == BTN_X);
    }
    if(eButton == (C_INPUT)IN_Y)
    {
        return ((uiJoy&BTN_Y) == BTN_Y);
    }
    if(eButton == (C_INPUT)IN_SELECT)
    {
        return ((uiJoy&BTN_SELECT) == BTN_SELECT);
    }
    if(eButton == (C_INPUT)IN_START)
    {
        return ((uiJoy&BTN_START) == BTN_START);
    }

    if(eButton == (C_INPUT)IN_NONE)
    {
        return (uiJoy == 0);
    }
}

