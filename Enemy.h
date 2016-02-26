#pragma once
#include "GameObject.h"

#define PI 3.1415926535897
#define ENEMY_ROTATION_SPEED  (2.0f*PI)
#define ENEMY_CHASE_SPEED     .05f
#define ENEMY_DETECT_RANGE    10.0f
#define ENEMY_KNOCKBACK_FORCE 0.25
#define ENEMY_MELEE_ATTACK_RANGE 3.0F

enum
{
  ENEMY_STATE_IDLE,
  ENEMY_STATE_PATROL,
  ENEMY_STATE_CHASE
};
static double attackCooldown; /**< timer before player can attack again*/

static double attackCooldownLength; /**< defined minimum time between attacks (attackCooldown is the timer)*/
static int attackDamage;
static Animation* tracerAnimation;

void EnemyInitialize();
void EnemySimulate(GameObject* _thisObject);
void EnemySimulateAI(GameObject* _thisObject);
void EnemyMeleeAttack(GameObject* _thisObject, Vector2D attackDirection);
void EnemyTracerFriendlyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject);
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject);
void EnemyOnKilled(GameObject* _self);

static void EnemyKnockBack(GameObject* _thisObject, GameObject* _otherObject);