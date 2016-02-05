#include "Vector2D.h"
#include "Isometric.h"
#include "Graphics.h"

static IsoMap* gameMap;

void IsoInit(int _mapHeight, int _mapWidth)
{
  gameMap = IsoCreateNewMap(_mapHeight, _mapWidth);

  int i = 0;
  int j = 0;

  while (i < _mapWidth)
  {
    j = 0;
    while (j < _mapHeight)
    {
      //printf("%iaa", j);
      if (i == 0 || i == _mapWidth - 1 || j == 0 || j == _mapHeight - 1)
      {
        IsoTileSet(i, j, 1);
      }
      else
      {
        IsoTileSet(i, j, 0);
        //printf("a");
      }
      j++;
    }
    i++;
  }
  IsoTileSet(4, 4, 1);

  IsoTileSet(6, 6, 1);
  IsoTileSet(6, 7, 1);
  IsoTileSet(7, 6, 1);
  IsoTileSet(7, 7, 1);
  IsoSpawnMap();
}
Vector2D IsoScreenToWorld(Vector2D* _input)
{
  Vector2D newVec = Vec2(
    (_input->y / 64) + (_input->x / 128),
    (_input->y / 64) - (_input->x / 128)
    );
  return newVec;
}

Vector2D IsoWorldToScreen(Vector2D* _input)
{
  Vector2D newVec = Vec2(
    (_input->x * 64) - (_input->y * 64),
    (_input->y + _input->x) * 32
    );
  return newVec;
}

IsoMap* IsoCreateNewMap(int _mapHeight, int _mapWidth)
{
  int mapLength = _mapHeight * _mapWidth;
  int i = 0; //index
  int j = 0; //index

  IsoMap* newObject =  malloc(sizeof(IsoMap));
  newObject->mapHeight = _mapHeight;
  newObject->mapWidth = _mapWidth;
  newObject->map = (int*) malloc(sizeof(int) * mapLength);
  
  

  return newObject;
}

//grabs the given coordinate on the gamemap
int IsoTileGet(int _x, int _y)
{
  return gameMap->map[_x + (_y * gameMap->mapWidth)];
}

//sets the given coordinate on the gamemap
void IsoTileSet(int _x, int _y, int _newValue)
{
  //printf("%i|", gameMap->map[_x + (_y * gameMap->mapWidth)]);
  gameMap->map[_x + (_y * gameMap->mapWidth)] = _newValue;
  //printf("%i|", gameMap->map[_x + (_y * gameMap->mapWidth)]);
}

//spawns the game map sprites
void IsoSpawnMap()
{
  int i = 0; //index
  int j = 0; //index
  int mapWidth = gameMap->mapWidth;
  int mapHeight = gameMap->mapHeight;

  Animation* tileAnim = GCreateAnimation(1,
    GCreateTexture("isotile.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);

  //playerSprite = GCreateSprite(0, 40, anim, 1);

  while (i < mapWidth)
  {
    j = 0;
    while (j < mapHeight)
    {
      if (IsoTileGet(i, j) == 1)
      {
        Vector2D tilePos = Vec2(i, j);
        float tileX = IsoWorldToScreen(&tilePos).x;
        float tileY = IsoWorldToScreen(&tilePos).y;
        printf("(%i, %i)", i, j);
        GCreateSprite(tileX, tileY, tileAnim, 0);
        
      }
      j++;
    }
    i++;
  }
}