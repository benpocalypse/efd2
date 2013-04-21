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
	
	ClearVram();
	SetTileTable(efd2_tiles);
	SetFontTilesIndex(EFD2_TILES_SIZE);
    
    InitializeMap();
	GenerateMap(0U);
//	FloodFill(MAPWIDTH/2,MAPHEIGHT/2, FLOOR);
	DrawMyMap();

	
	while(1)
	{
		WaitVsync(1);
		if(processControls() == 1)
		{
		    InitializeMap();
   	        DrawMyMap();
	        GenerateMap(0U);
	        DrawMyMap();
		}
	}		
	
}

unsigned char processControls(void){
	unsigned int joy=ReadJoypad(0);


	if(joy&BTN_A){
	
	}else if(joy&BTN_X){

	}else if(joy&BTN_Y){

	}else if(joy&BTN_SR){

	}else if(joy&BTN_SL){

	}else if(joy&BTN_UP){


	}else if(joy&BTN_DOWN){

	
	}
	
	if(joy&BTN_LEFT){
		
		
	}else if(joy&BTN_RIGHT){
	
	}

	if(joy&BTN_START){
        return 1;
	}

	if(joy&BTN_SELECT){

	}
	return 0;
}
