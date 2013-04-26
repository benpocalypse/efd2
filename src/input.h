#ifndef _INPUT_H_
#define _INPUT_H_

typedef enum
{
    IN_NONE,
    IN_UP,
    IN_DOWN,
    IN_LEFT,
    IN_RIGHT,
    IN_A,
    IN_B,
    IN_X,
    IN_Y,
    IN_L,
    IN_R,
    IN_SELECT,
    IN_START,
    IN_UNKNOWN
    
} C_INPUT;

C_INPUT INPUT_GetButton(void);

#endif

