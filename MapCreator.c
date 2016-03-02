/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  MapCreator.c
Created       :  3/1/16
Description   :  Provides functionality for generating tilemap info
ChangeLog

-3/2/16       :  Renamed functions to more consistent convention


NOTES       REMEMBER TO CONVERT YOUR X/Y to the iso X/Y
            currently using matrix[y,x]
            convert   to    matrix[x,y]


**************************************************************************************************/
/*-------------------------------------------------------------------------------------------------
INCLUDES
-------------------------------------------------------------------------------------------------*/

#include "Isometric.h"    /* Iso struct, etc */
#include "MyRandom.h"     /* For random number generation */
#include "conversions.h"  /* For numerical conversions */


#include <stdio.h>        /* printing to file */
#include <stdlib.h>       /* mem allocation */


#include <windows.h>  /* shows error messages */
/*-------------------------------------------------------------------------------------------------
END INCLUDES
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
DEFINES/ENUMS
-------------------------------------------------------------------------------------------------*/

#define true  1
#define false 0

#define PATH_MAX_LEN    255
#define MAP_DIRECTORY   "./maps/"

#define MAX_TILE_GROUPS 1024
#define MAX_TILE_CONNECTIONS 8

typedef int bool;

/* Positions representing connected tiles */
enum TILE_CONNECTIONS
{
  TILE_TOP = 0,
  TILE_LEFT,
  TILE_BOTTOM,
  TILE_RIGHT,

  TILE_TOP_LEFT,
  TILE_TOP_RIGHT,
  TILE_BOTTOM_LEFT,
  TILE_BOTTOM_RIGHT

  //TILE_TOP            = 1 << 0,
  //TILE_LEFT           = 1 << 1,
  //TILE_BOTTOM         = 1 << 2,
  //TILE_RIGHT          = 1 << 3,

  //TILE_TOP_LEFT       = 1 << 4,
  //TILE_TOP_RIGHT      = 1 << 5,
  //TILE_BOTTOM_LEFT    = 1 << 6,
  //TILE_BOTTOM_RIGHT   = 1 << 7
};
/*-------------------------------------------------------------------------------------------------
END DEFINES/ENUMS
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
STRUCTS
-------------------------------------------------------------------------------------------------*/

typedef struct TileGen_Tile TileGen_Tile;
typedef struct TileGen_Map  TileGen_Map;

typedef struct TILE_GROUP_MANAGER TILE_GROUP_MANAGER;

/* Stores information about the tile used in the map generator */
struct TileGen_Tile
{
  TileGen_Map   *mapOwner;                            /* The map this tile belongs to */
  bool          isWall;                               /* Whether this tile is a wall or not */
  bool          connectedToEdge;                      /* Whether this tile is connected to the edge or not */
  unsigned int  tileGroup;                            /* Which tilegroup this tile is in */
  TileGen_Tile  *connections[MAX_TILE_CONNECTIONS];   /* Other tiles that are connected to this one */
  int           x;                                    /* x position of tile */
  int           y;                                    /* y position of tile */
};

/* Stores information about the map used in the map generator */
struct TileGen_Map
{
  TileGen_Tile  **map;              /* Array containing the tile info */
  int           width;              /* The width of the map */
  int           height;             /* The height of the map */
};

/*
  Struct for the look-up table for the tile groups.
  Each index in the table corresponds to the group number.
  The contents of each index represents the number of tiles in that group.
  Example: if tileGroups[3] == 4, then tileGroup 3 has 4 tiles in it.
*/
struct TILE_GROUP_MANAGER
{
  unsigned int tileGroups[MAX_TILE_GROUPS];   /* The table for the tile groups */
  unsigned int numGroups;                     /* The number of active tile groups */
};
/*-------------------------------------------------------------------------------------------------
END STRUCTS
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
SWITCHES
-------------------------------------------------------------------------------------------------*/

bool SHOW_ERRORS = true;    /* Whether or not to display error messages */
/*-------------------------------------------------------------------------------------------------
END SWItCHES
-------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------
STATIC VARIABLES
-------------------------------------------------------------------------------------------------*/

/* The look up table for the tile groups. */
static TILE_GROUP_MANAGER tileGroupManager;
/*-------------------------------------------------------------------------------------------------
END STATIC VARIABLES
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
FORWARD DECLARATIONS
-------------------------------------------------------------------------------------------------*/

bool MapCreator_ToFile(char *targetFile, int width, int height, float wallDensity);
TileGen_Map *MapCreator_CreateMap(int width, int height, float wallDensity);
/*-------------------------------------------------------------------------------------------------
END FORWARD DECLARATIONS
-------------------------------------------------------------------------------------------------*/

/**************************************************************************************************
Function      : MapCreator_Map_Initialize
Description   : Initializes an empty map of given size.
Input         : width is the width of the map,
                height is the height of the map.
Output        : Returns pointer to new map if successful. Returns NULL pointer if unsuccessful.
**************************************************************************************************/
static TileGen_Map *MapCreator_Map_Initialize(int width, int height)
{
  /* Allocate memory for the tileGenMap. */
  TileGen_Map * tileGenMap = (TileGen_Map *)malloc(sizeof(TileGen_Map));

  /* If allocation fails, return NULL pointer. */
  if (!tileGenMap)
  {
    /* If error messages are enabled, print error dialogue.  */
    if (SHOW_ERRORS)
    {
      int msgboxID = MessageBox(
        NULL,
        "TileGenMap was not created.",
        "MapCreator_Map_Initialize ERROR",
        MB_ICONEXCLAMATION | MB_YESNO
        );
    }
    return NULL;
  }

  /* Else, proceed with map initialization. */

  /* Allocate memory for the mapgen data array. */
  TileGen_Tile *tile1D = (TileGen_Tile *)calloc(width * height, sizeof(TileGen_Tile));

  /* If allocation fails, return NULL pointer. */
  if (!tile1D)
  {
    /* If error messages are enabled, print error dialogue.  */
    if (SHOW_ERRORS)
    {
      int msgboxID = MessageBox(
        NULL,
        "TileGenTile array was not created.",
        "MapCreator_Map_Initialize ERROR",
        MB_ICONEXCLAMATION | MB_YESNO
        );
    }
    return NULL;
  }

  /* Else, proceed with map initialization. */

  /* Allocate memory for the mapgen tile pointers */
  TileGen_Tile **tile2D = (TileGen_Tile **)malloc(height * sizeof(TileGen_Tile *));

  /* If allocation fails, return NULL pointer. */
  if (!tile2D)
  {
    /* If error messages are enabled, print error dialogue.  */
    if (SHOW_ERRORS)
    {
      int msgboxID = MessageBox(
        NULL,
        "TileGenTile pointers array was not created.",
        "MapCreator_Map_Initialize ERROR",
        MB_ICONEXCLAMATION | MB_YESNO
        );
    }
    return NULL;
  }

  /* Else, proceed with map initialization. */

  /* Set up the array of tile gen pointers. */
  
  int row, column;    /* Iterators for array setup */

  /* Set each row of the array. */
  for (row = 0; row < height; ++row)
    tile2D[row] = tile1D + (row * width);

  /* Set the tile positions and owner of the tiles */
  for (row = 0; row < height; ++row)
    for (column = 0; column < width; ++column)
    {
      tile2D[row][column].mapOwner = tileGenMap;

      tile2D[row][column].x = row;
      tile2D[row][column].y = column;
    }

  /* Set the values of the tileGenMap. */
  tileGenMap->map = tile2D;
  tileGenMap->height = height;
  tileGenMap->width = width;

  /* Return pointer to the newly intialized tileGenMap. */
  return tileGenMap;
}

/**************************************************************************************************
Function      : MapCreator_Tile_Updateconnections
Description   : Updates the connections on the tile.
Input         : tile is the tile to update.
Output        : No return.
**************************************************************************************************/
void MapCreator_Tile_Updateconnections(TileGen_Tile *tile)
{
  /* If the top tile is valid, check it. */
  if (tile->x + 1 < tile->mapOwner->height)
  {
    /* Check if the top tile is a collision tile, */
    if (tile->mapOwner->map[tile->x + 1][tile->y].isWall)
    {
      /* Update the connections array. */
      tile->connections[TILE_TOP] = tile->mapOwner->map[tile->x + 1][tile->y];
    }
  }

  /* If the bottom tile is valid, check it. */
  if (tile->x - 1 >= 0)
  {
    /* Check if the bottom tile is a collision tile, */
    if (tile->mapOwner->map[tile->x - 1][tile->y].isWall)
    {
      /* Update the connections array. */
      tile->connections[TILE_BOTTOM] = tile->mapOwner->map[tile->x - 1][tile->y];
    }
  }

  /* If the left tile is valid, check it. */
  if (tile->y - 1 >= 0)
  {
    /* Check if the left tile is a collision tile, */
    if (tile->mapOwner->map[tile->x][tile->y - 1].isWall)
    {
      /* Update the connections array. */
      tile->connections[TILE_LEFT] = tile->mapOwner->map[tile->x][tile->y - 1];
    }
  }

  /* If the right tile is valid, check it. */
  if (tile->y + 1 < tile->mapOwner->width)
  {
    /* Check if the right tile is a collision tile, */
    if (tile->mapOwner->map[tile->x][tile->y + 1].isWall)
    {
      /* Update the connections array. */
      tile->connections[TILE_RIGHT] = tile->mapOwner->map[tile->x][tile->y + 1];
    }
  }

  /* If the top-left tile is valid, check it. */
  if ((tile->x + 1 < tile->mapOwner->height) && (tile->y - 1 >= 0))
  {
    /* Check if the top-left tile is a collision tile, */
    if (tile->mapOwner->map[tile->x + 1][tile->y - 1].isWall)
    {
      /* Update the connections array. */
      tile->connections[TILE_TOP_LEFT] = tile->mapOwner->map[tile->x + 1][tile->y - 1];
    }
  }

  /* If the top-right tile is valid, check it. */
  if ((tile->x + 1 < tile->mapOwner->height) && (tile->y + 1 < tile->mapOwner->width))
  {
    /* Check if the top-right tile is a collision tile, */
    if (tile->mapOwner->map[tile->x + 1][tile->y + 1].isWall)
    {
      /* Update the connections array. */
      tile->connections[TILE_TOP_RIGHT] = tile->mapOwner->map[tile->x + 1][tile->y + 1];
    }
  }

  /* If the bottom-left tile is valid, check it. */
  if ((tile->x - 1 >= 0) && (tile->y - 1 >= 0))
  {
    /* Check if the bottom-left tile is a collision tile, */
    if (tile->mapOwner->map[tile->x - 1][tile->y - 1].isWall)
    {
      /* Update the connections array. */
      tile->connections[TILE_BOTTOM_LEFT] = tile->mapOwner->map[tile->x - 1][tile->y - 1];
    }
  }

  /* If the bottom-right tile is valid, check it. */
  if ((tile->x - 1 >= 0) && (tile->y + 1 < tile->mapOwner->width))
  {
    /* Check if the bottom-right tile is a collision tile, */
    if (tile->mapOwner->map[tile->x - 1][tile->y + 1].isWall)
    {
      /* Update the connections array. */
      tile->connections[TILE_BOTTOM_RIGHT] = tile->mapOwner->map[tile->x - 1][tile->y + 1];
    }
  }
}

/**************************************************************************************************
Function      : MapCreator_Tile_CheckSetConfigSize_2
Description   : Checks configurations of connected tile sets of size 2.
Input         : tile is the tile to check config for.
Output        : Returns true if there is a conflict, false if there is not.
**************************************************************************************************/
bool MapCreator_Tile_CheckSetConfigSize_2(TileGen_Tile *tile)
{
  /*
  Test for config
  1 0 0
  1 X 0
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_LEFT] && tile->connections[TILE_TOP_LEFT])
    return false;

  /*
  Test for config
  0 0 0
  1 X 0
  1 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_LEFT] && tile->connections[TILE_BOTTOM_LEFT])
    return false;

  /*
  Test for config
  0 0 0
  0 X 0
  1 1 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_BOTTOM] && tile->connections[TILE_BOTTOM_LEFT])
    return false;

  /*
  Test for config
  0 0 0
  0 X 0
  0 1 1
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_BOTTOM] && tile->connections[TILE_BOTTOM_RIGHT])
    return false;

  /*
  Test for config
  0 0 0
  0 X 1
  0 0 1
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_RIGHT] && tile->connections[TILE_BOTTOM_RIGHT])
    return false;

  /*
  Test for config
  0 0 1
  0 X 1
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_RIGHT] && tile->connections[TILE_TOP_RIGHT])
    return false;

  /*
  Test for config
  0 1 1
  0 X 0
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_TOP] && tile->connections[TILE_TOP_RIGHT])
    return false;

  /*
  Test for config
  1 1 0
  0 X 0
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_TOP] && tile->connections[TILE_TOP_LEFT])
    return false;

  /* If none of the configs match, then it's not safe to build. Return true. */
  return true;

}

