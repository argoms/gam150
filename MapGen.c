/*!
Project (working title): Epoch
\file   MapGen.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functions for procedurally generating game levels.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/

//#include "MapGen.h"
#include "Vector2D.h"
#include "Isometric.h"
#include "AEEngine.h"
#include "GameLevel.h"
#include "Door.h"
#include "Gate.h"
#include "MyRandom.h"
#include "MapRoomInfo.h"
#include "GameObject.h"
#include "MapGen.h"
#include "conversions.h"
#include "EnvironmentalEffects.h"

//private info 
#define NUM_ROOMS 12

//Think of this as MAX room size, not just room size.
static int ROOM_SIZE = 9;//22; /**< Room size, subtract 2 from this due to increased wall thickness*/
static int MAP_SEED = 32;


//state enums for each room
enum MapGen_RoomStates
{
  roomstate_inactive, 
  roomstate_active,
  roomstate_cleared
};

enum MapGen_RoomTypes
{
  roomtype_start,
  roomtype_simple,
  roomtype_hall
};

enum directions
{
  dir_left,
  dir_right,
  dir_up,
  dir_down
};

//static function prototypes:
static void FillLine(Vector2D position1, Vector2D position2, int newValue);
static Vector2D directionOffsetGet(int dir);
//static void FillArea(int x, int y);
static int isValid(Vector2D position, IsoMap* map);
static void IsoSquareSet(Vector2D position, int newValue);
static void Room_BasicEnemies(Vector2D cursor);
static GameObject* RoomTemplate(Vector2D cursor, int spawnGates);
static void Room_StartRoom(Vector2D cursor);
static void SetupBaseMap(int mapWidth, int mapHeight);
static int RoomValid(Vector2D cursor, int mapW, int mapH);
static void SpawnMapRooms(MapRoomInfo* rooms);
static void ReplaceTiles(Vector2D position, Vector2D size, int oldTile, int newTile);
static void Room_HallsRoom(Vector2D cursor);

static Animation* GateAnimationHorizontal;

static Animation* GateAnimationVertical;
//implementation:
void GenerateMap(IsoMap* inputMap)
{
  //AEGfxGetCamPosition
  //printf("A");
  GateAnimationHorizontal = GCreateAnimation(1,
    GCreateTexture("animations/world/gateHorizontal.png"),
    GCreateMesh(256.f, 256.f, 1, 1),
    1);
  //printf("B");
  GateAnimationVertical = GCreateAnimation(1,
    GCreateTexture("animations/world/gateVertical.png"),
    GCreateMesh(256.f, 256.f, 1, 1),
    1);
  //printf("C");
  int mapHeight = inputMap->mapHeight;
  int mapWidth = inputMap->mapWidth;
  int i = 0;
  int j = 0;

  MAP_SEED =  rand();
  RandSeed(MAP_SEED);
  printf("\n \n \n GENERATING MAP WITH SEED %i AND %i ROOMS\n \n \n", MAP_SEED, NUM_ROOMS);

  //MapRoom* rooms = malloc(sizeof(MapRoom) * NUM_ROOMS);
  SetupBaseMap(mapWidth, mapHeight);
  //create path of rooms:

  Vector2D cursor = Vec2(ROOM_SIZE / 2, ROOM_SIZE / 2); //create a "cursor" for the rooms, starting at the spawn room
  int rooms_created = 1;
  int tries = 0; //infinite loop killer

  //set up start room:
  MapRoomInfo* rooms = malloc(sizeof(MapRoomInfo));
  rooms->next = NULL;
  rooms->type = roomtype_start;
  rooms->position = cursor;

  //roomsNum++;

  //start creating rooms:
  while (rooms_created < NUM_ROOMS && tries++ < NUM_ROOMS * 100)
  {
    int direction = (int)(RandFloat() * 4); //random direction
    Vector2D offset = directionOffsetGet(direction); //turn that random stupid number into an actual random direction
    Vector2DScale(&offset, &offset, ROOM_SIZE);

    Vector2D newCursorPosition;
    Vector2DAdd(&newCursorPosition, &cursor, &offset); //travel one room size in that random direction

    if (isValid(newCursorPosition, inputMap) && RoomValid(newCursorPosition, mapWidth, mapHeight) && IsoTileGet((int)newCursorPosition.x, (int)newCursorPosition.y) != 1)
    {
      //now bear with me here, first we're checking if the room we want to go into already exists
      if (IsoTileGet((int)newCursorPosition.x, (int)newCursorPosition.y) == 2)
      {
        //if it does, there's a 50% chance that we don't do anything with it
        if (RandFloat() < 0.5)
        {
          continue;
        }
          //otherwise, we branch back into it (this doesn't count as creating a room)
          //what this does is allows rooms to OCCASIONALLY loop onto themselves without always choosing to form loops

        //fill the path between rooms:
        FillLine(cursor, newCursorPosition, tile_path);
        cursor = newCursorPosition;
      }
      else
      {
        //fill the path between rooms & update cursor:
        FillLine(cursor, newCursorPosition, tile_path);
        cursor = newCursorPosition;
        
        //a room has been created! what do?
        rooms_created++;
        
        if (rooms_created == NUM_ROOMS)
        {
          //behavior specific to end (final) room
          DoorCreateDoorAt(cursor);
          MapRoomInfoAdd(rooms, cursor, roomtype_hall);
        }

        else
        {
          MapRoomInfoAdd(rooms, cursor, RandIntRange(1, 3));
        }
        continue;
      }
    }
  }

  //discount exceptions/infinite loop handling
  if (tries > NUM_ROOMS * 100)
  {
    printf("MapGen.c: seriously your algorithm is fucked, look at it.");
    //abort();
    //GenerateMap(inputMap); //comment this back in for release, but while debugging keep it out to avoid recursion.
  }

  SpawnMapRooms(rooms);
  MapRoomInfoClear(rooms); //clean up after done
  
}

