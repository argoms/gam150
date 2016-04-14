/*!
Project (working title): Epoch
\file   PlayerEntity.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Declarations of functions from PlayerEntity.c
*/
#pragma once

#include "GameObject.h"
#include "stats.h"
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
void RestoreHealth(GameObject* obj);
void OnPlayerKilled(void);
statistics* GetPlayerStats();