/*!
Project (working title): Epoch
\file   EnemyAnimations.h
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright � 2016 DigiPen (USA) Corporation.
\brief
  Function declarations for EnemyAnimations.h
*/
#pragma once
#include "GameObject.h"

// Enemy animation state enums
enum actions
{
  ENEMY_IDLE = 1,
  ENEMY_WALK = 2,
  ENEMY_ATTACK = 4,
  ENEMY_COOLDOWN = 8
};

void EnemyAnimationInitialize();

void EnemyAnimationStateManager(GameObject* enemy);