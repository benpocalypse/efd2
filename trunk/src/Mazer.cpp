// Mazer.cpp : main project file.
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


using namespace System;

#define MAPWIDTH	30U
#define FAKEMAPWIDTH 4U
#define MAPHEIGHT	27U

#define UP		0U
#define DOWN	1U
#define LEFT	2U
#define RIGHT	3U

// 240x224 (30 x 28 = 30 x 27 including statusbar)
typedef struct
{
	char ucTileIndex; //unsigned 
	unsigned char ucItem;
} sBlock;

sBlock objMap[FAKEMAPWIDTH][MAPHEIGHT];

void InitializeMap(void);
void GenerateMap(unsigned char ucRoomType);
void DrawMap(void);
unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax);
void DrawLine(signed char cStartX, signed char cStartY, signed char cEndX, signed char cEndY, signed char cTile);

int main(array<System::String ^> ^args)
{	
	ConsoleKeyInfo cki;

	/// Seed our random number generator.
	srand((unsigned int)time(NULL));
	
	for(unsigned char i = 0; i < 30; i++)
	{
		Console::Write(i/8);
		Console::Write(" ");
		Console::Write(i%8);
		Console::WriteLine();
	}
	Console::ReadKey(false);

	/*while(cki.Key != ConsoleKey::Enter)
	{
		InitializeMap();
		GenerateMap(0U);
		DrawMap();
		cki = Console::ReadKey(false);
	}*/
	
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// Initializes our map structure back to its defaults.
///////////////////////////////////////////////////////////////////////////////
void InitializeMap(void)
{
	for(unsigned char i = 0U; i < MAPWIDTH;i++)
	{
		for(unsigned char j = 0U; j < MAPHEIGHT;j++)
		{
			objMap[i][j].ucItem = 0U;
			objMap[i][j].ucTileIndex = 0U;
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

	/// Save our starting point.
	ucStart = RandomNum(0U, (MAPWIDTH - 1U)/2);
	ucRandom1 = RandomNum(MAPWIDTH/2, MAPWIDTH - 1U);
	// Top Wall
	DrawLine(ucStart, 0U, ucRandom1, 0U, '#'); 
	ucRandom2 = RandomNum(0U, (MAPHEIGHT - 1U)/2);
	// Top Right Diagonal
	DrawLine(ucRandom1, 0U, MAPWIDTH - 1U, ucRandom2, '#');
	ucRandom1 = RandomNum(MAPHEIGHT/2, MAPHEIGHT - 1U);
	// Right Wall
	DrawLine(MAPWIDTH - 1U, ucRandom2, MAPWIDTH - 1U, ucRandom1, '#');
	ucRandom2 = RandomNum((MAPWIDTH - 1U)/2, MAPWIDTH - 1U);
	// Bottom Right Diagonal
	DrawLine(ucRandom2, MAPHEIGHT - 1U, MAPWIDTH - 1U, ucRandom1, '#');
	ucRandom1 = RandomNum(0U, (MAPWIDTH - 1U)/2);
	// Bottom Wall
	DrawLine(ucRandom1, MAPHEIGHT - 1U, ucRandom2, MAPHEIGHT - 1U, '#');
	ucRandom2 = RandomNum((MAPHEIGHT - 1U)/2, MAPHEIGHT - 1U);
	// Bottom Left Diagonal
	DrawLine(ucRandom1, MAPHEIGHT - 1U, 0U, ucRandom2, '#');
	ucRandom1 = RandomNum(0U, MAPHEIGHT/2);
	// Left Wall
	DrawLine(0U, ucRandom2, 0U, ucRandom1, '#');
	// Top Left Diagonal
	DrawLine(0U, ucRandom1, ucStart, 0U, '#');

	switch(RandomNum(0,3))	
	{
		case LEFT:
			ucRandom1 = 0U;
			ucRandom2 = RandomNum(1U, MAPHEIGHT - 2U);
			break;
		case RIGHT:
			ucRandom1 = MAPWIDTH - 2U;
			ucRandom2 = RandomNum(1U, MAPHEIGHT - 2U);
			break;
		case UP:
			ucRandom1 = RandomNum(1U, MAPWIDTH - 2U);
			ucRandom2 = 0U;
			break;
		case DOWN:
			ucRandom1 = RandomNum(1U, MAPWIDTH - 2U);
			ucRandom2 = MAPHEIGHT - 1U;
			break;
		default:
			break;
	}

	objMap[ucRandom1][ucRandom2].ucTileIndex = 'D';
}

///////////////////////////////////////////////////////////////////////////////
/// Draws the current map structure.
///////////////////////////////////////////////////////////////////////////////
void DrawMap(void)
{
	for(unsigned char i = 0U; i < MAPHEIGHT; i++)
	{
		for(unsigned char j = 0U; j < MAPWIDTH; j++)
		{
			Console::Write(Convert::ToChar(objMap[j][i].ucTileIndex));
		}
		
		Console::Write("\n");
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
void DrawLine(signed char cStartX, signed char cStartY, signed char cEndX, signed char cEndY, signed char cTile) 
{
	signed char distance;
    signed char xerr = 0;
	signed char yerr = 0;
	signed char delta_x;
	signed char delta_y;
    signed char incx, incy;

    /// Compute an x and y delta.
    delta_x = cEndX - cStartX;
    delta_y = cEndY - cStartY;

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
    if(delta_y>0)
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
		objMap[cStartX][cStartY].ucTileIndex = cTile;
        
        xerr += delta_x;
        yerr += delta_y;

        if(xerr > distance)
		{
            xerr -= distance;
            cStartX += incx;
        }
        if(yerr > distance)
		{
            yerr -= distance;
            cStartY += incy;
        }
    }
}

