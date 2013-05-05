#include "logicmanager.h"
#include "player.h"
#include "input.h"
#include "map.h"
#include "player.h"
//#include "globals.h"

#include <avr/pgmspace.h>
#include <uzebox.h>

// Allows the Logic Manager class to access our sprite tiles
//#include "data/sprites.inc"

// Player sprite related defines
#define PLAYER_SPRITE       0U
#define PLAYER_IDLE         0U
#define PLAYER_RUN1         1U
#define PLAYER_RUN2         0U
#define PLAYER_RUN3         2U
#define PLAYER_RUN4         0U
#define PLAYER_RUN_FRAMES   4U

// This just defines how long we delay on each frame to achieve smooth animation
#define FRAME_COUNTER       3U

// Logic related defines
#define MAX_PLAYER_VELOCITY 3U

// Enum that holds each discrete state in our Logic FSM
typedef enum
{
    INIT,
    PROCESS_INPUT,
    UPDATE_PLAYER,
    ENEMY_LOGIC,
    COLLISIONS,
    UNKNOWN
}LOGIC_STATE;


// Private class variables
static LOGIC_STATE eCurrentState;
static LOGIC_STATE eRequestedState;

// These variables are used to animate the player
static unsigned char ucPlayerRunFrame;
static unsigned char ucPlayerRun[PLAYER_RUN_FRAMES];
static unsigned char bRunning;
static unsigned char bExitReached;


// Private class functions

// Mode handling functions
static void ModeEntry(LOGIC_STATE eState);
static void ModeExit(LOGIC_STATE eState);
static void ProcessInit(void);
static void ProcessInput(void);
static void ProcessUpdatePlayer(void);
static void ProcessEnemyLogic(void);
static void ProcessCollisions(void);

// Internal helper functions
static COORDINATE SetPlayerStartLocation(void);
static void DrawPlayer(void);

///****************************************************************************
/// Simply initializes all of our Logic related variables to a default state.
///****************************************************************************
void LGC_Init(void)
{
    // Set up our FSM variables to known safe states
    eCurrentState = UNKNOWN;
    eRequestedState = INIT;
    bRunning = false;
    bExitReached = false;
    
    // Now handle some sprite inits
    //SetSpritesTileTable(efd2_sprites);
    //SetSpriteVisibility(true);
   
    sprites[PLAYER_SPRITE].tileIndex = PLAYER_IDLE;
    sprites[PLAYER_SPRITE].x = OFF_SCREEN;
    sprites[PLAYER_SPRITE].y = OFF_SCREEN;
    sprites[PLAYER_SPRITE].flags = 0U;
    
    // Now set up our player animation variables
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
    if(bRunning == true)
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
        
        // FIXME - This should be handled elsewhere.
        DrawPlayer();
    }
}

unsigned char LGC_ExitReached(void)
{
    return bExitReached;
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
            bExitReached = false;
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
/// Here, we just initialize all of our internal variables in order to start
/// a new screen in our dungeon.
///****************************************************************************
void ProcessInit(void)
{
    // Put our player next to the entry door.
    PLY_SetCoordinate(SetPlayerStartLocation());

    // Now switch to our next state.
    eRequestedState = PROCESS_INPUT;
}


///****************************************************************************
/// This function looks at what button the player is pressing, and then sets up
/// the direction and speed with which the player wants to move. Keep in mind
/// we don't actually move at all yet, just get ready to do so.
///****************************************************************************
void ProcessInput(void)
{

    // FIXME - Pressing diagonal directions currently just makes the player run
    //         slowly. We need to handle this by just keeping them going in the
    //         same direction they were, without allowing them to travel in a
    //         diagonal direction.

    // If the player presses up...
    if(INPUT_GetButton(IN_UP) == true)
    {//...and we were going up before...
        if(PLY_GetDirection() == UP)
        {// then speed up!
            if(PLY_GetVelocity() < MAX_PLAYER_VELOCITY)
            {
                unsigned char ucVel = PLY_GetVelocity();
                PLY_SetVelocity(++ucVel);
            }
        }
        else
        {//...otherwise, start going up.
            PLY_SetDirection(UP);
            PLY_SetVelocity(1U);
        }
    }
    
    // If the player presses left...
    if(INPUT_GetButton(IN_LEFT) == true)
    {//...and we were going left before...
        if(PLY_GetDirection() == LEFT)
        {// then speed up!
            if(PLY_GetVelocity() < MAX_PLAYER_VELOCITY)
            {
                unsigned char ucVel = PLY_GetVelocity();
                PLY_SetVelocity(++ucVel);
            }
        }
        else
        {//...otherwise start going left.
            PLY_SetDirection(LEFT);
            PLY_SetVelocity(1U);
        }
    }
    
    // If the player presses right...
    if(INPUT_GetButton(IN_RIGHT) == true)
    {//...and we were going right before...
        if(PLY_GetDirection() == RIGHT)
        {// then speed up!
            if(PLY_GetVelocity() < MAX_PLAYER_VELOCITY)
            {
                unsigned char ucVel = PLY_GetVelocity();
                PLY_SetVelocity(++ucVel);
            }
        }
        else
        {//...otherwise start going right.
            PLY_SetDirection(RIGHT);
            PLY_SetVelocity(1U);
        }
    }
    
    // If the player presses down...
    if(INPUT_GetButton(IN_DOWN) == true)
    {//...and we were going right before...
        if(PLY_GetDirection() == DOWN)
        {// then speed up!
            if(PLY_GetVelocity() < MAX_PLAYER_VELOCITY)
            {
                unsigned char ucVel = PLY_GetVelocity();
                PLY_SetVelocity(++ucVel);
            }
        }
        else
        {//...otherwise start going down.
            PLY_SetDirection(DOWN);
            PLY_SetVelocity(1U);
        }
    }
    
    // And finally, if the player isn't pushing anything,
    // then coast to a stop slowly.
    if(INPUT_GetButton(IN_NONE) == true)
    {
        unsigned char ucVel = PLY_GetVelocity();
        
        if(ucVel == 0U)
        {
            PLY_SetDirection(NO_DIR);
        }
        else
        {
            PLY_SetVelocity(--ucVel);
        }
    }
    
    eRequestedState = UPDATE_PLAYER;
}


///****************************************************************************
/// This function only checks for collisions between the player and the map, 
/// and then adjusts the players position accordingly. This has nothing to do
/// with collisions between the player and enemies or items, only to do with
/// moving the player around the map correctly.
///****************************************************************************
void ProcessUpdatePlayer(void)
{
    COORDINATE objCurrentCoord = PLY_GetCoordinate();
    COORDINATE objNewCoord = PLY_GetCoordinate();
    
    MapObject objExitDoor = MAP_GetDoor(MT_EXIT);

    // Move our new coord right
    if(PLY_GetDirection() == RIGHT)
    {
        objNewCoord = GLB_MoveCoordinate(objCurrentCoord, PLY_GetVelocity(), 0);
    }
    
    // Move our new coord left.
    if(PLY_GetDirection() == LEFT)
    {
        objNewCoord = GLB_MoveCoordinate(objCurrentCoord, -PLY_GetVelocity(), 0);
    }

    // Move our new coord up.
    if(PLY_GetDirection() == UP)
    {
        objNewCoord = GLB_MoveCoordinate(objCurrentCoord, 0, -PLY_GetVelocity());
    }

    // Move our new coord down.
    if(PLY_GetDirection() == DOWN)
    {
        objNewCoord = GLB_MoveCoordinate(objCurrentCoord, 0, PLY_GetVelocity());
    }
    
    
    // Before we check for collisions, let's see if the player is at the exit door.
    if((objNewCoord.ucBigX == objExitDoor.ucX) && (objNewCoord.ucBigY == objExitDoor.ucY))
    {
        bExitReached = true;
        return;
    }
    
    
    // Now, we look at our new coord, and decide if it's inside a wall. If it is,
    // then we move it back outside the wall, and stop the player.
     
    // First check left.
    if(MAP_TileIs(objCurrentCoord.ucBigX-1, objCurrentCoord.ucBigY) != MT_FLOOR)
    {
        // If we've moved into a wall, then fix our location and stop us.
        if(objNewCoord.ucBigX == (objCurrentCoord.ucBigX-1))
        {
            objNewCoord.ucBigX++;
            objNewCoord.scSmallX = 0;
            
            PLY_SetDirection(NO_DIR);
            PLY_SetVelocity(0U);
        }
    }
    
    // Now check right.
    if(MAP_TileIs(objCurrentCoord.ucBigX+1, objCurrentCoord.ucBigY) != MT_FLOOR)
    {
        // If we've moved into a wall, then fix our location and stop us.
        if((objNewCoord.ucBigX == objCurrentCoord.ucBigX) && (objNewCoord.scSmallX > 0))
        {
            objNewCoord.scSmallX = 0;
            
            PLY_SetDirection(NO_DIR);
            PLY_SetVelocity(0U);
        }
    }
    
    // Now check up.
    if(MAP_TileIs(objCurrentCoord.ucBigX, objCurrentCoord.ucBigY-1) != MT_FLOOR)
    {
        // If we've moved into a wall, then fix our location and stop us.
        if(objNewCoord.ucBigY == (objCurrentCoord.ucBigY-1))
        {
            objNewCoord.ucBigY++;
            objNewCoord.scSmallY = 0;
            
            PLY_SetDirection(NO_DIR);
            PLY_SetVelocity(0U);
        }
    }
    
    // Now check down.
    if(MAP_TileIs(objCurrentCoord.ucBigX, objCurrentCoord.ucBigY+1) != MT_FLOOR)
    {
        // If we've moved into a wall, then fix our location and stop us.
        if((objNewCoord.ucBigY == objCurrentCoord.ucBigY) && (objNewCoord.scSmallY > 0))
        {
            objNewCoord.scSmallY = 0;
            
            PLY_SetDirection(NO_DIR);
            PLY_SetVelocity(0U);
        }
    }
    
    // And finally, update our player location    
    PLY_SetCoordinate(objNewCoord);
    
/*    Print(1,1,PSTR("bX:"));
    Print(1,2,PSTR("bY:"));
    Print(1,3,PSTR("sX:"));
    Print(1,4,PSTR("sY:"));
    

    PrintByte(5, 1, objNewCoord.ucBigX, true);
    PrintByte(5, 2, objNewCoord.ucBigY, true);
    PrintByte(5, 3, objNewCoord.scSmallX, true);
    PrintByte(5, 4, objNewCoord.scSmallY, true);  
    PrintByte(5, 5, objExitDoor.ucX, true);
    PrintByte(5, 6, objExitDoor.ucY, true);
*/    
    // Move to our next state.
    eRequestedState = ENEMY_LOGIC;
}


// FIXME - Implement functionality
void ProcessEnemyLogic(void)
{
    eRequestedState = COLLISIONS;
}


// FIXME - Implement functionality
void ProcessCollisions(void)
{
    

    eRequestedState = PROCESS_INPUT;
}


///****************************************************************************
/// This is a helper function that will place the player next to the entrance
/// door. All it does is get the location of the entrance, and check the four
/// tiles adjacent to see if they're open floor.
///****************************************************************************
COORDINATE SetPlayerStartLocation(void)
{
    // Start by getting the location of our entry door.
    MapObject objTempMapLocation = MAP_GetDoor(true);
    COORDINATE objTempCoord;

    objTempCoord.ucBigX = 0U;
    objTempCoord.ucBigY = 0U;
    objTempCoord.scSmallX = 0;
    objTempCoord.scSmallY = 0;

    // Then find a spot next to the entry door that is open (aka is floor)
    if(MAP_TileIs(objTempMapLocation.ucX+1, objTempMapLocation.ucY) == MT_FLOOR)
    {
        objTempCoord.ucBigX = objTempMapLocation.ucX+1;
        objTempCoord.ucBigY = objTempMapLocation.ucY;

        return objTempCoord;
    }

    if(MAP_TileIs(objTempMapLocation.ucX-1, objTempMapLocation.ucY) == MT_FLOOR)
    {
        objTempCoord.ucBigX = objTempMapLocation.ucX-1;
        objTempCoord.ucBigY = objTempMapLocation.ucY;

        return objTempCoord;
    }

    if(MAP_TileIs(objTempMapLocation.ucX, objTempMapLocation.ucY+1) == MT_FLOOR)
    {
        objTempCoord.ucBigX = objTempMapLocation.ucX;
        objTempCoord.ucBigY = objTempMapLocation.ucY+1;

        return objTempCoord;
    }

    if(MAP_TileIs(objTempMapLocation.ucX, objTempMapLocation.ucY-1) == MT_FLOOR)
    {
        objTempCoord.ucBigX = objTempMapLocation.ucX;
        objTempCoord.ucBigY = objTempMapLocation.ucY-1;

        return objTempCoord;
    }

    return objTempCoord;
}


///****************************************************************************
/// All this helper function does is update the Player's sprite to the 
/// appropriate X/Y location on the screen, to reduce function clutter. It
/// also handles animation in the case of the player running.
///****************************************************************************
void DrawPlayer(void)
{
    // This keeps track of which frame of animation the player sprite
    // is currently using.
    static unsigned char ucAnimDelay = 0U;

    // First get our players coordinate within the map (which is going to be 
    // centered in the middle of the screen.)
    COORDINATE objTempCoord = PLY_GetCoordinate();

    // Then put our player sprite where it is supposed to be, by offsetting it's
    // position the correct amount.
    sprites[PLAYER_SPRITE].x = (((objTempCoord.ucBigX + MAP_X_OFFSET)*TILE_SIZE) + objTempCoord.scSmallX);
    sprites[PLAYER_SPRITE].y = (((objTempCoord.ucBigY + MAP_Y_OFFSET)*TILE_SIZE) + objTempCoord.scSmallY);
    
    // If we're not moving, set our sprite to the idle state.
    if(PLY_GetDirection() == NO_DIR)
    {
        sprites[PLAYER_SPRITE].tileIndex = PLAYER_IDLE;
    }
    else
    {//...otherwise, we're running, so let's animate.
        // Here we just try to determine which way to flip the player sprite.
        if(PLY_GetDirection() == LEFT)
        {
            sprites[PLAYER_SPRITE].flags = SPRITE_FLIP_X;
        }

        if(PLY_GetDirection() == RIGHT)
        {
            sprites[PLAYER_SPRITE].flags = 0;
        }

        // If we're running, we just loop our running animations.
        // This is just used to ensure we only change animation frames every
        // 150mS by delaying our animation updates.
        if((ucAnimDelay++)%(FRAME_COUNTER) == 0)
        {
            sprites[PLAYER_SPRITE].tileIndex = ucPlayerRun[(ucPlayerRunFrame++)%(PLAYER_RUN_FRAMES)];        
        }
    }
}
