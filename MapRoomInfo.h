/*!
Project (working title): Epoch
\file   MapRoomInfo.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Declarations/interface for MapRoomInfo linked list objects containing information used during mapgen in MapGen.c

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Vector2D.h"

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