/*!
Project (working title): Epoch
\file   MapRoomInfo.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Declarations/interface for functionality on the map-room scope of map generation.
Includes MapRoomInfo linked list objects containing information used during mapgen in MapGen.c
*/
#pragma once
#include "Vector2D.h"
#include "MapGen.h"
typedef struct MapRoomInfo MapRoomInfo;
/*!
\struct MapRoomInfo
\brief linked list for some room info stuff
*/
struct MapRoomInfo
{
  Vector2D position;
  int type;
  MapRoomInfo* next;

};

void MapRoomInfoAdd(MapRoomInfo* first, Vector2D position, int type);
void MapRoomInfoClear(MapRoomInfo* first);

//room-specific behaviors:
void MapRoomBehavior_BasicEnemies(MapRoom* roomData);
void MapRoomBehavior_SmallRoom(MapRoom* roomData);