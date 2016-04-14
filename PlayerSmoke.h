/*!
Project (working title): Epoch
\file   PlayerSmoke.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Wrapper around particles to specifically manage the smoke trailing effect that the player (sometimes) has.
*/
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
void CreatePlayerSmokePuff(int size);