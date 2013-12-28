#include "gamemanager.h"
#include "globals.h"
#include "logicmanager.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "map.h"
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <uzebox.h>

// Include our tile data
#include "data/sprites.inc"
#include "data/tileset.inc"


// Internal type definitions
typedef enum
{
    GAME_INIT,
    GAME_TITLESCREEN,
    GAME_CUTSCENE,
    GAME_PLAYLEVEL,
    GAME_GAMEOVER,
    GAME_CREDITS,
    GAME_UNKNOWN
} GAME_STATE;

// Private class variables
static GAME_STATE eCurrentState;
static GAME_STATE eRequestedState;

// Private strings for our game to be printed.
const char cLife[] PROGMEM = "Life:";
const char cKeys[] PROGMEM = "Keys:";
const char cGold[] PROGMEM = "Gold:";
const char cPressStart[] PROGMEM = "Press Start!";
const char cTitle[] PROGMEM = "Escape From";
const char cBensoft[] PROGMEM = ";2013 Bentricity";
const char cTiles[] PROGMEM = "Oryx Lo-Fi, oryxdesignlab.com";
const char cEnding1[] PROGMEM = "Sorry this game is incomplete.";
const char cEnding2[] PROGMEM = "Thanks for playing!";

// Private class functions
void GAMEi_DrawStaticHUD(void);
void GAMEi_ShowCredits(void);

// Mode handling functions
static void ModeEntry(GAME_STATE eState);
static void ModeExit(GAME_STATE eState);
static void ProcessInit(void);
static void ProcessTitlescreen(void);
static void ProcessCutscene(void);
static void ProcessPlaylevel(void);
static void ProcessGameover(void);
static void ProcessCredits(void);


void GAME_ManageGame(void)
{
    if(eRequestedState != eCurrentState)
    {
        ModeExit(eCurrentState);
        ModeEntry(eRequestedState);
        eCurrentState = eRequestedState; 
    }

    switch(eCurrentState)
    {
        case GAME_INIT:
            ProcessInit();
            break;

        case GAME_TITLESCREEN:
            ProcessTitlescreen();
            break;

        case GAME_CUTSCENE:
            ProcessCutscene();
            break;

        case GAME_PLAYLEVEL:
            ProcessPlaylevel();
            break;

        case GAME_GAMEOVER:
            ProcessGameover();
            break;

        case GAME_CREDITS:
            ProcessCredits();
            break;

        case GAME_UNKNOWN:
            break;
    }
}

static void ModeEntry(GAME_STATE eState)
{
    switch(eState)
    {
        case GAME_INIT:
            break;

        case GAME_TITLESCREEN:
            GAME_DrawTitleScreen();
            break;

        case GAME_CUTSCENE:
            break;

        case GAME_PLAYLEVEL:
            MAP_InitializeMap();
            MAP_GenerateMap(GLB_RandomNum(0,2));
            MAP_DrawMyMap();
            MAP_DrawObjects();
            GAME_DrawHud();
            LGC_Init();
            LGC_Start();
            break;

        case GAME_GAMEOVER:
            break;

        case GAME_CREDITS:
            break;

        case GAME_UNKNOWN:
            break;

        default:
            break;
    }
}


static void ModeExit(GAME_STATE eState)
{
    switch(eState)
    {
        case GAME_INIT:
            break;

        case GAME_TITLESCREEN:
            GAME_DrawBlankScreen();
            break;

        case GAME_CUTSCENE:
            break;

        case GAME_PLAYLEVEL:
            break;

        case GAME_GAMEOVER:
            break;

        case GAME_CREDITS:
            break;

        case GAME_UNKNOWN:
            break;

        default:
            break;
    }
}


static void ProcessInit(void)
{
    ClearVram();
    SetTileTable(efd2_tiles);
    SetSpritesTileTable(efd2_sprites);
    SetSpriteVisibility(true);
    PLY_Init();
    NME_Init();
    LGC_Init();
    LGC_Start();
    eRequestedState = GAME_TITLESCREEN;
}

