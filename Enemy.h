#pragma once
#include "GameObject.h"

#define PI 3.1415926535897
#define ENEMY_ROTATION_SPEED  (2.0f*PI)
#define ENEMY_CHASE_SPEED     .05f
#define ENEMY_DETECT_RANGE    10.0f
#define ENEMY_KNOCKBACK_FORCE 0.25
enum
{
  ENEMY_STATE_IDLE,
  ENEMY_STATE_PATROL,
  ENEMY_STATE_CHASE
};
void EnemySimulate(GameObject* _thisObject);
void EnemySimulateAI(GameObject* _thisObject);
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject);
void EnemyOnKilled(GameObject* _self);