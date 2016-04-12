#pragma once
#include "GameObject.h"

enum actions
{
  ENEMY_IDLE = 1,
  ENEMY_WALK = 2,
  ENEMY_ATTACK = 4,
  ENEMY_COOLDOWN = 8
};

void EnemyAnimationInitialize();

void EnemyAnimationStateManager(GameObject* enemy);