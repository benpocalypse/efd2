#include "input.h"
#include <uzebox.h>

C_INPUT INPUT_GetButton(void)
{
	unsigned int uiJoy = ReadJoypad(0);
    
	if(uiJoy&BTN_LEFT)
	{
	    return (C_INPUT)IN_LEFT;
        //while(ReadJoypad(0)!=0);
	}
	if(uiJoy&BTN_RIGHT)
	{ 
        return (C_INPUT)IN_RIGHT;
	}
	if(uiJoy&BTN_UP)
	{
        return (C_INPUT)IN_UP;
	}
	if(uiJoy&BTN_DOWN)
	{
        return (C_INPUT)IN_DOWN;
	}
	if(uiJoy&BTN_A)
	{
        return (C_INPUT)IN_A;
	}
    if(uiJoy&BTN_B)
    {
        return (C_INPUT)IN_B;
    }
    if(uiJoy&BTN_X)
    {
        return (C_INPUT)IN_X;
    }
    if(uiJoy&BTN_Y)
    {
        return (C_INPUT)IN_Y;
    }
    if(uiJoy&BTN_SELECT)
    {
        return (C_INPUT)IN_SELECT;
    }
    if(uiJoy&BTN_START)
    {
        return (C_INPUT)IN_START;
    }    
}
