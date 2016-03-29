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
#include "Gate.h"

//private info (would be defines but you can't make those private?

#define NUM_ROOMS 5

//Think of this as MAX room size, not just room size.
static int ROOM_SIZE = 17;//22; /**< Room size, subtract 2 from this due to increased wall thickness*/

typedef struct MapRoom MapRoom;
//state enums for each room
enum MapGen_RoomStates
{
  roomstate_inactive, 
  roomstate_active,
  roomstate_cleared
};

enum MapGen_RoomTypes
{
  roomtype_simple,
  roomtype_start
};


/*!
\struct MapRoom
\brief contains a full room of objects
*/
struct MapRoom
{
  int type;
  int state; //see MapGen_RoomStates enum
  int size; //height/width of room (square rooms)
  int numEnemies; // number of enemies still alive in room
  
  Vector2D position;
  GameObject* gates[4];

  int roomNum; //internal, order of creation
};

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
static void Room_BasicEnemies(Vector2D cursor);
static void RoomTemplate(Vector2D cursor, int spawnGates);
static void Room_StartRoom(Vector2D cursor);

//implementation:
void GenerateMap(IsoMap* inputMap)
{
  int mapHeight = inputMap->mapHeight;
  int mapWidth = inputMap->mapWidth;
  int i = 0;
  int j = 0;

  int roomsNum = 0;
  MapRoom rooms[NUM_ROOMS];

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
  int rooms_created = 1;
  int tries = 0; //infinite loop killer

  //generate a room for start
  rooms[roomsNum].type = roomtype_start;
  rooms[roomsNum].position.x = cursor.x;
  rooms[roomsNum].position.y = cursor.y;
  printf("room: %f, %f, int %i START ROOM\n", rooms[roomsNum].position.x, rooms[roomsNum].position.y, roomsNum);

  roomsNum++;

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
      //tile 2 (path) for default, tile 3 (gate) for door areas
      FillLine(cursor, newCursorPosition, 2);
      if (IsoTileGet(cursor.x, cursor.y))
      {
        //printf("MAP TILE", IsoTileGet(cursor.x, cursor.y));
      }
      cursor = newCursorPosition;


      //a room has been created! what do?
      //arbitrary scope to help visualize things:
      {
        if (rooms_created == NUM_ROOMS - 1)
        {
          DoorCreateDoorAt(cursor);
        }

        //placeholder: spawn one enemy.

        //MapRoom inst = rooms[roomsNum];
        rooms[roomsNum].type = roomtype_simple;
        rooms[roomsNum].position.x = cursor.x;
        rooms[roomsNum].position.y = cursor.y;
        printf("room: %f, %f, int %i PREGEN\n", rooms[roomsNum].position.x, rooms[roomsNum].position.y, roomsNum);

        roomsNum++;
        //Room_BasicEnemies(cursor);

      }
      rooms_created++;
    }
    else
    {
      continue; //as if I have to tell you, you silly program.
    }
  }

  //discount exceptions
  if (tries > NUM_ROOMS * 100)
  {
    printf("MapGen.c: seriously your algorithm is fucked, look at it. Re-generating map.");
    abort();
    //GenerateMap(inputMap); //comment this back in for release, but while debugging keep it out to avoid recursion.
  }

  //spawn rooms:
  i = 0;
  while (i < NUM_ROOMS)
  {
    if (rooms[i].type == roomtype_start)
    {
      Room_StartRoom(rooms[i].position);
      printf("startroom");
    }
    else
    {
      Room_BasicEnemies(rooms[i].position);
    }
    printf("room: %f, %f, %i \n \n", rooms[i].position.x, rooms[i].position.y, rooms[i].type);
    i++;
    /*

    else if (i == 1)
    {
    printf("whyisthisdoingnothing");
    DoorCreateDoorAt(rooms[i].position);
    }
    */
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
    //IsoSquareSet(position1, IsoTileGet(position1.x + unitVector.x * 2, position1.y + unitVector.y * 2) == 1 ? 3 : 2);
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

static void Room_BasicEnemies(Vector2D cursor)
{
  RoomTemplate(cursor, 1);
}

static void Room_StartRoom(Vector2D cursor)
{
  RoomTemplate(cursor, 0);
}

/*!
\brief Basic room setup/initialization
\param cursor Location to set up room.
\param spawnGates whether or not to spawn gate objects.
*/
static void RoomTemplate(Vector2D cursor, int spawnGates)
{
  
  GameObject* newRoom = GameObjectCreate(PhysicsCreateObject(cursor, 0), 0, 0, entity_room); //999 is placeholder, should be enum later
  
  newRoom->syncSpritePhysics = 0;

  MapRoom* newMiscData = malloc(sizeof(MapRoom));
  newMiscData->state = roomstate_inactive;
  newMiscData->size = ROOM_SIZE;
  //newMiscData->gates[0] = 1;
  
  //ImportEnemyData(cursor.x, cursor.y, "Level1EnemyRanged2.txt", GetPlayerObject());

  ImportEnemyData(cursor.x + 5, cursor.y, "Level1EnemyMelee1.txt", GetPlayerObject());
  //ImportEnemyData(cursor.x - 5, cursor.y, "Level1EnemyMelee1.txt", GetPlayerObject());

  if (spawnGates) //assign gates where space exists
  {
    newRoom->simulate = &GateRoomSimulate;
    if (IsoTileGet(cursor.x + ROOM_SIZE / 2, cursor.y) == 2)
    {
      //GameObject* CreateWorldGate(Vector2D position);

      newMiscData->gates[0] = CreateWorldGate(Vec2(cursor.x + ROOM_SIZE / 2, cursor.y));
      //IsoTileSet(cursor.x + ROOM_SIZE / 2, cursor.y, 3);
    }
    else
    {
      newMiscData->gates[0] = NULL;
    }

    if (IsoTileGet(cursor.x - ROOM_SIZE / 2, cursor.y) == 2)
    {
      newMiscData->gates[1] = CreateWorldGate(Vec2(cursor.x - ROOM_SIZE / 2, cursor.y));
    }
    else
    {
      newMiscData->gates[1] = NULL;
    }

    if (IsoTileGet(cursor.x, cursor.y - ROOM_SIZE / 2) == 2)
    {
      newMiscData->gates[2] = CreateWorldGate(Vec2(cursor.x, cursor.y - ROOM_SIZE / 2));
    }
    else
    {
      newMiscData->gates[2] = NULL;
    }

    if (IsoTileGet(cursor.x, cursor.y + ROOM_SIZE / 2) == 2)
    {
      newMiscData->gates[3] = CreateWorldGate(Vec2(cursor.x, cursor.y + ROOM_SIZE / 2));
    }
    else
    {
      newMiscData->gates[3] = NULL;
    }
  }
  else
  {
    newRoom->simulate = NULL;
    for (int i = 0; i < 4; i++)
    {
      newMiscData->gates[i] = NULL;
    }
  }
  newRoom->miscData = newMiscData;
}

/*!
\brief Given a room object, opens all gates in the room (this kills the gates)

\param room Room object to open.
*/
void OpenRoom(GameObject* room)
{
  //safety
  if (room->type != entity_room)
  {
    printf("Why are you opening a non-room object");
    abort();
  }

  
  MapRoom* roomData = (MapRoom*)(room->miscData);
  int i = 0;
  while (i < 4)
  {
    GameObject* inst = roomData->gates[i];
    GateOpened(inst);
    i++;
  }
}

/*!
\brief Given a room object, closes all gates in the room (activating collisions)

\param room Room object to open.
*/
void CloseRoom(GameObject* room)
{
  //safety
  if (room->type != entity_room)
  {
    printf("Why are you closing a non-room object");
    abort();
  }


  MapRoom* roomData = (MapRoom*)(room->miscData);
  int i = 0;
  while (i < 4)
  {
    GameObject* inst = roomData->gates[i];
    if (inst)
    {
      printf("CLOSED");
      inst->sprite->tint.alpha = 1.f;
      inst->sprite->tint.red = 1.f;
      IsoTileSet(inst->physics->position.x, inst->physics->position.y, 1);
    }
    i++;
  }
}

/*!
\brief Getter to get size of a room
*/
int GetRoomSize(GameObject* room)
{
  if (room->type != entity_room)
  {
    abort();
  }

  MapRoom* roomData = (MapRoom*)(room->miscData);
  return roomData->size;
}


void EnemyKilled(GameObject* room)
{
  MapRoom* roomData = (MapRoom*)(room->miscData);
  if (roomData->numEnemies < 1)
  {
    OpenRoom(room);
  }
}