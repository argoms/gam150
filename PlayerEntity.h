#pragma once

#include "GameObject.h"
void PlayerInit();
void PlayerSimulate();
void PlayerInput();
void TracerSimulate();
void TracerFriendlyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject);