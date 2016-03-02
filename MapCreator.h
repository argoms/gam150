/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  MapCreator.c
Created       :  3/1/16
Description   :  Provides forward declarations for map creator functions
ChangeLog
-

DON'T READ
NOTES       REMEMBER TO CONVERT YOUR X/Y to the iso X/Y
currently using matrix[y,x]
convert   to    matrix[x,y]


**************************************************************************************************/

#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H

#include <stdio.h>
#include <stdlib.h>

typedef int bool;

/**************************************************************************************************
Function      : MapCreator_ToFile
Description   : Generates map data and sends it to a file
Input         : targetFile is where the map data output will go.
                width is the width of the map,
                height is the height of the map,
                wallDensity is the ratio of walls to open spaces, range from 0 to 1.
                  0 means don't create walls, 1 means cover the entire map with walls.
Output        : Returns true (1) if map data generation is successful, false (0) if unsuccessful.
**************************************************************************************************/
bool MapCreator_ToFile(char *targetFile, int width, int height, float wallDensity);

#endif // !MAP_CREATOR_H
