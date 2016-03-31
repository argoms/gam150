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