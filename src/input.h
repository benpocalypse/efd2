#ifndef _INPUT_H_
#define _INPUT_H_

typedef enum
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    A,
    B,
    X,
    Y,
    L,
    R,
    SELECT,
    START,
    UNKNOWN
    
} C_INPUT;

C_INPUT INPUT_GetButton(void);

#endif