static void ProcessTitlescreen(void)
{
    unsigned char ucTime = 0U;
    
    while(INPUT_GetButton(IN_START) != true)
    {
        ucTime++;
    }
	
	// We seed our random number here, because it relies on the randomness of
	// the player pressing start after they've been to the title screen.
	srand((unsigned)ucTime);
    if(INPUT_GetButton(IN_START) == true)
    {
        eRequestedState = GAME_PLAYLEVEL;
    }
}

static void ProcessCutscene(void)
{
}

static void ProcessPlaylevel(void)
{
    if(PLY_GetScreensPassed() == 5)
    {
        eRequestedState = GAME_CREDITS;
    }
    else
    {
        LGC_ManageLogic();
    
        if(LGC_ExitReached() == true)
        {
            MAP_InitializeMap();
	        MAP_GenerateMap(GLB_RandomNum(0,2));
	        MAP_DrawMyMap();
	        MAP_DrawObjects();

            GAME_ScreenPassed();	    
        }
    }
}

static void ProcessGameover(void)
{
}

static void ProcessCredits(void)
{
    GAMEi_ShowCredits();

    if(INPUT_GetButton(IN_START) == true)
    {
        eRequestedState = GAME_INIT;
    }
}


void GAME_Init(void)
{
    eCurrentState = GAME_UNKNOWN;
    eRequestedState = GAME_INIT;
}


void GAME_DrawBlankScreen(void)
{
    Fill(0,0, 30, 28, 0);
}


void GAME_ScreenPassed(void)
{
    LGC_Init();
    LGC_Start();
    PLY_PassedScreen();
}


void GAME_DrawHud(void)
{
    // Draw HUD text
    GLB_PrintString(2, 2, cLife); 
    GLB_PrintString(2, 3, cKeys);
    GLB_PrintString(2, 4, cGold);
    
    // Draw player hearts.
    for(unsigned char i = 0; i < PLY_GetTotalHealth(); i++)
    {
        if(i <= PLY_GetHealth())
        {
            SetTile(7+i, 2, 43);
        }
        else
        {
            SetTile(7+i, 2+i, 44);
        }
    }


    GAMEi_DrawStaticHUD();
}


///****************************************************************************
/// This function draws the part of the HUD that are static and down't change
/// include the border, and player portrait.
///****************************************************************************
void GAMEi_DrawStaticHUD(void)
{
    // Draw the vertical lines.
    for(unsigned char i=2;i<27;i++)
    {
        SetTile(1,i,HUD_VERT);
    }
        
    for(unsigned char i=2;i<27;i++)
    {
        SetTile(27,i,HUD_VERT);
    }
    
    // Now draw the corners.
    SetTile(1,  1, HUD_CORNER);
    SetTile(27, 1, HUD_CORNER);
    SetTile(1,  5, HUD_CORNER);
    SetTile(27, 5, HUD_CORNER);
    SetTile(1, 26, HUD_CORNER);
    SetTile(27,26, HUD_CORNER);
    
    // Draw the horizontal lines.
    for(unsigned char i=2;i<27;i++)
    {
        SetTile(i,1,HUD_HORIZ);
    }    
    for(unsigned char i=2;i<27;i++)
    {
        SetTile(i,5,HUD_HORIZ);
    }
    for(unsigned char i=2;i<27;i++)
    {
        SetTile(i,26,HUD_HORIZ);
    }

    // And now the hero portrait.
    DrawMap2(24,2, hero_portrait);
    
}


void GAMEi_ShowCredits(void)
{
    GAME_DrawBlankScreen();
    GLB_PrintString(0, 14, cEnding1);
    GLB_PrintString(5, 15, cEnding2);
}


