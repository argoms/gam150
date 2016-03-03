#include "GameObject.h"

enum directions
{
  ENEMY_LEFT = 1,
  ENEMY_RIGHT = 2,
  ENEMY_UP = 4,
  ENEMY_DOWN = 8
};

enum actions
{
  ENEMY_IDLE = 16,
  ENEMY_WALK = 32,
  ENEMY_SWORD = 64
};

void EnemyAnimationInitialize(GameObject* enemy);

void EnemyAnimationStateManager(GameObject* enemy);