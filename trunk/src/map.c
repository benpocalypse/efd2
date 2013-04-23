#include "map.h"

// Map size defines.
#define MAPWIDTH	20U // The actual width of the map
#define MAPHEIGHT	20U // The actual height of the map
#define MODWIDTH	2U  // The number of tiles to be stored in a single unsigned char
#define MODSHIFT    4U  // The number of bits to shift when compressing our tiles
#define MODBITS     15U // The bits for an OR/AND mask in half of an unsigned char

// Defines that tell us which side of the map our entrance/exit is on.
#define UP		0U
#define DOWN	1U
#define LEFT	2U
#define RIGHT	3U

// Defines for our tile types.
#define EMPTY	    0U
#define FLOOR1      1U
#define FLOOR2      2U
#define FLOOR3      3U
#define DOOR	    4U
#define STAIRS      5U
#define TABLE       6U
#define SHELF       7U
#define BARREL      8U
#define WALL_UP     9U
#define WALL_DOWN   10U
#define WALL_LEFT   11U
#define WALL_TOP    12U
#define WALL_MIDDLE 13U
#define WALL_RIGHT  14U
#define WALL_SINGLE 15U


// Object type that holds an item that will appear somewhere on the map.
struct MapObject
{
	unsigned char ucX;
	unsigned char ucY;
	unsigned char ucType;
};


// 240 x 224 (30 x 28)
unsigned char objMap[(MAPWIDTH/MODWIDTH)][MAPHEIGHT];
struct MapObject    objEntrance;
struct MapObject	objExit;



// Internal function prototypes.
static void DrawLine(unsigned char ucStartX, unsigned char ucStartY, unsigned char ucEndX, unsigned char ucEndY, unsigned char cTile);
static void Draw(unsigned char ucX, unsigned char ucY, unsigned char ucType);
static unsigned char IsA(unsigned char ucType, unsigned char ucX, unsigned char ucY);
static unsigned char TileIs(unsigned char ucX, unsigned char ucY);
static void FloodFill(unsigned char x, unsigned char y, unsigned char ucType);
static unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax);


///****************************************************************************
/// Initializes our map structure to contain all empty tiles initially.
///****************************************************************************
void MAP_InitializeMap(void)
{
    for(unsigned char i = 0U; i < (MAPWIDTH/MODWIDTH);i++)
    {
        for(unsigned char j = 0U; j < MAPHEIGHT;j++)
        {
            objMap[i][j] = EMPTY;
        }
    }
}


///****************************************************************************
/// This function takes care of actually generating the 'maze' and fills it
/// with the appropriate tiles.
///****************************************************************************
void MAP_GenerateMap(unsigned char ucRoomType)
{
    if(ucRoomType == 0U)
    {
        unsigned char ucEntrace = 0U;
        unsigned char ucExit = 0U;
        unsigned char ucWidth = 0U;
        unsigned char ucHeight = 0U;

        ucWidth = RandomNum(10, MAPWIDTH);
        ucHeight = RandomNum(10, MAPHEIGHT);

        // Draw the left wall
        DrawLine((MAPWIDTH/2) - (ucWidth/2), (MAPHEIGHT/2) - (ucHeight/2), (MAPWIDTH/2) - (ucWidth/2), (MAPHEIGHT/2) + (ucHeight/2), WALL_TOP);
        // Draw the right wall
        DrawLine((MAPWIDTH/2) + (ucWidth/2), (MAPHEIGHT/2) - (ucHeight/2), (MAPWIDTH/2) + (ucWidth/2), (MAPHEIGHT/2) + (ucHeight/2), WALL_TOP);
        // Draw the top of our room
        DrawLine((MAPWIDTH/2) - (ucWidth/2), (MAPHEIGHT/2) + (ucHeight/2), (MAPWIDTH/2) + (ucWidth/2), (MAPHEIGHT/2) + (ucHeight/2), WALL_MIDDLE);
        // Draw the bottom wall
        DrawLine((MAPWIDTH/2) - (ucWidth/2) + 1, (MAPHEIGHT/2) - (ucHeight/2), (MAPWIDTH/2) + (ucWidth/2) - 1, (MAPHEIGHT/2) - (ucHeight/2), WALL_MIDDLE);

        // Finally, fill the floor with 1 of our 3 floor tile types.
        FloodFill(MAPWIDTH/2,MAPHEIGHT/2, RandomNum(1,3));
    }
    
    if(ucRoomType == 1U)
    {/*
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
	    */
    }
}


///****************************************************************************
/// Draws the current map structure.
///****************************************************************************
void MAP_DrawMyMap(void)
{
	for(unsigned char j = 0U; j < MAPHEIGHT; j++)
	{
		for(unsigned char i = 0U; i < MAPWIDTH; i++)
		{
		    SetTile(i, j, TileIs(i, j));
		}
	}	
}


///****************************************************************************
/// Generates random numbers starting at and including ucMin up to and 
/// including ucMax, and returns the number.
///****************************************************************************
unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax)
{
	return (unsigned char)(ucMin + rand() % ((ucMax - ucMin)+1));
}


///****************************************************************************
/// Implements Bresenham's line drawing algorithm to efficiently draw lines
/// from one point to another in any direction.
///****************************************************************************
void DrawLine(unsigned char ucStartX, unsigned char ucStartY, 
              unsigned char ucEndX, unsigned char ucEndY, unsigned char cTile) 
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


///****************************************************************************
/// Sets the passed tile to the passed in Type.
///****************************************************************************
void Draw(unsigned char ucX, unsigned char ucY, unsigned char ucType)
{
    unsigned char ucTemp = ~(MODBITS << ((ucX%MODWIDTH)*MODSHIFT));
    ucTemp &= objMap[ucX/MODWIDTH][ucY];
    ucTemp |= ucType << ((ucX%MODWIDTH)*MODSHIFT);
    objMap[ucX/MODWIDTH][ucY] = ucTemp;
}


///****************************************************************************
/// Returns whether or not the specified block is of the specified type.
///****************************************************************************
unsigned char IsA(unsigned char ucType, unsigned char ucX, unsigned char ucY)
{
    return ((objMap[ucX/MODWIDTH][ucY] & (MODBITS << ((ucX%MODWIDTH)*MODSHIFT))) == (ucType << ((ucX%MODWIDTH)*MODSHIFT)));
}


///****************************************************************************
/// Returns whether or not the specified block is of the specified type.
///****************************************************************************
unsigned char TileIs(unsigned char ucX, unsigned char ucY)
{
    return (objMap[ucX/MODWIDTH][ucY] & (MODBITS << ((ucX%MODWIDTH)*MODSHIFT))) >> ((ucX%MODWIDTH)*MODSHIFT);
}


///****************************************************************************
/// Fills the specified x,y empty space with the destination tile.
///****************************************************************************
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

