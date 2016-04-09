#pragma once
#include "GameLevel.h"
#include "PlayerSmoke.h"
#include "EnvironmentalEffects.h"
#include "EnvironmentAssets.h"
#include "GameObject.h"
#include "PlayerEntity.h"
void UpdateSmokePosition(Vector2D worldPos);
void SetSmoke(int input);
void InitializePlayerSmoke();
GameObject* CreatePlayerSmoke();