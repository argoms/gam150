/**************************************************************************************************
Project       :  EPOCH
File          :  MapCreator.h
Author        :  Matt Chasengnou
Created       :  3/1/16
Description   :  Provides forward declarations for map creator functions
ChangeLog

-3/2/16       :  Added generate to map direct pipelining

DON'T READ
NOTES       REMEMBER TO CONVERT YOUR X/Y to the iso X/Y
currently using matrix[y,x]
convert   to    matrix[x,y]

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
**************************************************************************************************/

#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H

#include <stdio.h>
#include <stdlib.h>

#include "Isometric.h"

typedef int bool;

/**************************************************************************************************
Function      : MapCreator_ToMap
Description   : Generates map data and sends it to an iso map.
Input         : targetMap is where the map data output will go.
wallDensity is the ratio of walls to open spaces, range from 0 to 1.
0 means don't create walls, 1 means cover the entire map with walls.
Output        : Returns true (1) if map data generation is successful, false (0) if unsuccessful.
**************************************************************************************************/
bool MapCreator_ToMap(IsoMap *targetMap, float wallDensity);

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
