#include "map.h"
#include "globals.h"
#include <stdbool.h>
#include <uzebox.h>

// Map compresipon size defines
#define MODWIDTH	4U  // The number of tiles to be stored in a single unsigned char
#define MODSHIFT    2U  // The number of bits to shift when compressing our tiles
#define MODBITS     3U  // The bits for an OR/AND mask in half of an unsigned char

// Array size defines
#define MAX_OBSTACLES   5U
#define MAX_ITEMS       5U

// Our map data storage array
static unsigned char objMap[(MAPWIDTH/MODWIDTH)][MAPHEIGHT];

// Our entry and exit door objects.
static MapObject objEntrance;
static MapObject objExit;
static MapObject objObstacles[MAX_OBSTACLES];
static MapObject objItems[MAX_ITEMS];

// Private function prototypes
static void DrawLine(unsigned char ucStartX, unsigned char ucStartY, unsigned char ucEndX, unsigned char ucEndY, unsigned char cTile);
static void Draw(unsigned char ucX, unsigned char ucY, unsigned char ucType);
static void FloodFill(unsigned char x, unsigned char y, unsigned char ucType);
static void AddDoor(unsigned char ucDirection, bool bEntrance);
static void AddObstacles(unsigned char ucNumber);
static void AddItems(unsigned char ucNumber);


///****************************************************************************
/// Initializes our map structure to contain all empty tiles initially.
///****************************************************************************
void MAP_InitializeMap(void)
{
    // Empty our map...
    for(unsigned char i = 0U; i < (MAPWIDTH/MODWIDTH);i++)
    {
        for(unsigned char j = 0U; j < MAPHEIGHT;j++)
        {
            objMap[i][j] = MT_EMPTY;
        }
    }

    // And our obstacles...
    for(unsigned char i = 0U; i < MAX_OBSTACLES; i++)
    {
        objObstacles[i].ucType = EMPTY;
        objObstacles[i].ucX = 0U;
        objObstacles[i].ucY = 0U;
    }

    // and finally our items.
    for(unsigned char i = 0U; i < MAX_ITEMS; i++)
    {
        objItems[i].ucType = EMPTY;
        objItems[i].ucX = 0U;
        objitems[i].ucY = 0U;
    }
}