/*!
\brief Given a linked list of MapRoomInfo objects, creates relevant map data
*/
static void SpawnMapRooms(MapRoomInfo* rooms)
{
  MapRoomInfo* index = rooms;
  while (index)
  {
    switch (index->type)
    {
    case roomtype_start:
      Room_StartRoom(index->position);
      printf("made a start room");
      break;
    case roomtype_simple:
      Room_BasicEnemies(index->position);
      printf("enemyroom\n");
      break;
    case roomtype_hall:
      Room_HallsRoom(index->position);
      printf("halls");
      break;
    }
    index = index->next;
  }
}
/*!
\brief Sets up a basic map grid with ROOM_SIZE-sized rooms separated by walls.
*/
static void SetupBaseMap(int mapWidth, int mapHeight)
{
  int i = 0;
  int j = 0;
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
        IsoTileSet(i, j, tile_wall);
      }
      else
      {
        IsoTileSet(i, j, tile_empty);
        //printf("a");
      }
      j++;

    }
    i++;
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
    i++; 
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

///*!
//\brief Unused at the moment.
//*/
//static void FillArea(int x, int y, int tileType)
//{
//  
//}


/*!
\brief Replaces a given tile with another given tile in an area. If the tile to replace is tile_any, all tiles will be replaced
*/
static void ReplaceTiles(Vector2D position, Vector2D size, int oldTile, int newTile)
{
  int overrideAll = (oldTile == tile_any); //if overrideall, just replaces everything.
  Vector2D index = size; // a 2D loop counter, yup.

  //index through everything
  while (index.x != 0 || index.y != 0)
  {
    //cursor: tile we're currently looking at
    Vector2D cursor = Vec2(position.x + index.x, position.y + index.y);

    if (overrideAll || IsoTileGet(cursor.x, cursor.y) == oldTile)
    {
      IsoTileSet(cursor.x, cursor.y, newTile);
    }

    //indexing 2D-style
    index.x--;
    if (index.x < 0)
    {
      index.x = size.x;
      index.y--;
    }
  }
}

/*!
\brief Room-specific setup functions for a basic room with enemies in it.
*/
static void Room_BasicEnemies(Vector2D cursor)
{
  
  GameObject* room = RoomTemplate(cursor, 1);
  MapRoom* roomData = (MapRoom*)(room->miscData);
  roomData->type = roomtype_simple;
}


/*!
\brief Room-specific setup for the starting spawn room.
*/
static void Room_StartRoom(Vector2D cursor)
{
  GameObject* room = RoomTemplate(cursor, 0);
  MapRoom* roomData = (MapRoom*)(room->miscData);
  roomData->type = roomtype_start;
}

/*!
\brief Room-specific setup for empty halls
*/
static void Room_HallsRoom(Vector2D cursor)
{
  GameObject* room = RoomTemplate(cursor, 0);
  MapRoom* roomData = (MapRoom*)(room->miscData);
  roomData->type = roomtype_hall;
  ReplaceTiles(Vec2(cursor.x - ROOM_SIZE / 2, cursor.y - ROOM_SIZE / 2), Vec2(ROOM_SIZE, ROOM_SIZE), tile_floor, tile_wall);
}

