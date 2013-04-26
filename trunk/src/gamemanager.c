#include "gamemanager.h"
#include "globals.h"
#include "logicmanager.h"
#include <avr/pgmspace.h>
#include <uzebox.h>

// Include our tile data
#include "data/tileset.inc"



void GAME_Init(void)
{
    SetTileTable(efd2_tiles);
    LGC_Init();
    LGC_Start();
    //SetSpriteVisibility(false);
	//SetFontTilesIndex(EFD2_TILES_SIZE);
}

void GAME_ManageGame(void)
{
    LGC_ManageLogic();
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

