/*
 *  Uzebox(tm) - Escape from Dungeon 2
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

#include "gamemanager.h"
//#include "map.h"
#include "input.h"
#include "globals.h"


// External data
//#include "data/tileset.inc"
//#include "data/patches.inc"


int main()
{
	//TriggerFx(0,0,0);
	//InitMusicPlayer(patches);
	//SetMasterVolume(0x40);
	//StartSong(song_nsmb);
	
	ClearVram();
	
	GAME_Init();

	while(1)
	{	    
		WaitVsync(1);
        GAME_ManageGame();
/*
        // FIXME - This is here just in case you get stuck.
		if(INPUT_GetButton(IN_START) == true)
		{
    		
		    MAP_InitializeMap();
   	        MAP_DrawMyMap();   	        
	        MAP_GenerateMap(GLB_RandomNum(0,2));
	        MAP_DrawMyMap();
	        MAP_DrawObjects();
	        GAME_ScreenPassed();
		}
*/		
	}		
	
}

