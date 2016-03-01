//#include "MapGen.h"
#include "Vector2D.h"
#include "Isometric.h"
#include "AEEngine.h"

#define ROOM_SIZE 10

void GenerateMap(IsoMap* inputMap)
{
  int mapHeight = inputMap->mapHeight;
  int mapWidth = inputMap->mapWidth;
  int i = 0;
  int j = 0;
  while (i < mapWidth)
  {
    j = 0;
    while (j < mapHeight)
    {
      //printf("%iaa", j);
      if (i == 0 || i == mapWidth - 1 || j == 0 || j == mapHeight - 1)
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

  i = 1;
  j = 1;
  //create some walls around arbitrary rooms:
  while (i < mapWidth - 1)
  {
    j = 1;
    while (j < mapHeight - 1)
    {
      //printf("%iaa", j);
      if ((i % ROOM_SIZE == 0 || j % ROOM_SIZE == 0) && AERandFloat() > 0.6f)
      {
        IsoTileSet(i, j, 1);
      }
      j++;
    }
    i++;
  }
}