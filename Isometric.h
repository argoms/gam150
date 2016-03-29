/*!
\file   Isometric.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Isometric tilemap implementation. Contains functions relating to tilemaps and isometric conversions.
*/
#pragma once
#include "Vector2D.h"

enum tileTypes
{
  tile_empty = -1,
  tile_floor = 0,
  tile_wall = 1,
  tile_path = 2
};



typedef struct IsoMap IsoMap;

/*!
\struct IsoMap
\brief Contains information on a given tilemap
*/
struct IsoMap
{
  int* map; /**< pointer to tile data array*/
  int mapHeight; /**< height of the map in tiles*/
  int mapWidth; /**< width of the map in tiles*/
};
void IsoInit(int _mapHeight, int _mapWidth);
Vector2D IsoScreenToWorld(Vector2D* _input);
Vector2D IsoWorldToScreen(Vector2D* _input);
IsoMap* IsoCreateNewMap(int _mapHeight, int _mapWidth);
int IsoTileGet(int _x, int _y);
void IsoTileSet(int _x, int _y, int _newValue);
void IsoSpawnMap();