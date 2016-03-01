#pragma once
#include "GameObject.h"

#define PI 3.1415926535897
#define ENEMY_ROTATION_SPEED  (2.0f*PI + 1)

enum
{
  ENEMY_STATE_IDLE,
  ENEMY_STATE_PATROL,
  ENEMY_STATE_CHASE
};

/*i'M SO SORRY */
GameObject* EnemyCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type,
  int enemyType, float chaseSpeed, float detectRange, float knockback, float attackCooldown, float attackCooldownLength, float attackWindup, float attackWindupLength,
  float attackRange, float attackKnockbackForce, int attackDamage, float projectileSpeed);
//GameObject* EnemyCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type);
void EnemyInitialize(GameObject* _thisObject);
void EnemySimulate(GameObject* _thisObject);
void EnemySimulateAI(GameObject* _thisObject);
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject);
void EnemyOnKilled(GameObject* _self);

/*Enemy's AI here*/
Vector2D EnemyMovement(GameObject* _thisObject, const float distanceToPlayer);
void EnemyAttackDetect(GameObject* _thisObject);
void EnemyMeleeAttack(GameObject* _thisObject, Vector2D attackDirection);
void EnemyRangedAttack(GameObject* _thisObject, Vector2D attackDirection, float projectileSpeed);
void EnemyTracerProjectileCollision(GameObject* _thisObject, GameObject* _otherObject);
static void EnemyKnockBack(GameObject* _thisObject, GameObject* _otherObject);