//#include "MapGen.h"
#include "Vector2D.h"
#include "Isometric.h"

#define ROOM_SIZE 10

void GenerateMap(IsoMap* inputMap)
{
  int mapHeight = inputMap->mapHeight;
  int mapWidth = inputMap->mapWidth;
  int i = 0;
  int j = 0;
  
  //create borders:
  while (i < mapWidth)
  {
    j = 0;
    while (j < mapHeight)
    {
      //printf("%iaa", j);
      if (i % ROOM_SIZE == 0 || j % ROOM_SIZE == 0)
      {
        IsoTileSet(i, j, 1);
      }
      else
      {
        IsoTileSet(i, j, 0);
        //printf("a");
      }
      IsoTileSet(i, j, 0);
      j++;
      
    }
    i++;
  }
}