///****************************************************************************
/// This function takes care of actually generating the 'maze' and fills it
/// with the appropriate tiles. The argument just tells us which type of 
/// random room we're going to generate.
///****************************************************************************
void MAP_GenerateMap(unsigned char ucRoomType)
{
    // 2 randomly placed rooms connected by tunnels.
    if(ucRoomType == 0U)
    {
        unsigned char ucWidth = 0U;
        unsigned char ucWidth2 = 0U;
        unsigned char ucHeight = 0U;
        unsigned char ucHeight2 = 0U;
        
        unsigned char ucRoomOnePosition = GLB_RandomNum(0,1);
        unsigned char ucRoomTwoPosition = GLB_RandomNum(0,1);

        // First, decide the room width/height.
        ucWidth = GLB_RandomNum(9, 14);
        ucHeight = GLB_RandomNum(10, 15);
        
        // Draw room 1 first.
        // Draw the left, right, top, and then bottom walls.
        DrawLine(0, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)),
                 0, ucHeight + (ucRoomOnePosition*(MAPHEIGHT-ucHeight-1)), MT_WALL_TOP);
        DrawLine(ucWidth, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)),
                 ucWidth, ucHeight + (ucRoomOnePosition*(MAPHEIGHT-ucHeight-1)), MT_WALL_TOP);
        DrawLine(1, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)), 
                 ucWidth - 1, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)), MT_WALL_MID);
        DrawLine(0, ucHeight + (ucRoomOnePosition*(MAPHEIGHT-ucHeight-1)),
                 ucWidth, ucHeight + (ucRoomOnePosition*(MAPHEIGHT-ucHeight-1)), MT_WALL_MID);

        // Finally, fill the floor of room 1 with 1 of our 3 floor tile types.
        FloodFill(ucWidth/2,(ucRoomOnePosition*(MAPHEIGHT-(ucHeight/2)))+1, MT_FLOOR);


        // Now draw room 2.     
        // First, decide the room width/height.
        ucWidth2 = GLB_RandomNum(6, 21-ucWidth);
        ucHeight2 = GLB_RandomNum(9, 16);

        // Draw the left, right, top, and then bottom walls.
        DrawLine(MAPWIDTH-ucWidth2, (ucRoomTwoPosition*(MAPHEIGHT-ucHeight2)),
                 MAPWIDTH-ucWidth2, ucHeight2 + (ucRoomTwoPosition*(MAPHEIGHT-ucHeight2-1)), MT_WALL_TOP);
        DrawLine(MAPWIDTH-1, (ucRoomTwoPosition*(MAPHEIGHT-ucHeight2)),
                 MAPWIDTH-1, ucHeight2 + (ucRoomTwoPosition*(MAPHEIGHT-ucHeight2-1)), MT_WALL_TOP);
        DrawLine(MAPWIDTH-ucWidth2+1, (ucRoomTwoPosition*(MAPHEIGHT-ucHeight2)), 
                 MAPWIDTH-2, (ucRoomTwoPosition*(MAPHEIGHT-ucHeight2)), MT_WALL_MID);
        DrawLine(MAPWIDTH-ucWidth2, ucHeight2 + (ucRoomTwoPosition*(MAPHEIGHT-ucHeight2-1)),
                 MAPWIDTH-1, ucHeight2 + (ucRoomTwoPosition*(MAPHEIGHT-ucHeight2-1)), MT_WALL_MID);

        // Finally, fill the floor of room 2 with 1 of our 3 floor tile types.
        FloodFill(MAPWIDTH-(ucWidth2/2),(ucRoomTwoPosition*(MAPHEIGHT-(ucHeight2/2)))+1, MT_FLOOR);

        // Now lets connect the two rooms with a hallway...

        // If both rooms are on the same level..
        if(ucRoomOnePosition == ucRoomTwoPosition)
        {//...then lets just run a straight hallway between them.
            // Draw the top.
            DrawLine(ucWidth, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)) + (ucHeight/2) - 1,
                     MAPWIDTH-ucWidth2, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)) + (ucHeight/2) - 1, MT_WALL_MID);
                     
            // Draw the floor.
            DrawLine(ucWidth, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)) + (ucHeight/2),
                     MAPWIDTH-ucWidth2, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)) + (ucHeight/2), MT_FLOOR);
            
            // Now draw the bottom.
            DrawLine(ucWidth+1, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)) + (ucHeight/2) + 1,
                     MAPWIDTH-ucWidth2-1, (ucRoomOnePosition*(MAPHEIGHT-ucHeight)) + (ucHeight/2) + 1, MT_WALL_MID);

            AddDoor(UP, true);
            AddDoor(DOWN, false);
        }
        else
        {//...we need to make a connecting hallway with a bend.
            // If room 1 is on the top...
            if(ucRoomOnePosition == 0)
            {// Then draw a right elbow.
                DrawLine((ucWidth/2)-1, ucHeight, (ucWidth/2)-1, MAPHEIGHT-2, MT_WALL_TOP);
                DrawLine((ucWidth/2)-1, MAPHEIGHT-1, MAPWIDTH-ucWidth2, MAPHEIGHT-1, MT_WALL_MID);
                
                DrawLine((ucWidth/2), ucHeight, (ucWidth/2)+1, MAPHEIGHT-2, MT_FLOOR);
                DrawLine((ucWidth/2), MAPHEIGHT-2, MAPWIDTH-ucWidth2, MAPHEIGHT-2, MT_FLOOR);
                
                DrawLine((ucWidth/2)+1, ucHeight, (ucWidth/2)+1, MAPHEIGHT-3, MT_WALL_TOP);
                DrawLine((ucWidth/2)+1, MAPHEIGHT-3, MAPWIDTH-ucWidth2, MAPHEIGHT-3, MT_WALL_MID);

                AddDoor(UP, true);
                AddDoor(RIGHT, false);
            }
            else
            {//...finally, room 2 must be on top.
                DrawLine((ucWidth/2)-1, MAPHEIGHT-ucHeight-1, (ucWidth/2)-1, 0, MT_WALL_TOP);
                DrawLine((ucWidth/2), 0, MAPWIDTH-ucWidth2, 0, MT_WALL_MID);
                
                DrawLine((ucWidth/2), MAPHEIGHT-ucHeight, (ucWidth/2), 1, MT_FLOOR);
                DrawLine((ucWidth/2), 1, MAPWIDTH-ucWidth2, 1, MT_FLOOR);
                
                DrawLine((ucWidth/2)+1, MAPHEIGHT-ucHeight-1, (ucWidth/2)+1, 2, MT_WALL_TOP);
                DrawLine((ucWidth/2)+2, 2, MAPWIDTH-ucWidth2-1, 2, MT_WALL_MID);

                AddDoor(RIGHT, true);
                AddDoor(LEFT, false);
            }
        }

        return;
    }

    // One big room, with multiple obstacles.
    if(ucRoomType == 1U)
    {
        unsigned char ucWidth = GLB_RandomNum(18, MAPWIDTH-1);
        unsigned char ucHeight = GLB_RandomNum(14, MAPHEIGHT-1);
        unsigned char ucDoor = GLB_RandomNum(UP, RIGHT);
        
        // Draw the walls.
        DrawLine((MAPWIDTH/2) - (ucWidth/2), (MAPHEIGHT/2) - (ucHeight/2), 
                 (MAPWIDTH/2) - (ucWidth/2), (MAPHEIGHT/2) + (ucHeight/2), MT_WALL_TOP);
        DrawLine((MAPWIDTH/2) + (ucWidth/2), (MAPHEIGHT/2) - (ucHeight/2), 
                 (MAPWIDTH/2) + (ucWidth/2), (MAPHEIGHT/2) + (ucHeight/2), MT_WALL_TOP);
        DrawLine((MAPWIDTH/2) - (ucWidth/2), (MAPHEIGHT/2) + (ucHeight/2), 
                 (MAPWIDTH/2) + (ucWidth/2), (MAPHEIGHT/2) + (ucHeight/2), MT_WALL_MID);
        DrawLine((MAPWIDTH/2) - (ucWidth/2) + 1, (MAPHEIGHT/2) - (ucHeight/2), 
                 (MAPWIDTH/2) + (ucWidth/2) - 1, (MAPHEIGHT/2) - (ucHeight/2), MT_WALL_MID);

        // Fill the floor.
        FloodFill(MAPWIDTH/2,MAPHEIGHT/2, MT_FLOOR);

        // And add the doors.
        unsigned char ucDoor2 = GLB_RandomNum(UP,RIGHT);
        
        while(ucDoor == ucDoor2)
        {
            ucDoor2 = GLB_RandomNum(UP, RIGHT);
        }
        
        AddDoor(ucDoor, true);
        AddDoor(ucDoor2, false);
    }

    // A long skinny room, with doors at opposite ends.
    if(ucRoomType == 2U)
    {
        if(GLB_RandomNum(0,1) == 0)
        {            
            unsigned char ucHeight = GLB_RandomNum(8, 11);
            // Draw the walls.
            DrawLine(0, (MAPHEIGHT/2) - (ucHeight/2), 
                     0, (MAPHEIGHT/2) + (ucHeight/2), MT_WALL_TOP);
            DrawLine(MAPWIDTH-1, (MAPHEIGHT/2) - (ucHeight/2), 
                     MAPWIDTH-1, (MAPHEIGHT/2) + (ucHeight/2), MT_WALL_TOP);
            DrawLine(0, (MAPHEIGHT/2) + (ucHeight/2), 
                     MAPWIDTH-1, (MAPHEIGHT/2) + (ucHeight/2), MT_WALL_MID);
            DrawLine(1, (MAPHEIGHT/2) - (ucHeight/2), 
                     MAPWIDTH-2, (MAPHEIGHT/2) - (ucHeight/2), MT_WALL_MID);

            // Fill the floor.
            FloodFill(MAPWIDTH/2,MAPHEIGHT/2, MT_FLOOR);

            // And add the doors.
            AddDoor(LEFT, true);
            AddDoor(RIGHT, false);
        }
        else
        {
            unsigned char ucWidth = GLB_RandomNum(8, 11);
            // Draw the walls.
            DrawLine((MAPWIDTH/2) - (ucWidth/2), 0,
                     (MAPWIDTH/2) - (ucWidth/2), MAPHEIGHT-2, MT_WALL_TOP);
            DrawLine((MAPWIDTH/2) + (ucWidth/2), 0,
                     (MAPWIDTH/2) + (ucWidth/2), MAPHEIGHT-2, MT_WALL_TOP);
            DrawLine((MAPWIDTH/2) - (ucWidth/2)+1, 0, 
                     (MAPWIDTH/2) + (ucWidth/2)-1, 0, MT_WALL_MID);
            DrawLine((MAPWIDTH/2) - (ucWidth/2), MAPHEIGHT-1, 
                     (MAPWIDTH/2) + (ucWidth/2), MAPHEIGHT-1, MT_WALL_MID);

            // Fill the floor.
            FloodFill(MAPWIDTH/2,MAPHEIGHT/2, MT_FLOOR);

            // And add the doors.
            AddDoor(UP, true);
            AddDoor(DOWN, false);
        }
    }


    // Now pepper our room with 'stuff.'
    AddObstacles(MAX_OBSTACLES);
}


