#pragma once
#include "GameObject.h"

void HealthPickupInitialize();

void HealthPickupSpawn(float x, float y);

void HealthPickupCollide(GameObject* thisObject, GameObject* otherObject);