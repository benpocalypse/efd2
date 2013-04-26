#include "logicmanager.h"
#include "player.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "globals.h"

#include <uzebox.h>

#include "data/sprites.inc"

// Player sprite related defines
#define PLAYER_SPRITE       0U
#define PLAYER_IDLE         0U
#define PLAYER_RUN1         1U
#define PLAYER_RUN2         0U
#define PLAYER_RUN3         2U
#define PLAYER_RUN4         0U
#define PLAYER_RUN_FRAMES   4U

// Logic related defines
#define MAX_PLAYER_VELOCITY 3U

// Enum that holds each discrete state in our Logic FSM
enum LOGIC_STATE
{
    INIT,
    PROCESS_INPUT,
    UPDATE_PLAYER,
    ENEMY_LOGIC,
    COLLISIONS,
    UNKNOWN
};


// Private class variables
LOGIC_STATE eCurrentState;
LOGIC_STATE eRequestedState;
unsigned char ucPlayerRunFrame;
unsigned char ucPlayerRun[PLAYER_RUN_FRAMES];


// Private class functions

// Mode handling functions
static void ModeEntry(LOGIC_STATE eState);
static void ModeExit(LOGIC_STATE eState);
static void ProcessInit(void);
static void ProcessInput(void);
static void ProcessUpdatePlayer(void);
static void ProcessEnemyLogic(void);
static void ProcessCollisions(void);


///****************************************************************************
/// Simply initializes all of our Logic related variables to a default state.
///****************************************************************************
void LGC_Init(void)
{
    eCurrentState = UNKNOWN;
    eRequestedState = INIT;
    
    // Now handle some sprite inits.
    SetSpritesTileTable(efd2_sprites);
    SetSpriteVisibility(true);
    sprites[PLAYER_SPRITE].tileIndex = PLAYER_IDLE;
    sprites[PLAYER_SPRITE].x = OFF_SCREEN;
    sprites[PLAYER_SPRITE].y = OFF_SCREEN;
    sprites[PLAYER_SPRITE].flags = 0U;
    
    ucPlayerRunFrame = 0U;
    ucPlayerRun[0] = PLAYER_RUN1;
    ucPlayerRun[1] = PLAYER_RUN2;
    ucPlayerRun[2] = PLAYER_RUN3;
    ucPlayerRun[3] = PLAYER_RUN4;
}


///****************************************************************************
/// This function is the heart of our FSM. It handles changing modes, and
/// processing any tasks that would happen inside any of the individual states.
///****************************************************************************
void LGC_ManageLogic(void)
{
    if(bRunning = true)
    {    
        // If we're supposed to switch states, take care of that here.
        if(eCurrentState != eRequestedState)
        {
            ModeExit(eCurrentState);
            ModeEntry(eRequestedState);
            
            eCurrentState = eRequestedState;
        }
        
        switch(eCurrentState)
        {
            case INIT:
                ProcessInit();
                break;
            
            case PROCESS_INPUT:
                ProcessInput();
                break;
                
            case UPDATE_PLAYER:
                ProcessUpdatePlayer();
                break;
                
            case ENEMY_LOGIC:
                ProcessEnemyLogic();
                break;

            case COLLISIONS:
                ProcessCollisions();
                break;
                
            default:
                break;          
        }
    }
}


void LGC_Start(void)
{
    bRunning = true;
}


void LGC_Stop(void)
{
    bRunning = false;
}


///****************************************************************************
/// This function takes care of any one time actions that will happen upon 
/// exiting from the current mode.
///****************************************************************************
void ModeExit(LOGIC_STATE eState)
{
    switch(eState)
    {
        case INIT:
            for(unsigned char j = 0; j < MAPHEIGHT; j++)
            {
                for(unsigned char i = 0; i < MAPWIDTH; i++)
                {
                    if(MAP_TileIs(i, j) == MT_FLOOR)
                    {
                        COORDINATE objNewCoord;
                        objNewCoord.ucBigX = i;
                        objNewCoord.ucBigY = j;
                        objNewCoord.ucSmallX = 0U;
                        objNewCoord.ucSmallY = 0U;
                        
                        PLY_SetCoordinate(objNewCoord);
                        sprites[PLAYER_SPRITE].x = (i+MAP_X_OFFSET)*8;
                        sprites[PLAYER_SPRITE].y = (j+MAP_Y_OFFSET)*8;
                        // FIXME - It'l too late, and I dont know what I'm doing.
                    }
                }
            }
            break;
        
        case PROCESS_INPUT:
            break;
            
        case UPDATE_PLAYER:
            break;
            
        case ENEMY_LOGIC:
            break;

        case COLLISIONS:
            break;
            
        default:
            break;          
    }
}


///****************************************************************************
/// This function takes care of any one time actions that will happen upon 
/// entry into a new mode.
///****************************************************************************
void ModeEntry(LOGIC_STATE eState)
{
    switch(eState)
    {
        case INIT:
            break;
        
        case PROCESS_INPUT:
            break;
            
        case UPDATE_PLAYER:
            break;
            
        case ENEMY_LOGIC:
            break;

        case COLLISIONS:
            break;
            
        default:
            break;          
    }
}


void ProcessInit(void)
{
    eRequestedState = PROCESS_INPUT;
}

void ProcessInput(void)
{
    if(INPUT_GetButton() == IN_UP)
    {
        PLY_SetDirection(UP);
        if(PLY_GetVelocity() < MAX_PLAYER_VELOCITY)
        {
            PLY_SetVelocity(++PLY_GetVelocity());
        }
    }
    
    if(INPUT_GetButton() == IN_LEFT)
    {
        PLY_SetDirection(LEFT);
        if(PLY_GetVelocity() < MAX_PLAYER_VELOCITY)
        {
            PLY_SetVelocity(++PLY_GetVelocity());
        }
    }
    
    if(INPUT_GetButton() == IN_RIGHT)
    {
        PLY_SetDirection(RIGHT);
        if(PLY_GetVelocity() < MAX_PLAYER_VELOCITY)
        {
            PLY_SetVelocity(++PLY_GetVelocity());
        }
    }
    
    if(INPUT_GetButton() == DOWN)
    {
        PLY_SetDirection(DOWN);
        if(PLY_GetVelocity() < MAX_PLAYER_VELOCITY)
        {
            PLY_SetVelocity(++PLY_GetVelocity());
        }
    }
    
    if(INPUT_GetButton() == IN_NONE)
    {
        PLY_SetDirection(NONE);
        PLY_SetVelocity(0U);
    }
    
    eRequestedState = UPDATE_PLAYER;
}


// FIXME - This function needs to be finished
void ProcessUpdatePlayer(void)
{
    if(PLY_GetDirection() == NONE)
    {
        sprites[PLAYER_SPRITE].tileIndex = PLAYER_IDLE;
    }
    else
    {
        sprites[PLAYER_SPRITE].tileIndex = ucPlayerRun[(ucPlayerRunFrame++)%(PLAYER_RUN_FRAMES-1)];
    }
    
    //if(PLY_GetDirection() == RIGHT)
    
    eRequestedState = COLLISIONS;
}

void ProcessEnemyLogic(void)
{
    // FIXME - duh    
}

void ProcessCollisions(void)
{
    eRequestedState = PROCESS_INPUT;
}

