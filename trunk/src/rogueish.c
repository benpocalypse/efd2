/*
 *  Uzebox(tm) Super Mario Demo
 *  Copyright (C) 2009  Alec Bourque
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

/*

About this program:
-------------------

This program demonstrates the latest sprites engine with video mode 3. This mode uses
'ramtiles' to display sprites. Search the forums for more info on theses.

Full screen scrolling, overlay and sprites horizontal flipping are supported. use the Screen object to set scrolling and control 
the visibility & height of the overlay.

*/

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <uzebox.h>


//external data
#include "data/fonts_8x8.pic.inc"
#include "data/tileset.inc"
#include "data/patches.inc"


#define MAPWIDTH	28U
#define MAPHEIGHT	30U
#define MODWIDTH	 3U

#define UP		0U
#define DOWN	1U
#define LEFT	2U
#define RIGHT	3U

#define EMPTY	0U
#define WALL	1U
#define DOOR	2U
#define FLOOR	3U


struct MapObject
{
	unsigned char ucX;
	unsigned char ucY;
	unsigned char ucType;
};

// 240 x 224 (30 x 28)
unsigned char	objMap[MODWIDTH][MAPHEIGHT];
struct MapObject		objEntrance;
struct MapObject		objExit;

void InitializeMap(void);
void GenerateMap(unsigned char ucRoomType);
void DrawMyMap(void);
unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax);
void DrawLine(unsigned char ucStartX, unsigned char ucStartY, unsigned char ucEndX, unsigned char ucEndY, unsigned char cTile);
void Draw(unsigned char ucX, unsigned char ucY, unsigned char ucType);
unsigned char IsA(unsigned char ucType, unsigned char ucX, unsigned char ucY);
void FloodFill(unsigned char x, unsigned char y, unsigned char ucType);


unsigned char processControls(void);

extern void SetColorBurstOffset(unsigned char value);

