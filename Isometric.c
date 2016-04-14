/*!
Project (working title): Epoch
\file   Isometric.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Isometric tilemap implementation. Contains functions relating to tilemaps and isometric conversions.
*/
#include "Vector2D.h"
#include "Isometric.h"
#include "Graphics.h"
#include <stdlib.h>
#include "MapGen.h"
#include "MyRandom.h"
#include "MapLevelSpawning.h"
//#include "MapCreator.h"
#include "Gate.h"
static IsoMap* gameMap; /**< contains currently active game map*/



/*!
\brief initializes isometric map system
Sets the private gameMap var to a newly generated array with dimensions specified by the function's parameters. 
Note that isometry means that 'x' goes up-right, and 'y' goes up-left increasing.

\param _mapHeight height of the map (in tiles)
\param _mapWidth width of the map (in tiles)
*/
void IsoInit(int _mapHeight, int _mapWidth)
{
  gameMap = IsoCreateNewMap(_mapHeight, _mapWidth);

  int i = 0;
  int j = 0;
  //MapCreator_ToMap(gameMap, 0.1f);
  
  GenerateMap(gameMap);

  IsoSpawnMap();
}

/*!
\brief given on-screen coordinates, returns their equivalents in-world

\param _input 2D vector containing screen coordinates (x and y floats)
\return Returns the world coordinates in the form of a vector.
*/
Vector2D IsoScreenToWorld(Vector2D* _input)
{
  Vector2D newVec = Vec2(
    (_input->y / 64) + (_input->x / 128),
    (_input->y / 64) - (_input->x / 128)
    );
  return newVec;
}

/*!
\brief given in-world coordinates, returns their equivalents on-screen

\param _input 2D vector containing world coordinates (x and y floats)
\return Returns the screen coordinates in the form of a vector.
*/
Vector2D IsoWorldToScreen(Vector2D* _input)
{
  Vector2D newVec = Vec2(
    (_input->x * 64) - (_input->y * 64),
    (_input->y + _input->x) * 32
    );
  return newVec;
}

/*!
\brief Given a height and with, creates a new map
Note that the "2D" array is actually a long one-dimensional array.

\param _mapHeight height of the map (in tiles)
\param _mapWidth width of the map (in tiles)
\return Returns a pointer to the newly created map object.
*/
IsoMap* IsoCreateNewMap(int _mapHeight, int _mapWidth)
{
  int mapLength = _mapHeight * _mapWidth;
  int i = 0; //index
  int j = 0; //index

  IsoMap* newObject =  (IsoMap*) malloc(sizeof(IsoMap));
  newObject->mapHeight = _mapHeight;
  newObject->mapWidth = _mapWidth;
  newObject->map = (int*) malloc(sizeof(int) * mapLength);
  

  return newObject;
}

/*!
\brief Gets the tile info of a given coordinate on the active game map
\param _x x coordinate of tile
\param _y y coordinate of tile
\return Returns the tile at the given position.
*/
int IsoTileGet(int _x, int _y)
{
  if (_x > -1 && _y > -1 && _x < gameMap->mapWidth && _y < gameMap->mapHeight)
  {
    return gameMap->map[_x + (_y * gameMap->mapWidth)];
  }
  else
  {
    return tile_error;
  }
}

/*!
\brief Sets the tile info of a given coordinate on the active game map
\param _x x coordinate of tile
\param _y y coordinate of tile
\param _newValue the value to set at the given tile coordinate
*/
void IsoTileSet(int _x, int _y, int _newValue)
{
  if (_x > -1 && _y > -1 && _x < gameMap->mapWidth && _y < gameMap->mapHeight)
  {
    //printf("%i|", gameMap->map[_x + (_y * gameMap->mapWidth)]);
    gameMap->map[_x + (_y * gameMap->mapWidth)] = _newValue;
    //printf("%i|", gameMap->map[_x + (_y * gameMap->mapWidth)]);
  }
}

/*!
\brief Spawns sprites to create the map on-screen
*/
void IsoSpawnMap()
{
  GenerateMapObjects();
  //there used to be code here, it got moved to MapLevelSpawning and MapGEn.
}

/*!
\brief Getter for the currently active isomap object (level tile data information)
\return Returns pointer to isomap.
*/
IsoMap* GetGameMap()
{
  return gameMap;
}