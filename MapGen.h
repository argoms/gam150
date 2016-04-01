/*!
Project (working title): Epoch
\file   MapGen.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Declarations for interface for procedurally generating game levels.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Isometric.h"
typedef struct GameObject GameObject;
typedef struct MapRoom MapRoom;
typedef struct Animation Animation;
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

void GenerateMap(IsoMap* inputMap);
void OpenRoom(GameObject* room);
int GetRoomSize(GameObject* room);
void CloseRoom(GameObject* room);
void EnemyKilled(GameObject* room);
Animation* GetGateAnimation();