/**************************************************************************************************
Function      : MapCreator_Tile_CheckSetConfigSize_3
Description   : Checks configurations of connected tile sets of size 3.
Input         : tile is the tile to check config for.
Output        : Returns true if there is a conflict, false if there is not.
**************************************************************************************************/
bool MapCreator_Tile_CheckSetConfigSize_3(TileGen_Tile *tile)
{
  /*
  Test for config
  1 0 0
  1 X 0
  1 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_LEFT] && tile->connections[TILE_TOP_LEFT] && tile->connections[TILE_BOTTOM_LEFT])
    return false;

  /*
  Test for config
  1 1 1
  0 X 0
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_TOP] && tile->connections[TILE_TOP_LEFT] && tile->connections[TILE_TOP_RIGHT])
    return false;

  /*
  Test for config
  0 0 1
  0 X 1
  0 0 1
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_RIGHT] && tile->connections[TILE_BOTTOM_RIGHT] && tile->connections[TILE_TOP_RIGHT])
    return false;

  /*
  Test for config
  0 0 0
  0 X 0
  1 1 1
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_BOTTOM] && tile->connections[TILE_BOTTOM_LEFT] && tile->connections[TILE_BOTTOM_RIGHT])
    return false;

  /*
  Test for config
  1 1 0
  1 X 0
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_TOP] && tile->connections[TILE_TOP_LEFT] && tile->connections[TILE_LEFT])
    return false;

  /*
  Test for config
  0 1 1
  0 X 1
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_TOP] && tile->connections[TILE_TOP_RIGHT] && tile->connections[TILE_RIGHT])
    return false;

  /*
  Test for config
  0 0 0
  0 X 1
  0 1 1
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_RIGHT] && tile->connections[TILE_BOTTOM_RIGHT] && tile->connections[TILE_BOTTOM])
    return false;

  /*
  Test for config
  0 0 0
  1 X 0
  1 1 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_BOTTOM] && tile->connections[TILE_BOTTOM_LEFT] && tile->connections[TILE_LEFT])
    return false;

  /*
  Test for config
  1 1 0
  0 X 1
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_TOP] && tile->connections[TILE_TOP_LEFT] && tile->connections[TILE_RIGHT])
    return false;

  /*
  Test for config
  0 0 1
  0 X 1
  0 1 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_RIGHT] && tile->connections[TILE_TOP_RIGHT] && tile->connections[TILE_BOTTOM])
    return false;

  /*
  Test for config
  0 0 0
  1 X 0
  0 1 1
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_BOTTOM] && tile->connections[TILE_BOTTOM_RIGHT] && tile->connections[TILE_LEFT])
    return false;

  /*
  Test for config
  0 1 0
  1 X 0
  1 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_LEFT] && tile->connections[TILE_BOTTOM_LEFT] && tile->connections[TILE_TOP])
    return false;

  /*
  Test for config
  0 1 1
  1 X 0
  0 0 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_TOP] && tile->connections[TILE_LEFT] && tile->connections[TILE_TOP_RIGHT])
    return false;

  /*
  Test for config
  1 0 0
  1 X 0
  0 1 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_LEFT] && tile->connections[TILE_TOP_LEFT] && tile->connections[TILE_BOTTOM])
    return false;

  /*
  Test for config
  0 0 0
  0 X 1
  1 1 0
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_BOTTOM] && tile->connections[TILE_BOTTOM_LEFT] && tile->connections[TILE_RIGHT])
    return false;

  /*
  Test for config
  0 1 0
  0 X 1
  0 0 1
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_RIGHT] && tile->connections[TILE_TOP] && tile->connections[TILE_BOTTOM_RIGHT])
    return false;

  /* If none of the configs match, then it's not safe to build. Return true. */
  return true;
}

