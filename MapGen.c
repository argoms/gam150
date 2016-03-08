/*!
\file   MapGen.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions for procedurally generating game levels.
*/
//#include "MapGen.h"
#include "Vector2D.h"
#include "Isometric.h"
#include "AEEngine.h"
#include "ImportData.h"
#include "GameLevel.h"
#include "Door.h"

//private info (would be defines but you can't make those private?

#define NUM_ROOMS 5

//Think of this as MAX room size, not just room size.
static int ROOM_SIZE = 18; /**< Room size, subtract 2 from this due to increased wall thickness*/


//static enums/structs:
static enum directions
{
  dir_left,
  dir_right,
  dir_up,
  dir_down
};

//static function prototypes:
static void FillLine(Vector2D position1, Vector2D position2, int newValue);
static Vector2D directionOffsetGet(int dir);
static void FillArea(int x, int y);
static int isValid(Vector2D position, IsoMap* map);
static void IsoSquareSet(Vector2D position, int newValue);

//implementation:
void GenerateMap(IsoMap* inputMap)
{
  int mapHeight = inputMap->mapHeight;
  int mapWidth = inputMap->mapWidth;
  int i = 0;
  int j = 0;


  /*
  //borders:
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
      if ((i % ROOM_SIZE == 0 || j % ROOM_SIZE == 0)
        || ((i + 1) % ROOM_SIZE == 0 || (j + 1) % ROOM_SIZE == 0)
        || ((i - 1) % ROOM_SIZE == 0 || (j - 1) % ROOM_SIZE == 0))
      {
        IsoTileSet(i, j, 1);
      }
      j++;
    }
    i++;
  }
  */
  //the following code combines the upper two things into one loop. Seriously, that's like half the number of operations, it's not premature optimization :I 
  //create borders and ROOM_SIZE-sized rooms:
  while (i < mapWidth)
  {
    j = 0;
    while (j < mapHeight)
    {
      //printf("%iaa", j);
      if (i == 0 || i == mapWidth - 1 || j == 0 || j == mapHeight - 1 || (i % ROOM_SIZE == 0 || j % ROOM_SIZE == 0)
        || ((i + 1) % ROOM_SIZE == 0 || (j + 1) % ROOM_SIZE == 0)
        || ((i - 1) % ROOM_SIZE == 0 || (j - 1) % ROOM_SIZE == 0))
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

  //create path of rooms:

  Vector2D cursor = Vec2(ROOM_SIZE / 2, ROOM_SIZE / 2); //create a "cursor" for the rooms, starting at the spawn room
  int rooms_created = 0;
  int tries = 0; //infinite loop killer

  //start creating rooms:
  while (rooms_created < NUM_ROOMS && tries++ < NUM_ROOMS * 100)
  {
    int direction = (int)(AERandFloat() * 4); //random direction
    Vector2D offset = directionOffsetGet(direction); //turn that random stupid number into an actual random direction
    Vector2DScale(&offset, &offset, ROOM_SIZE);

    Vector2D newCursorPosition;
    Vector2DAdd(&newCursorPosition, &cursor, &offset); //travel one room size in that random direction

    if (isValid(newCursorPosition, inputMap) && IsoTileGet((int)newCursorPosition.x, (int)newCursorPosition.y) != 1)
    {
      printf("MapGen.c: x: %f, y: %f, valid apparently. \n", newCursorPosition.x, newCursorPosition.y);
      
      
      //now bear with me here, first we're checking if the room we want to go into already exists
      if (IsoTileGet((int)newCursorPosition.x, (int)newCursorPosition.y) == 2)
      {
        //if it does, there's a 50% chance that we don't do anything with it
        if (AERandFloat() < 0.5)
        {
          continue;
        }
        else
        {
          //otherwise, we branch back into it (this doesn't count as creating a room)
          rooms_created--;
        }
      }

      //fill the path between rooms:
      FillLine(cursor, newCursorPosition, 2);
      cursor = newCursorPosition;


      //a room has been created! what do?
      //arbitrary scope to help visualize things:
      {
        if (rooms_created == NUM_ROOMS - 1)
        {
          DoorCreateDoorAt(cursor);
        }

        //placeholder: spawn one enemy.
        ImportEnemyData(cursor.x, cursor.y, "Level1EnemyMelee1.txt", GetPlayerObject());

      }
      rooms_created++;
    }
    else
    {
      continue; //as if I have to tell you, you silly program.
    }
  }
  if (tries > NUM_ROOMS * 100)
  {
    printf("MapGen.c: seriously your algorithm is fucked, look at it. Re-generating map.");
    //GenerateMap(inputMap); //comment this back in for release, but while debugging keep it out to avoid recursion.
  }

  
  
}

/*!
\brief Fills a line between two given vector2Ds
*/
static void FillLine(Vector2D position1, Vector2D position2, int newValue)
{
  int dist = (int)Vector2DSquareDistance(&position1, &position2);
  Vector2D unitVector;
  Vector2DSub(&unitVector, &position2, &position1);
  Vector2DNormalize(&unitVector, &unitVector);

  int i = 0; //because we're technically using c, right?
  while (i * i < dist)
  {
    Vector2DAdd(&position1, &position1, &unitVector);
    //IsoTileSet((int)position1.x, (int)position1.y, newValue);
    IsoSquareSet(position1, newValue);
    i++; //fuck your ++i
  }
}

/*!
\brief Fills in a 3x3 square around a given area.
*/
static void IsoSquareSet(Vector2D position, int newValue)
{
  IsoTileSet((int)position.x, (int)position.y, newValue);
  IsoTileSet((int)position.x + 1, (int)position.y, newValue);
  IsoTileSet((int)position.x - 1, (int)position.y, newValue);

  IsoTileSet((int)position.x, (int)position.y - 1, newValue);
  IsoTileSet((int)position.x + 1, (int)position.y - 1, newValue);
  IsoTileSet((int)position.x - 1, (int)position.y - 1, newValue);

  IsoTileSet((int)position.x, (int)position.y + 1, newValue);
  IsoTileSet((int)position.x + 1, (int)position.y + 1, newValue);
  IsoTileSet((int)position.x - 1, (int)position.y + 1, newValue);
}
/*!
\brief Checks wheter or not a given position is within the bounds of the map.

\return Returns boolean (aka 1/0).
*/
static int isValid(Vector2D position, IsoMap* map)
{
  if (position.x < 0 || position.y < 0 || position.x > map->mapWidth + 1 || position.y > map->mapHeight + 1)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

/*!
\brief Turns a direction enum into a vector2D equivalent unit vector.

\return Returns vector2D by value.
*/
static Vector2D directionOffsetGet(int dir)
{
  Vector2D offset;
  switch (dir)
  {
  case dir_up:
    offset.x = 0;
    offset.y = 1;
    break;
  case dir_down:
    offset.x = 0;
    offset.y = -1;
    break;
  case dir_left:
    offset.x = -1;
    offset.y = 0;
    break;
  case dir_right:
    offset.x = 1;
    offset.y = 0;
    break;
  }
  return offset;
}

//meant to fill area for floor tiles, unused for now
//I wonder what the best fill algorithm would be.
static void FillArea(int x, int y, int tileType)
{

}