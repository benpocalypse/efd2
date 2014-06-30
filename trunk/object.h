#ifndef _OBJECT_H_
#define _OBJECT_H_

// Object type that holds a "thing" that will appear somewhere on the map.
typedef struct
{
	unsigned char ucX;
	unsigned char ucY;
	unsigned char ucType;
} MapObject;

#endif