/**************************************************************************************************
Function      : MapCreator_Tile_CheckSetConfigSize_4
Description   : Checks configurations of connected tile sets of size 4.
Input         : tile is the tile to check config for.
Output        : Returns true if there is a conflict, false if there is not.
**************************************************************************************************/
bool MapCreator_Tile_CheckSetConfigSize_4(TileGen_Tile *tile)
{
  /*
  Test for config
  0 1 0
  0 X 1
  0 0 1
  If config matches, then okay to build. Return false.
  */
  if (tile->connections[TILE_RIGHT] && tile->connections[TILE_TOP] && tile->connections[TILE_BOTTOM_RIGHT])
    return false;



  /* If none of the configs match, then it's not safe to build. Return true. */
  return true;
}

/**************************************************************************************************
Function      : MapCreator_Tile_CheckConnectionConflicts
Description   : Checks for conflicts between connecting tiles.
Input         : tile is the tile to check for conflicts.
Output        : Returns true if there is a conflict, false if there is not.
**************************************************************************************************/
bool MapCreator_Tile_CheckConnectionConflicts(TileGen_Tile *tile)
{
  /*
    First, count the number of active connections.
    If there are less than 2, then there is room to add a tile.
    If there are more than 6, then this is a closed area, so it is okay to put in a tile.
    If there are between 2 to 6, then this may be a pathway, so we need to check the tiles.
  */

  int activeConnections = 0;  /* The number of active tile connections */
  int i;                      /* Iterator */

  /* Check each connecting position. */
  for (i = 0; i < MAX_TILE_CONNECTIONS; ++i)
  {
    /* If the connection is active, incremement the active connection counter. */
    if (tile->connections[i])
    {
      ++activeConnections;
    }
  }

  /* If there are less than 2 or more than 6 active connections, we're safe to build here. Return false. */
  if ((activeConnections < 2) || (activeConnections > 6))
    return false;

  /* Else, we have to check the layout of the tiles. */

  /* Check configs for 2 active connections. */
  if (activeConnections == 2)
    return MapCreator_Tile_CheckSetConfigSize_2(tile);

  /* Check configs for 3 active connections. */
  if (activeConnections == 3)
    return MapCreator_Tile_CheckSetConfigSize_3(tile);

  if (activeConnections == 4)
  {
  }

  /*
      1 0 1
      1 X 1
      1 0 1
    (X is our tile, 1 is collision tile, 0 is non-collision tile)
  */

}

/**************************************************************************************************
Function      : MapCreator_Tile_AssignCollision
Description   : Sets collision flags on tile.
Input         : tile is the tile to set,
                enableCollision is flag to set tile with.
Output        : Returns true if resulting tile is set to collision, false if not.
**************************************************************************************************/
bool MapCreator_Tile_AssignCollision(TileGen_Tile *tile, bool enableCollision)
{
  /* If setting collision */
  if (enableCollision)
  {
    /* Check the tiles around this one */
    MapCreator_Tile_Updateconnections(tile);
    


  }
}

