#pragma once
#include "GameObject.h"

#define PI 3.1415926535897

enum
{
  ENEMY_STATE_IDLE,
  ENEMY_STATE_PATROL,
  ENEMY_STATE_CHASE
};

GameObject* EnemyCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type,
  int enemyType, float chaseSpeed, float detectRange, float knockback, float attackCooldown, float attackCooldownLength, float attackWindup, float attackWindupLength,
  float attackRange, float attackKnockbackForce, int attackDamage, float projectileSpeed);

void EnemyInitialize(GameObject* _thisObject);
void EnemySimulate(GameObject* _thisObject);
void EnemySimulateAI(GameObject* _thisObject);
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject);
void EnemyOnKilled(GameObject* _self);

void EnemyChangeAnimationFlag(EnemyContainer*, Vector2D* worldFacingDirection);

Vector2D EnemyMovement(GameObject* _thisObject, const float distanceToPlayer);
void EnemyAttackDetect(GameObject* _thisObject);
void EnemyMeleeAttack(GameObject* _thisObject, Vector2D attackDirection);
void EnemyRangedAttack(GameObject* _thisObject, Vector2D attackDirection, float projectileSpeed);
void EnemyTracerProjectileCollision(GameObject* _thisObject, GameObject* _otherObject);
void EnemyTracerSimulate(GameObject* _thisTracer);
static void EnemyKnockBack(GameObject* _thisObject, GameObject* _otherObject);