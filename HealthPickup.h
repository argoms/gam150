/*!
Project (working title): Epoch
\file   HealthPickup.h
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  Function declarations for HealthPickup.c
*/
#pragma once
#include "GameObject.h"

void HealthPickupInitialize();

void HealthPickupSpawn(float x, float y);

void HealthPickupCollide(GameObject* thisObject, GameObject* otherObject);