///****************************************************************************
/// Draws the current map structure in the center of the screen to allow room
/// for the HUD to be drawn as well.
///****************************************************************************
void MAP_DrawMyMap(void)
{
    // Each time we draw a map, randomly pick one of the 3 types of floors
    // to add a little variety to our maps.
    unsigned char ucFloor = GLB_RandomNum(1,3);
    
	for(unsigned char j = 0U; j < MAPHEIGHT; j++)
	{
		for(unsigned char i = 0U; i < MAPWIDTH; i++)
		{
		    unsigned char ucTile = MAP_TileIs(i,j);
		    
		    switch(ucTile)
		    {
		        case MT_FLOOR:
    		        SetTile(i+MAP_X_OFFSET, j+MAP_Y_OFFSET, ucFloor);
		            break;
		        case MT_WALL_TOP:
		            SetTile(i+MAP_X_OFFSET, j+MAP_Y_OFFSET, WALL_TOP);
		            break;
		        case MT_WALL_MID:
		            SetTile(i+MAP_X_OFFSET, j+MAP_Y_OFFSET, WALL_MIDDLE);
		            break;
		        default:
		            SetTile(i+MAP_X_OFFSET, j+MAP_Y_OFFSET, EMPTY);
		            break;
		            
		    }
		}
	}	
}


