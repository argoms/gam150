#pragma once
typedef struct IsoMap IsoMap;

struct IsoMap
{
  int* map;
  int mapHeight;
  int mapWidth;
};
void IsoInit(int _mapHeight, int _mapWidth);
Vector2D IsoScreenToWorld(Vector2D* _input);
Vector2D IsoWorldToScreen(Vector2D* _input);
IsoMap* IsoCreateNewMap(int _mapHeight, int _mapWidth);
int IsoTileGet(int _x, int _y);
void IsoTileSet(int _x, int _y, int _newValue);
void IsoSpawnMap();