/*
 *  Uzebox(tm) Escape from Dungeon 2
 *  Copyright (C) 2013 Ben Foote
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Uzebox is a reserved trade mark
*/

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <uzebox.h>

#include "map.h"
#include "input.h"


//external data
//#include "data/fonts_8x8.pic.inc"
#include "data/tileset.inc"
//#include "data/patches.inc"


unsigned char processControls(void);

int main(){
	//TriggerFx(0,0,0);
	//InitMusicPlayer(patches);
	//SetMasterVolume(0x40);
	//StartSong(song_nsmb);

	//SetSpritesTileTable(mario_sprites_tileset);
	
	unsigned char ucTime = 0;
	
	ClearVram();
	SetTileTable(efd2_tiles);
	SetFontTilesIndex(EFD2_TILES_SIZE);
    
    MAP_InitializeMap();
	MAP_GenerateMap(0U);
	MAP_DrawMyMap();

	
	while(1)
	{
	    ucTime++;
		WaitVsync(1);
		if(INPUT_GetButton() == START)
		{
    		srand((unsigned)ucTime);
		    MAP_InitializeMap();
   	        MAP_DrawMyMap();
	        MAP_GenerateMap(0U);
	        MAP_DrawMyMap();
		}
	}		
	
}