/**************************************************************************************************
Function      : MapCreator_Map_GenerateValues
Description   : Generates data for tiles inside of a map.
Input         : tileMap is the map to use,
                wallDensity is the ratio of walls to open spaces, range from 0 to 1.
                  0 means don't create walls, 1 means cover the entire map with walls.
                passes is the number of times to pass through the generated tiles for wall quality.
Output        : No return.
**************************************************************************************************/
void MapCreator_Map_GenerateValues(TileGen_Map *tileMap, float wallDensity, int passes)
{
  int row, column;      /* Iterators */

  /* Calculate the number of walls to create. */
  int wallsToCreate = FloatToInt(tileMap->height * tileMap->width * wallDensity);

  /* First, create borders on the map. */

  /* Start with the left and right side of the map. */
  //for (row = 0; row < tileMap->height; ++row)
  //{
  //  tileMap->map[row][0].isWall = 1;
  //  tileMap->map[row][0].connections = 1;
  //  tileMap->map[row][0].tileGroup = 1;
  //  tileMap->map[row][0].isWall = 1;
  //}

  // DUMB TEST CODE
  int i;

  RandSeed(1);

  for (i = 0; i < wallsToCreate; ++i)
  {
    int x = RandIntRange(1, tileMap->height);
    int y = RandIntRange(0, tileMap->width);

    tileMap->map[x][y].isWall = 1;
  }

}

/**************************************************************************************************
Function      : MapCreator_CreateMap
Description   : Generates map data for a map of given size and wall density.
Input         : width is the width of the map,
                height is the height of the map,
                wallDensity is the ratio of walls to open spaces, range from 0 to 1.
                  0 means don't create walls, 1 means cover the entire map with walls.
Output        : Returns pointer to new map if successful. Returns NULL pointer if unsuccessful.
**************************************************************************************************/
TileGen_Map *MapCreator_CreateMap(int width, int height, float wallDensity)
{
  /* Initialize a TileGen_Map. */
  TileGen_Map *tileGenMap = MapCreator_Map_Initialize(width, height);

  /* If initialization fails, return NULL pointer. */
  if (!tileGenMap)
  {
    /* If error messages are enabled, print error dialogue.  */
    if (SHOW_ERRORS)
    {
      int msgboxID = MessageBox(
        NULL,
        "TileGenMap was not created.",
        "MapCreator_CreateMap ERROR",
        MB_ICONEXCLAMATION | MB_YESNO
        );
    }
    return NULL;
  }

  /* Else, proceed with map generation. */

  /* Generate values for the map. */
  MapCreator_Map_GenerateValues(tileGenMap, wallDensity, 3);

  /* Return the generated map. */
  return tileGenMap;
}

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
bool MapCreator_ToFile(char *targetFile, int width, int height, float wallDensity)
{

  TileGen_Map *tileGenMap = MapCreator_CreateMap(width, height, wallDensity);

  /* If map was not created, return false. */
  if (!tileGenMap)
  {
    /* If error messages are enabled, print error dialogue.  */
    if (SHOW_ERRORS)
    {
      int msgboxID = MessageBox(
        NULL,
        "TileGenMap was not created.",
        "MapCreator_ToFile ERROR",
        MB_ICONEXCLAMATION | MB_YESNO
        );
    }
    return false;
  }

  /* Else, proceed with writing to file. */

  /* Create the name of the path. */
  char *path = (char *)calloc(PATH_MAX_LEN, sizeof(char));
  strcat_s(path, PATH_MAX_LEN * sizeof(char), MAP_DIRECTORY);
  strcat_s(path, PATH_MAX_LEN * sizeof(char), targetFile);


  /* Open the file for writing. */
  FILE *destinationFile = NULL;
  fopen_s(&destinationFile, path, "wt");

  /* If fopen_s fails, return false. */
  if (!destinationFile)
  {
    /* If error messages are enabled, print error dialogue.  */
    if (SHOW_ERRORS)
    {
      int msgboxID = MessageBox(
        NULL,
        "File was not opened.",
        "MapCreator_ToFile ERROR",
        MB_ICONEXCLAMATION | MB_YESNO
        );
    }
    return false;
  }

  /* Else, write the map data to the file */

  /* Write the width and height information. */
  fprintf_s(destinationFile, "Width %i\nHeight %i\n\n", width, height);

  int i, j;   /* Iterators */

  /* Write the collision information for each tile in the map. */
  for (i = 0; i < height; ++i)
  {
    for (j = 0; j < width; ++j)
    {
      fprintf_s(destinationFile, "|%2i,%2i|", tileGenMap->height - i - 1, j);// tileGenMap->map[i][j].isWall);
    }
    fprintf_s(destinationFile, "\n");
  }

  /* Close the file. */
  fclose(destinationFile);

  /* Done. Return true. */
  return true;
}