/*!
\brief Basic room setup/initialization
\param cursor Location to set up room.
\param spawnGates whether or not to spawn gate objects.
*/
static GameObject* RoomTemplate(Vector2D cursor, int spawnGates)
{
  //replace empty with floor
  ReplaceTiles(Vec2(cursor.x - ROOM_SIZE / 2, cursor.y - ROOM_SIZE / 2), Vec2(ROOM_SIZE, ROOM_SIZE), tile_empty, tile_floor);

  //set up the game object
  GameObject* newRoom = GameObjectCreate(PhysicsCreateObject(cursor, 0), 0, 0, entity_room); 
  
  newRoom->syncSpritePhysics = 0;

  //set up custom room "component"
  MapRoom* newMiscData = malloc(sizeof(MapRoom));
  newMiscData->state = roomstate_inactive;
  newMiscData->size = ROOM_SIZE;
  newMiscData->position = cursor;
  newMiscData->parent = newRoom;
  //newMiscData->gates[0] = 1;
  
  //ImportEnemyData(cursor.x, cursor.y, "Level1EnemyRanged2.txt", GetPlayerObject());

  //ImportEnemyData(cursor.x, cursor.y, "Level1EnemyMelee1.txt", GetPlayerObject());
  //ImportEnemyData(cursor.x - 5, cursor.y, "Level1EnemyMelee1.txt", GetPlayerObject());

  if (spawnGates) //assign gates where space exists
  {
    newRoom->simulate = &GateRoomSimulate;

    //if a tile at a given border is a path, make a gate
    if (IsoTileGet(cursor.x + ROOM_SIZE / 2, cursor.y) == 2)
    {
      newMiscData->gates[0] = CreateWorldGate(Vec2(cursor.x + ROOM_SIZE / 2, cursor.y), gate_vertical);
    }
    else
    {
      newMiscData->gates[0] = NULL;
    }

    if (IsoTileGet(cursor.x - ROOM_SIZE / 2, cursor.y) == 2)
    {
      newMiscData->gates[1] = CreateWorldGate(Vec2(cursor.x - ROOM_SIZE / 2, cursor.y), gate_vertical);
    }
    else
    {
      newMiscData->gates[1] = NULL;
    }

    if (IsoTileGet(cursor.x, cursor.y - ROOM_SIZE / 2) == 2)
    {
      newMiscData->gates[2] = CreateWorldGate(Vec2(cursor.x, cursor.y - ROOM_SIZE / 2), gate_horizontal);
    }
    else
    {
      newMiscData->gates[2] = NULL;
    }

    if (IsoTileGet(cursor.x, cursor.y + ROOM_SIZE / 2) == 2)
    {
      newMiscData->gates[3] = CreateWorldGate(Vec2(cursor.x, cursor.y + ROOM_SIZE / 2), gate_horizontal);
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

  return newRoom;
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
    if (inst)
    {
      GateOpened(inst);
    }
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

  
  switch (roomData->type)
  {
  case roomtype_simple:
    MapRoomBehavior_BasicEnemies(roomData);
    break;
  }

  int i = 0;
  //loop through the 4 potential door positions (since rooms are square)
  while (i < 4)
  {
    GameObject* inst = roomData->gates[i];
    if (inst)
    {
      printf("CLOSED");
      inst->sprite->tint.alpha = 0.5f;
      inst->sprite->tint.red = 1.f;
      IsoTileSet(inst->physics->position.x, inst->physics->position.y, tile_wall);
      
      GateClosed(inst);

      switch(GetWorldGate(inst)->orientation)
      {
      case gate_horizontal:
        IsoTileSet(inst->physics->position.x + 1, inst->physics->position.y, tile_wall);
        IsoTileSet(inst->physics->position.x - 1, inst->physics->position.y, tile_wall);
        break;
      case gate_vertical:
        IsoTileSet(inst->physics->position.x, inst->physics->position.y + 1, tile_wall);
        IsoTileSet(inst->physics->position.x, inst->physics->position.y - 1, tile_wall);
        break;
      }
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
  //printf("\n thiswascalled \n \n");
  MapRoom* roomData = (MapRoom*)(room->miscData);
  roomData->numEnemies--;
  if (roomData->numEnemies == 0)
  {
    OpenRoom(room);
  }
}

/*!
\brief Getter for a gate animation object so gates don't all have to create their own animations per create call.
*/
Animation* GetGateAnimation(int orientation)
{
  switch (orientation)
  {
  case gate_horizontal:
    return GateAnimationHorizontal;
    break;
  case gate_vertical:
    return GateAnimationVertical;
    break;
  default:
    return NULL;
    break;
  }
  
}

/*!
\brief checks if an area around room vector is valid to place a room in (is in bounds)
*/
static int RoomValid(Vector2D cursor, int mapW, int mapH)
{
  int roomRad = (ROOM_SIZE / 2);
  return !(cursor.x - roomRad< 0 || cursor.y - roomRad < 0 || cursor.x + roomRad> mapW || cursor.y + roomRad> mapH);
}