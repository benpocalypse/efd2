#include "input.h"
#include <uzebox.h>

C_INPUT INPUT_GetButton(void)
{
	unsigned int uiJoy;

	uiJoy=ReadJoypad(0);
    
	if(uiJoy&BTN_LEFT)
	{
	    return (C_INPUT)LEFT;
        //while(ReadJoypad(0)!=0);
	}
	if(uiJoy&BTN_RIGHT)
	{ 
        return (C_INPUT)RIGHT;
	}
	if(uiJoy&BTN_UP)
	{
        return (C_INPUT)UP;
	}
	if(uiJoy&BTN_DOWN)
	{
        return (C_INPUT)DOWN;
	}
	if(uiJoy&BTN_A)
	{
        return (C_INPUT)A;
	}
    if(uiJoy&BTN_B)
    {
        return (C_INPUT)B;
    }
    if(uiJoy&BTN_X)
    {
        return (C_INPUT)X;
    }
    if(uiJoy&BTN_Y)
    {
        return (C_INPUT)Y;
    }
    if(uiJoy&BTN_SELECT)
    {
        return (C_INPUT)SELECT;
    }
    if(uiJoy&BTN_START)
    {
        return (C_INPUT)START;
    }    
}