int main(){	
	TriggerFx(0,0,0);
	
	ClearVram();
	InitMusicPlayer(patches);
	SetMasterVolume(0x40);
	//StartSong(song_nsmb);

	//SetSpritesTileTable(mario_sprites_tileset);
		SetTileTable(roguetiles);
	SetFontTilesIndex(ROGUETILES_SIZE);
	//SetColorBurstOffset(4);

	

	//DrawMap2(0,0,map_main);
	//DrawMap2(0,VRAM_TILES_V,map_hud);
			


    //SetTile(5,5,1);
    
    InitializeMap();
	GenerateMap(0U);
	FloodFill(MAPWIDTH/2,MAPHEIGHT/2, FLOOR);
	DrawMyMap();

	
	while(1)
	{
		WaitVsync(1);
		if(processControls() == 1)
		{
		    InitializeMap();
	        GenerateMap(0U);
	        FloodFill(MAPWIDTH/2,MAPHEIGHT/2, FLOOR);
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


///////////////////////////////////////////////////////////////////////////////
/// Initializes our map structure to its defaults.
///////////////////////////////////////////////////////////////////////////////
void InitializeMap(void)
{
	for(unsigned char i = 0U; i < MAPWIDTH;i++)
	{
		for(unsigned char j = 0U; j < MAPHEIGHT;j++)
		{
			objMap[i/MODWIDTH][j] = EMPTY;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// This function takes care of actually generating the 'maze' and fills the
/// appropriate datastructures.
///////////////////////////////////////////////////////////////////////////////
void GenerateMap(unsigned char ucRoomType)
{	
	/// Room Type 1
	/// 1. Draw 4 walls of random length on each of the 4 edges of the screen.
	/// 2. Pick a random spot on one of the 4 walls to start from.
	/// 3. Pick an exit on one of the 3 other walls.
	/// 4. Draw diagonal walls to connect the 4 walls that are already drawn.
	/// 5. Sprinkle monsters and items.
	
	unsigned char ucStart = 0U;
	unsigned char ucRandom1 = 0U;
	unsigned char ucRandom2 = 0U;
	unsigned char ucEntrance = 0U;
	unsigned char ucExit = 0U;

	objEntrance.ucX = 0U;
	objEntrance.ucY = 0U;

	objEntrance.ucType = DOOR;
	objExit.ucType = DOOR;

	ucEntrance = RandomNum(0,4);

	switch(ucEntrance)
	{
		case UP:
			ucExit = RandomNum(1,4);
			break;
		case DOWN:
			if(RandomNum(0,2))
				ucExit = RandomNum(2,4);
			else
				ucExit = 0;
			break;
		case LEFT:
			ucExit = RandomNum(0,4);
			if(ucExit == 2)
				ucExit = 3;
			break;
		case RIGHT:
			ucExit = RandomNum(0,3);
			break;
	}

	/// Save our starting point.
	ucStart = RandomNum(0U, (MAPWIDTH - 1U)/2);
	ucRandom1 = RandomNum(MAPWIDTH/2, MAPWIDTH - 1U);
	/// Top Wall
	DrawLine(ucStart, 0U, ucRandom1, 0U, WALL); 
	if(ucEntrance == UP)
	{
		//Console::Write("UP\n");
		objEntrance.ucX = RandomNum(ucStart+1, ucRandom1-1);
		objEntrance.ucY = 0U;
	}	

	if(ucExit == UP)
	{
		objExit.ucX = RandomNum(ucStart+1, ucRandom1-1);
		objExit.ucY = 0U;
	}
	/// Top Right Diagonal
	ucRandom2 = RandomNum(0U, (MAPHEIGHT - 1U)/2);
	DrawLine(ucRandom1, 0U, MAPWIDTH - 1U, ucRandom2, WALL);	
	/// Right Wall
	ucRandom1 = RandomNum(MAPHEIGHT/2, MAPHEIGHT - 1U);
	DrawLine(MAPWIDTH - 1U, ucRandom2, MAPWIDTH - 1U, ucRandom1, WALL);
	if(ucEntrance == RIGHT)
	{
		//Console::Write("RIGHT\n");
		objEntrance.ucX = MAPWIDTH - 1U;
		objEntrance.ucY = RandomNum(ucRandom2+1, ucRandom1-1);
	}
	
	if(ucExit == RIGHT)
	{
		objExit.ucX = MAPWIDTH - 1U;
		objExit.ucY = RandomNum(ucRandom2+1, ucRandom1-1);
	}
	/// Bottom Right Diagonal
	ucRandom2 = RandomNum((MAPWIDTH - 1U)/2, MAPWIDTH - 1U);
	DrawLine(ucRandom2, MAPHEIGHT - 1U, MAPWIDTH - 1U, ucRandom1, WALL);	
	/// Bottom Wall
	ucRandom1 = RandomNum(0U, (MAPWIDTH - 1U)/2);
	DrawLine(ucRandom1, MAPHEIGHT - 1U, ucRandom2, MAPHEIGHT - 1U, WALL);
	if(ucEntrance == DOWN)
	{
		//Console::Write("DOWN\n");
		objEntrance.ucX = RandomNum(ucRandom1+1, ucRandom2-1);
		objEntrance.ucY = MAPHEIGHT - 1U;
	}
	
	if(ucExit == DOWN)
	{
		objExit.ucX = RandomNum(ucRandom1+1, ucRandom2-1);
		objExit.ucY = MAPHEIGHT - 1U;
	}
	/// Bottom Left Diagonal
	ucRandom2 = RandomNum((MAPHEIGHT - 1U)/2, MAPHEIGHT - 1U);
	DrawLine(ucRandom1, MAPHEIGHT - 1U, 0U, ucRandom2, WALL);	
	/// Left Wall
	ucRandom1 = RandomNum(0U, MAPHEIGHT/2);
	DrawLine(0U, ucRandom2, 0U, ucRandom1, WALL);
	if(ucEntrance == LEFT)
	{
		//Console::Write("LEFT\n");
		objEntrance.ucX = 0U;
		objEntrance.ucY = RandomNum(ucRandom1+1, ucRandom2-1);
	}
	
	if(ucExit == LEFT)
	{
		objExit.ucX = 0U;
		objExit.ucY = RandomNum(ucRandom1+1, ucRandom2-1);
	}
	/// Top Left Diagonal
	DrawLine(0U, ucRandom1, ucStart, 0U, WALL);

	//Console::Write("x: ");
	//Console::Write(objEntrance.ucX);
	//Console::Write(" y: ");
	//Console::Write(objEntrance.ucY);
	//Console::Write("\n");

	Draw(objEntrance.ucX, objEntrance.ucY, objEntrance.ucType);
	Draw(objExit.ucX, objExit.ucY, objExit.ucType);
}

///////////////////////////////////////////////////////////////////////////////
/// Draws the current map structure.
///////////////////////////////////////////////////////////////////////////////
void DrawMyMap(void)
{
	for(unsigned char i = 0U; i < MAPHEIGHT; i++)
	{
		for(unsigned char j = 0U; j < MAPWIDTH; j++)
		{
			if(IsA(WALL, j, i))
				//Console::Write("W");
				SetTile(i,j,1);
			
			if(IsA(DOOR, j, i))
				//Console::Write("D");
				SetTile(i,j,3);

			if(IsA(FLOOR, j, i))
				//Console::Write(".");
				SetTile(i,j,2);
			
			if(IsA(EMPTY, j, i))
				//Console::Write(" ");
				SetTile(i,j,0);
		}
		
		//Console::Write("\n");
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// Generates random numbers starting at and including ucMin up to and 
/// including ucMax, and returns the number.
///////////////////////////////////////////////////////////////////////////////
unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax)
{
	return (unsigned char)(ucMin + rand() % (ucMax - ucMin));
}

///////////////////////////////////////////////////////////////////////////////
/// Implements Bresenham's line drawing algorithm to efficiently draw lines
/// from one point to another in any direction.
///////////////////////////////////////////////////////////////////////////////
void DrawLine(unsigned char ucStartX, unsigned char ucStartY, unsigned char ucEndX, unsigned char ucEndY, unsigned char cTile) 
{
	signed char distance;
    signed char xerr = 0;
	signed char yerr = 0;
	signed char delta_x;
	signed char delta_y;
    signed char incx, incy;

    /// Compute an x and y delta.
    delta_x = ucEndX - ucStartX;
    delta_y = ucEndY - ucStartY;

    // Compute the direction of the incrementation in the x direction
    // An increment of 0 means either a horizontal or vertical line.
    if(delta_x > 0)
	{
		incx = 1;
	}
    else 
	{
		if(delta_x==0) 
		{
			incx = 0;
		}
		else 
		{
			incx = -1;
		}
	}    

	// Compute the direction of the incrementation in the y direction
    // An increment of 0 means either a horizontal or vertical line.
    if(delta_y > 0)
	{
		incy = 1;
	}
    else 
	{
		if(delta_y == 0)
		{
			incy = 0;
		} 
		else
		{
			incy = -1;
		}
	}

    /// Determine which direction is the greater incrementation.
    delta_x = abs(delta_x);
    delta_y = abs(delta_y);

    if(delta_x > delta_y) 
	{
		distance = delta_x;
	}
    else
	{
		distance = delta_y;
	}

    /// Now, finally draw our line.
    for(signed char t = 0; t <= (distance + 1); t++) 
	{
		Draw(ucStartX, ucStartY, cTile);
		//objMap[ucStartX/4][ucStartY] |= WALL << ((ucStartX%4)*2);
        
        xerr += delta_x;
        yerr += delta_y;

        if(xerr > distance)
		{
            xerr -= distance;
            ucStartX += incx;
        }
        if(yerr > distance)
		{
            yerr -= distance;
            ucStartY += incy;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Sets the passed tile to the passed in Type.
///////////////////////////////////////////////////////////////////////////////
void Draw(unsigned char ucX, unsigned char ucY, unsigned char ucType)
{
	unsigned char ucTemp = ~(3U << ((ucX%MODWIDTH)*2));
	ucTemp &= objMap[ucX/MODWIDTH][ucY];
	ucTemp |= ucType << ((ucX%MODWIDTH)*2);
	objMap[ucX/MODWIDTH][ucY] = ucTemp;
}

///////////////////////////////////////////////////////////////////////////////
/// Returns whether or not the specified block is of the specified type.
///////////////////////////////////////////////////////////////////////////////
unsigned char IsA(unsigned char ucType, unsigned char ucX, unsigned char ucY)
{
	return ((objMap[ucX/MODWIDTH][ucY] & (3 << ((ucX%MODWIDTH)*2))) == (ucType << ((ucX%MODWIDTH)*2)));
}

///////////////////////////////////////////////////////////////////////////////
/// Fills the specified x,y empty space with the destination tile.
///////////////////////////////////////////////////////////////////////////////
void FloodFill(unsigned char x, unsigned char y, 
				unsigned char ucType)
 {
   unsigned char ucLeft;
   unsigned char ucRight;
   unsigned char ucInLine = 1;
 
   /// Search to the left, filling along the way.
   ucLeft = ucRight = x;

   while(ucInLine == 1)
   {
		Draw(ucLeft, y, ucType);
		ucLeft--;
		ucInLine = (ucLeft < 0) ? 0 : (IsA(EMPTY, ucLeft, y));
   }

   ucLeft++;

   /// Search to the right, filling along the way.
   ucInLine = 1;

   while(ucInLine == 1)
   {
     Draw(ucRight, y, ucType);     
     ucRight++;
     ucInLine = (ucRight > MAPWIDTH-1) ? 0 : (IsA(EMPTY, ucRight, y));
   }

   ucRight--;

   /// Fill the top and bottom.
   for(unsigned char i = ucLeft; i <= ucRight; i++)
   {
     if( y > 0 && (IsA(EMPTY, i, y-1)))
	 {
         FloodFill(i, y - 1, ucType);
	 }

     if( y < MAPHEIGHT-1 && (IsA(EMPTY, i, y+1)))
	 {
         FloodFill(i, y + 1, ucType);
	 }
   }
 }

