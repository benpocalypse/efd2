#ifndef _MAP_H_
#define _MAP_H_

// Outside classes need to know how big our map is
#define MAPWIDTH	24U // The actual width of the map
#define MAPHEIGHT	18U // The actual height of the map

// Internally defined storage for our 4 tiles
#define MT_EMPTY    0U
#define MT_FLOOR    1U
#define MT_WALL_TOP 2U
#define MT_WALL_MID 3U


void MAP_InitializeMap(void);
void MAP_GenerateMap(unsigned char ucRoomType);
void MAP_DrawMyMap(void);
unsigned char MAP_TileIs(unsigned char ucX, unsigned char ucY);

#endif
