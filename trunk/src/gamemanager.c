#include "gamemanager.h"
#include "globals.h"
#include "logicmanager.h"
#include <avr/pgmspace.h>
#include <uzebox.h>

// Include our tile data
//#include "data/font-8x8-full.inc"
#include "data/font.inc"
#include "data/tileset.inc"

//#include "data/font.inc"
//#include "data/fonts_8x8.pic.inc"

#include "data/sprites.inc"
//#include "data/font.inc"



void GAME_Init(void)
{
    ClearVram();
    SetFontTilesIndex(EFD2_TILES_SIZE);
    SetTileTable(efd2_tiles);
    SetSpritesTileTable(efd2_sprites);
    SetSpriteVisibility(true);
    LGC_Init();
    LGC_Start();
    //SetSpriteVisibility(false);
}

void GAME_ScreenPassed(void)
{
    LGC_Init();
    LGC_Start();
}

void GAME_ManageGame(void)
{
    LGC_ManageLogic();
    
    if(LGC_ExitReached() == true)
    {
        GAME_ScreenPassed();
        MAP_InitializeMap();
	    MAP_GenerateMap(GLB_RandomNum(0,2));
	    MAP_DrawMyMap();
	    MAP_DrawObjects();	    
    }
    //SetSpriteVisibility(true);    
}

void GAME_DrawHud(void)
{
    // Draw the vertical lines.
    for(unsigned char i=1;i<28;i++)
    {
        SetTile(0,i,HUD_VERT);
    }
        
    for(unsigned char i=1;i<28;i++)
    {
        SetTile(29,i,HUD_VERT);
    }
    
    // Now draw the corners.
    SetTile(0,0, HUD_CORNER);
    SetTile(29,0, HUD_CORNER);
    SetTile(0,4, HUD_CORNER);
    SetTile(29,4, HUD_CORNER);
    SetTile(0,27, HUD_CORNER);
    SetTile(29,27, HUD_CORNER);
    
    // Draw the horizontal lines.
    for(unsigned char i=1;i<29;i++)
    {
        SetTile(i,0,HUD_HORIZ);
    }    
    for(unsigned char i=1;i<29;i++)
    {
        SetTile(i,4,HUD_HORIZ);
    }
    for(unsigned char i=1;i<29;i++)
    {
        SetTile(i,27,HUD_HORIZ);
    }
    
    // And now the hero portrait.
    DrawMap2(26,1, hero_portrait);
}

void GAME_DrawTitleScreen(void)
{
    DrawMap2(12,13, title);
}

