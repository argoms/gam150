/*!
Project (working title): Epoch
\file   Gate.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Contains declarations for gates that close when a player enters a room.

*/
#pragma once
#include "GameObject.h"

#define GATE_LENGTH 3
typedef struct WorldGate WorldGate;
enum WorldGateStates
{
  gate_closed,
  gate_open,
  gate_vertical,
  gate_horizontal
};
struct WorldGate
{
  int status;
  int orientation;
  int positionX;
  int positionY;

  GameObject* particleSystems[GATE_LENGTH]; 
};

void GateClosed(GameObject* inst);
GameObject* CreateWorldGate(Vector2D position, int orientation);
void GateOpened(GameObject* DeadGate);
void GateRemoveEnemy(GameObject* Target);
void GateAddEnemy(GameObject* Target);
void GateRoomSimulate(GameObject* instance);
WorldGate* GetWorldGate(GameObject* input);;
void GateSimulate(GameObject* inst);