///****************************************************************************
/// This function draws our titlescreen, which is completely static.
///****************************************************************************
void GAME_DrawTitleScreen(void)
{
#define VERT_START 4

    FadeIn(4, false);
    // First, a box.
    for(unsigned char i = 1; i < 29;i++)
    {
        SetTile(i, VERT_START - 2, 41);
        SetTile(i, VERT_START + 9, 41);
    }
    
    SetTile(0, VERT_START-2, 40);
    SetTile(29, VERT_START-2, 40);
    SetTile(0, VERT_START+9, 40);
    SetTile(29, VERT_START+9, 40);
    
    for(unsigned char i = VERT_START-1; i < VERT_START+9;i++)
    {
        SetTile(0, i, 42);
        SetTile(29, i, 42);
    }

    // Put up our text...
    GLB_PrintString(5, VERT_START+1, cTitle);
    GLB_PrintString(8, VERT_START+14, cPressStart);
    GLB_PrintString(0, 25, cBensoft);
    GLB_PrintString(0, 26, cTiles);
    DrawMap2(24,25, creative_commons);
    
    // And now draw the "big" stuff.
    
    // First, the D
    SetTile(1,VERT_START,32);
    SetTile(2,VERT_START,32);
    SetTile(3,VERT_START,33);
    for(unsigned char i = VERT_START+1; i < (VERT_START+5);i++)
    {
        SetTile(1,i,32);
        SetTile(3,i,32);
    }
    SetTile(1,VERT_START+5,32);
    SetTile(2,VERT_START+5,32);
    SetTile(3,VERT_START+5,35);
    
    // now the u
    SetTile(5,VERT_START+3,32);
    SetTile(7,VERT_START+3,32);
    SetTile(5,VERT_START+4,32);
    SetTile(7,VERT_START+4,32);
    SetTile(5,VERT_START+5,36);
    SetTile(6,VERT_START+5,32);
    SetTile(7,VERT_START+5,32);
    
    // now the n's
    SetTile(9,VERT_START+3, 33);
    SetTile(10,VERT_START+3, 32);
    SetTile(11,VERT_START+3, 33);
    SetTile(9,VERT_START+4,32);
    SetTile(9,VERT_START+5,32);
    SetTile(11,VERT_START+4,32);
    SetTile(11,VERT_START+5,32);
    SetTile(25,VERT_START+3, 33);
    SetTile(26,VERT_START+3, 32);
    SetTile(27,VERT_START+3, 33);
    SetTile(25,VERT_START+4,32);
    SetTile(25,VERT_START+5,32);
    SetTile(27,VERT_START+4,32);
    SetTile(27,VERT_START+5,32);
    
    // now the g
    SetTile(13, VERT_START+3, 34);
    SetTile(14, VERT_START+3, 32);
    SetTile(15, VERT_START+3, 32);
    SetTile(13, VERT_START+4, 32);
    SetTile(15, VERT_START+4, 32);
    SetTile(13, VERT_START+5, 36);
    SetTile(14, VERT_START+5, 32);
    SetTile(15, VERT_START+5, 32);
    SetTile(15, VERT_START+6, 32);
    SetTile(13, VERT_START+7, 36);
    SetTile(14, VERT_START+7, 32);
    SetTile(15, VERT_START+7, 35);
    
    // now e
    SetTile(17,VERT_START+3,32);
    SetTile(18,VERT_START+3,32);
    SetTile(19,VERT_START+3,35);
    SetTile(17,VERT_START+4,32);
    SetTile(18,VERT_START+4,33);
    SetTile(17,VERT_START+5,32);
    SetTile(18,VERT_START+5,32);
    SetTile(19,VERT_START+5,35);
    
    
    // finally, the o
    SetTile(21,VERT_START+3,34);
    SetTile(22,VERT_START+3,32);
    SetTile(23,VERT_START+3,33);
    SetTile(21,VERT_START+4,32);
    SetTile(23,VERT_START+4,32);
    SetTile(21,VERT_START+5,36);
    SetTile(22,VERT_START+5,32);
    SetTile(23,VERT_START+5,35);
}
