#pragma once
#include "GameObject.h"
typedef struct WorldGate WorldGate;

GameObject* CreateWorldGate(Vector2D position);
void GateOpened(GameObject* DeadGate);
void GateRemoveEnemy(GameObject* Target);
void GateAddEnemy(GameObject* Target);
void GateRoomSimulate(GameObject* instance);