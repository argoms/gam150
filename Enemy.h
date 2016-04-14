#pragma once
#include "GameObject.h"
#include "EnemyStateMachine.h"

//#define PI 3.1415926535897

enum
{
  ENEMY_STATE_IDLE,
  ENEMY_STATE_PATROL,
  ENEMY_STATE_CHASE,
  ENEMY_STATE_ATTACK,
  ENEMY_STATE_COOLDOWN
};

typedef struct
{
  int enemyType;    /* Enemy type, refer to enum in GameObject.h | REMEMBER THIS TAKES AN INTEGER NUMBER NOT A STRING */
  float size;       /* Size */

  int health;           /* Duh */
  float chaseSpeed;     /* Chasing speed */
  float patrolSpeed;    /* Default patroling state speed*/
  float detectRange;    /* Detect range */
  float knockbackForce; /* Knockback force when the enemy collides with the player, NOT the attack knockback */

  float attackCooldown;       /* Attack cooldown timer, set this to 0 as default, setting it to something else might end up with weird behavior */
  float attackCooldownLength; /* The actual length of the attack cooldown */
  float attackWindup;         /* Attack windup timer, set this to 0 as default, code will set this to attackWindupLength by default anyways */
  float attackWindupLength;   /* The actual length of the attack windup */
  float attackRange;          /* Attack range of enemy, applies to both melee and range enemies. Set it higher for ranged enemies, obviously */
  int attackDamage;           /* Self explanatory, applies to both melee and projectile enemies*/
  float attackKnockback;      /* Knockback the PLAYER feels when hit by an ENEMY'S attack */
  float enemyProjectileSpeed; /* Projectile speed for ranged enemies, set it to 0 for melee enemies obviously. Setting it to anything but 0 for melee enemies will do nothing anyways */
  Animation* enemyAnimation;
}EnemyInfo;

void EnemyHitBoxInitialize();

void EnemyImportInfo(int enemyType, const char *file);

GameObject* EnemyCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type,
  int enemyType, float chaseSpeed, float detectRange, float knockback, float attackCooldown, float attackCooldownLength, float attackWindup, float attackWindupLength,
  float attackRange, float attackKnockbackForce, int attackDamage, float projectileSpeed, int health);

GameObject* EnemySpawn(float x, float y, int enemyType, GameObject* player);

void EnemyInitialize(GameObject* _thisObject);
void EnemySimulate(GameObject* _thisObject);
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject);
void EnemyOnKilled(GameObject* _self);

void EnemyChangeAnimationFlag(EnemyContainer* container, Vector2D* worldFacingDirection);

void EnemyMeleeAttack(GameObject* _thisObject, Vector2D attackDirection);
void EnemyRangedAttack(GameObject* _thisObject, Vector2D attackDirection, float projectileSpeed);
void EnemyTracerProjectileCollision(GameObject* _thisObject, GameObject* _otherObject);
void EnemyTracerSimulate(GameObject* _thisTracer);
static void EnemyKnockBack(GameObject* _thisObject, GameObject* _otherObject);