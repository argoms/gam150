#pragma once

#include "GameObject.h"
#define LEFT_CLICK 1
#define RIGHT_CLICK 2
void PlayerInit();
void PlayerSimulate();
void PlayerInput();
void TracerSimulate();
void TracerFriendlyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject);
void PlayerAnimations();

void IncrementPlayerDrag();
void DecrementPlayerDrag();
void SetPlayerDrag(float drag);
float  GetPlayerDrag();