///****************************************************************************
/// This function draws all non-tile objects in the map class on the map.
/// FIXME - Is this really how we should handle this?
///****************************************************************************
void MAP_DrawObjects(void)
{
    SetTile(objEntrance.ucX+MAP_X_OFFSET, objEntrance.ucY+MAP_Y_OFFSET, objEntrance.ucType);
    SetTile(objExit.ucX+MAP_X_OFFSET, objExit.ucY+MAP_Y_OFFSET, objExit.ucType);    

    for(unsigned char i=0; i < MAX_OBSTACLES; i++)
    {
        if(objObstacles[i].ucType != 0)
        {
            SetTile(objObstacles[i].ucX+MAP_X_OFFSET, objObstacles[i].ucY+MAP_Y_OFFSET, objObstacles[i].ucType);
        }
    }
}


///****************************************************************************
/// Returns the location of either the entrance or exit of the map.
///****************************************************************************
MapObject MAP_GetDoor(unsigned char bEntrance)
{
    if(bEntrance == MT_ENTRANCE)
    {
        return objEntrance;
    }
    else
    {
        return objExit;
    }
}


///****************************************************************************
/// This function allows outside classes to see which tiles are at which X/Y
/// coordinate to allow for things like collision detection.
///****************************************************************************
unsigned char MAP_TileIs(unsigned char ucX, unsigned char ucY)
{
    if((ucX < MAPWIDTH) && (ucY < MAPHEIGHT))
    {
        return (objMap[ucX/MODWIDTH][ucY] & (MODBITS << ((ucX%MODWIDTH)*MODSHIFT))) >> ((ucX%MODWIDTH)*MODSHIFT);
    }
    else
    {
        return 0U;
    }   
}


///****************************************************************************
/// This function allows outside classes to see which objects are at which X/Y
/// coordinate to allow for things like collision detection.
///****************************************************************************
unsigned char MAP_ObjectIs(unsigned char ucX, unsigned char ucY)
{
    for(unsigned char i = 0U; i < MAX_OBSTACLES; i++)
    {
        if((objObstacles[i].ucX == ucX) && (objObstacles[i].ucY == ucY))
        {
            return objObstacles[i].ucType;
        }
    }

    for(unsigned char i = 0U; i < MAX_ITEMS; i++)
    {
        if((objItems[i].ucX == ucX) && (objItems[i].ucY == ucY))
        {
            return objItems[i].ucType;
        }
    }
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
/// Sets the tile at x/y to the passed in Type. This uses some tricky math
/// to work with our compressed map structure.
///****************************************************************************
void Draw(unsigned char ucX, unsigned char ucY, unsigned char ucType)
{
    unsigned char ucTemp = ~(MODBITS << ((ucX%MODWIDTH)*MODSHIFT));
    ucTemp &= objMap[ucX/MODWIDTH][ucY];
    ucTemp |= ucType << ((ucX%MODWIDTH)*MODSHIFT);
    objMap[ucX/MODWIDTH][ucY] = ucTemp;
}


///****************************************************************************
/// Fills the specified x,y empty space with the destination tile until a 
/// non-empty tile is encountered.
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
		ucInLine = (ucLeft < 0) ? 0 : (MAP_TileIs(ucLeft, y) == MT_EMPTY);//(IsA(EMPTY, ucLeft, y));
   }

   ucLeft++;

   /// Search to the right, filling along the way.
   ucInLine = 1;

   while(ucInLine == 1)
   {
     Draw(ucRight, y, ucType);     
     ucRight++;
     ucInLine = (ucRight > MAPWIDTH-1) ? 0 : (MAP_TileIs(ucRight,y) == MT_EMPTY);//(IsA(EMPTY, ucRight, y));
   }

   ucRight--;

   /// Fill the top and bottom.
   for(unsigned char i = ucLeft; i <= ucRight; i++)
   {
     if( y > 0 && (MAP_TileIs(i, y-1) == MT_EMPTY))//(IsA(EMPTY, i, y-1)))
	 {
         FloodFill(i, y - 1, ucType);
	 }

     if( y < MAPHEIGHT-1 && (MAP_TileIs(i, y+1) == MT_EMPTY))//(IsA(EMPTY, i, y+1)))
	 {
         FloodFill(i, y + 1, ucType);
	 }
    }
}


///****************************************************************************
/// This function adds doors to our map based on the type of map we've 
/// generated. Essentially, we want the entrance and exit to a room to be
/// fairly far apart to force the player to navigate our maze.
///****************************************************************************
void AddDoor(unsigned char ucDirection, bool bEntrance)
{
    // If our entry door is to be on the top, scan left to right, top to bottom for a spot
    // to add it.
    if(ucDirection == UP)
    {
        for(unsigned char j = 0; j < MAPHEIGHT; j++)
        {
            for(unsigned char i = 0; i < MAPWIDTH; i++)
            {
                if((MAP_TileIs(i, j) == MT_WALL_TOP) || (MAP_TileIs(i, j) == MT_WALL_MID))
                {
                    if(bEntrance == true)
                    {
                        objEntrance.ucX = i+GLB_RandomNum(3, 7);
                        objEntrance.ucY = j;
                        objEntrance.ucType = DOOR;
                    }
                    else
                    {
                        objExit.ucX = i+GLB_RandomNum(3,7);
                        objExit.ucY = j;
                        objExit.ucType = DOOR;
                    }
                    
                    return;
                }                
            }            
        }
    }

    // If our door is to be on the bottom, start in the lower right hand corner, and count
    // right to left, bottom to top.
    if(ucDirection == DOWN)
    {
        for(unsigned char j = (MAPHEIGHT-1); j > 0; j--)
        {
            for(unsigned char i = (MAPWIDTH-1); i > 0; i--)
            {
                if((MAP_TileIs(i, j) == MT_WALL_TOP) || (MAP_TileIs(i, j) == MT_WALL_MID))
                {
                    if(bEntrance == true)
                    {
                        objEntrance.ucX = i-GLB_RandomNum(1,4);
                        objEntrance.ucY = j;
                        objEntrance.ucType = DOOR;
                    }
                    else
                    {
                        objExit.ucX = i-GLB_RandomNum(1,4);
                        objExit.ucY = j;
                        objExit.ucType = DOOR;
                    }

                    return;
                }
            }
        }
    }
    
    // If our door is to be on the right, start in the lower right hand corner, and count
    // bottom to top, right to left.
    if(ucDirection == RIGHT)
    {
        for(unsigned char i = (MAPWIDTH-1); i > 0; i--)
        {
            for(unsigned char j = (MAPHEIGHT-1); j > 0; j--)
            {
                if((MAP_TileIs(i, j) == MT_WALL_TOP) || (MAP_TileIs(i, j) == MT_WALL_MID))
                {
                    if(bEntrance == true)
                    {
                        objEntrance.ucX = i;
                        objEntrance.ucY = j-GLB_RandomNum(3,6);
                        objEntrance.ucType = DOOR;
                    }
                    else
                    {
                        objExit.ucX = i;
                        objExit.ucY = j-GLB_RandomNum(3,6);
                        objExit.ucType = DOOR;
                    }
                    
                    return;
                }
            }
        }
    }

    // If our door is to be on the left, start in the lower left hand corner, and count
    // bottom to top, left to right.
    if(ucDirection == LEFT)
    {
        for(unsigned char i = 0; i < MAPWIDTH; i++)
        {
            for(unsigned char j = (MAPHEIGHT-1); j > 0; j--)
            {
                if((MAP_TileIs(i, j) == WALL_TOP) || (MAP_TileIs(i, j) == MT_WALL_MID))
                {
                    if(bEntrance == true)
                    {
                        objEntrance.ucX = i;
                        objEntrance.ucY = j-GLB_RandomNum(2,4);
                        objEntrance.ucType = DOOR;
                    }
                    else
                    {
                        objExit.ucX = i;
                        objExit.ucY = j-GLB_RandomNum(2,4);
                        objExit.ucType = DOOR;
                    }
                
                    return;
                }
            }
        }
    }
}


