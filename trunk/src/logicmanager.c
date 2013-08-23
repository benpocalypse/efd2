#include "logicmanager.h"
#include "player.h"
#include "input.h"
#include "map.h"
#include "player.h"

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

// This defines a delay counter to prevent the Logic Manager from processing 
// too quickly, and having everything run too fast.
#define LOGIC_DELAY         3U

// Logic related defines
#define MAX_PLAYER_VELOCITY 3U


// These variables are used to animate the player
static unsigned char ucPlayerRunFrame;
static unsigned char ucPlayerRun[PLAYER_RUN_FRAMES];
static unsigned char bRunning;
static unsigned char bExitReached;


// Private class functions
void ProcessInput(void);
void ProcessUpdatePlayer(void);


// Internal helper functions
static COORDINATE SetPlayerStartLocation(void);
static void DrawPlayer(void);

///****************************************************************************
/// Simply initializes all of our Logic related variables to a default state.
///****************************************************************************
void LGC_Init(void)
{
    bRunning = false;
    bExitReached = false;
   
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

    // Put our player next to the entry door. This is somewhat of a hack,
    // because the map must first be generated for this to work, and that
    // is outside the responsibility of the logic manager class.
    PLY_SetCoordinate(SetPlayerStartLocation());
}


///****************************************************************************
/// This function handles, sequentially, all of the things the logic manager
/// class is tasked with handling. This includes checking input during gameply,
/// doing player collision detection, and drawing the player.
///****************************************************************************
void LGC_ManageLogic(void)
{   
    static unsigned char ucDelay = 0U;
        
    if((ucDelay == LOGIC_DELAY) && (bRunning == true))
    {
        ProcessInput();
        ProcessUpdatePlayer();
        DrawPlayer();
        ucDelay = 0U;
    }
    
    ucDelay++;
}


///****************************************************************************
/// Simple outside getter to see if the player is at the exit door.
///****************************************************************************
unsigned char LGC_ExitReached(void)
{
    return bExitReached;
}


///****************************************************************************
/// Outside setter function that will enable the processing of the logic
/// manager's functionality.
///****************************************************************************
void LGC_Start(void)
{
    bRunning = true;
}


///****************************************************************************
/// Outside setter function that will disable the processing of the logic
/// manager's functionality.
///****************************************************************************
void LGC_Stop(void)
{
    bRunning = false;
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
    
    // Now, before we "correct" the movement based on collisions, let's check to
    // see if the player has reached the exit door.
    if(GLB_CoordinateToObjectCollision(objNewCoord, objExitDoor) == true)
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
