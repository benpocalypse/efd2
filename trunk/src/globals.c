#include "globals.h"

///****************************************************************************
/// Generates random numbers starting at and including ucMin up to and 
/// including ucMax, and returns the number.
///****************************************************************************
unsigned char RandomNum(unsigned char ucMin, unsigned char ucMax)
{
	return (unsigned char)(ucMin + rand() % ((ucMax - ucMin)+1));
}