///****************************************************************************
/// This function adds obstacles to our map based on how many we want. The idea
/// is that we randomly sprinkle obstacles around our map.
/// FIXME - this funtion needs to be made more robust to allow for more functionality.
///****************************************************************************
void AddObstacles(unsigned char ucNumber)
{
    // First, clear out our old obstacles.
    for(unsigned char i = 0; i < MAX_OBSTACLES; i++)
    {
        objObstacles[i].ucX = 0U;
        objObstacles[i].ucY = 0U;
        objObstacles[i].ucType = EMPTY;
    }

    // Now, populate a new set of random obstacles, up to the number requested.
    if(ucNumber < MAX_OBSTACLES)
    {
        for(unsigned char i = 0; i < ucNumber; i++)
        {
            objObstacles[i].ucX = GLB_RandomNum(0, MAPWIDTH);
            objObstacles[i].ucY = GLB_RandomNum(0, MAPHEIGHT);

            while(MAP_TileIs(objObstacles[i].ucX, objObstacles[i].ucY) != MT_FLOOR)
            {
                objObstacles[i].ucX = GLB_RandomNum(0, MAPWIDTH);
                objObstacles[i].ucY = GLB_RandomNum(0, MAPHEIGHT);
            }

            objObstacles[i].ucType = WALL_SINGLE;
        }
    }
}


///****************************************************************************
/// This function adds items to our map based on how many we want. The idea
/// is that we randomly sprinkle items around our map.
/// FIXME - This function doesn't work...items are 1's hot notation.
///****************************************************************************
void AddItems(unsigned char ucNumber)
{
    // First, clear out our old obstacles.
    for(unsigned char i = 0; i < MAX_OBSTACLES; i++)
    {
        objItems[i].ucX = 0U;
        objItems[i].ucY = 0U;
        objItems[i].ucType = EMPTY;
    }

    // Now, populate a new set of random obstacles, up to the number requested.
    if(ucNumber < MAX_OBSTACLES)
    {
        for(unsigned char i = 0; i < ucNumber; i++)
        {
            objItems[i].ucX = GLB_RandomNum(0, MAPWIDTH);
            objItems[i].ucY = GLB_RandomNum(0, MAPHEIGHT);

            while(MAP_TileIs(objItems[i].ucX, objItems[i].ucY) != MT_FLOOR)
            {
                objItems[i].ucX = GLB_RandomNum(0, MAPWIDTH);
                objItems[i].ucY = GLB_RandomNum(0, MAPHEIGHT);
            }

            objItems[i].ucType = EMPTY;
        }
    }
}
