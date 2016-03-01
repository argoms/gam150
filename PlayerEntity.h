#pragma once

#include "GameObject.h"

#define PLAYERS_KNOCKBACK_FORCE .25f
void PlayerInit();
void PlayerSimulate();
void PlayerInput();
void TracerSimulate();
void TracerEnemyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject);