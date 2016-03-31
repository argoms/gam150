/*!
Project (working title): Epoch
\file   Gate.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains declarations for gates that close when a player enters a room.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "GameObject.h"
typedef struct WorldGate WorldGate;
enum WorldGateStates
{
  gate_closed,
  gate_open
};

GameObject* CreateWorldGate(Vector2D position);
void GateOpened(GameObject* DeadGate);
void GateRemoveEnemy(GameObject* Target);
void GateAddEnemy(GameObject* Target);
void GateRoomSimulate(GameObject* instance);
