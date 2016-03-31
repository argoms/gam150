#include "Enemy.h"
#include "EnemyAnimations.h"
#include "GameLevel.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "Isometric.h"
#include "AEEngine.h"
#include <stdio.h>
#include <math.h>
#include "ParticleSystems(Redo).h"

static Animation* tracerAnimation;

/*
\brief
It's prety huge but deal with it because we want full access to everything an enemy could ever possibly do....
James' idea, not mine

Don't actually call this method. Use the import enemy data function instead, which calls this anyways

//KILL ME
\param
	a metrik fuk ton
*/
GameObject* EnemyCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type,
  int enemyType, float chaseSpeed, float detectRange, float knockback, float attackCooldown, float attackCooldownLength, float attackWindup, float attackWindupLength,
  float attackRange, float attackKnockbackForce, int attackDamage, float projectileSpeed)
{
  GameObject* enemy = GameObjectCreate(_physics, _sprite, _entity, _type);

  enemy->enemyAI = (EnemyAI*)malloc(sizeof(EnemyAI));
  enemy->enemyAI->currentEnemyState = ENEMY_STATE_PATROL;

  EnemyContainer* enemyContainer = (EnemyContainer*)malloc(sizeof(EnemyContainer));
  enemyContainer->enemyType = enemyType;
  enemyContainer->chaseSpeed = chaseSpeed;
  enemyContainer->detectRange = detectRange;
  enemyContainer->knockback = knockback;
  enemyContainer->attackCooldown = attackCooldown;
  enemyContainer->attackCooldownLength = attackCooldownLength;
  enemyContainer->attackWindup = attackWindup;
  enemyContainer->attackWindupLength = attackWindupLength;
  enemyContainer->attackRange = attackRange;
  enemyContainer->attackKnockbackForce = attackKnockbackForce;
  enemyContainer->attackDamage = attackDamage;
  enemyContainer->projectileSpeed = projectileSpeed;

  enemyContainer->animationCooldown = .2f;

  enemy->miscData = enemyContainer;

  return enemy;
}

void EnemyInitialize(GameObject* _thisObject)
{

  if (_thisObject->miscData == NULL)
  {
    printf("No enemy container");
    return;
  }

  /*Melee attack visual effect*/
  tracerAnimation = GCreateAnimation(1,
    GCreateTexture("isotilePlaceholder1.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);
}

void EnemySimulate(GameObject* _thisObject)
{
  EnemySimulateAI(_thisObject);
  EnemyAnimationStateManager(_thisObject);
}

/*
\brief
Handles enemy AI
\param _thisObject
The game object we want to simulate ai
*/
void EnemySimulateAI(GameObject* _thisObject)
{
  EnemyAttackDetect(_thisObject);
}

/*!
\brief example/default collision event function

\param _thisObject pointer to gameobject of obect 1
\param _otherObject pointer to gameobject of object 2
*/
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  //printf("%i||", _thisObject->type);
  if (_thisObject->type == entity_enemy && _otherObject->type == entity_player)
  {
    //GameObjectDestroy(&_thisObject);
    //LevelSetNext(level_mainMenu);
    EnemyKnockBack(_thisObject, _otherObject);
    EntityTakeDamage(&(_otherObject->entity), 2);

    //printf("PLAYER TAKING DAMAGE \n");
  }
}

/*!
\brief called on enemy's death

\param _self pointer to enemy object being killed
*/
void EnemyOnKilled(GameObject* _self)
{
  GameObjectDestroy(&_self);
  pPS_B->PS_Burst->StartPosX = _self->physics->position.x;
  pPS_B->PS_Burst->StartPosY = _self->physics->position.y;
  Start_PS(pPS_B);
}

void EnemyChangeAnimationFlag(EnemyContainer* enemyContainer, Vector2D* worldFacingDirection)
{

  double worldFacingAngle = atan2(worldFacingDirection->y, worldFacingDirection->x) * (180.0f / PI);
  //printf("%f\n", worldFacingAngle);
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & (~(ENEMY_LEFT + ENEMY_RIGHT + ENEMY_UP + ENEMY_DOWN));
  
  if (worldFacingAngle < 67.5 && worldFacingAngle > -67.5)
  {
    //printf("right");
    //printf("%f\n", worldFacingAngle);
    enemyContainer->enemyAnimationState += ENEMY_RIGHT;
  }
  
  if (worldFacingAngle > 112.5 || worldFacingAngle < -112.5)
  {
    //printf("left\n");
    enemyContainer->enemyAnimationState += ENEMY_LEFT;
  }

  if (worldFacingAngle > 22.5 && worldFacingAngle < 157.5)
  {
    //printf("up\n");
    enemyContainer->enemyAnimationState += ENEMY_UP;
  }
  if (worldFacingAngle < -22.5 && worldFacingAngle > -157.5)
  {
    //printf("down\n");
    enemyContainer->enemyAnimationState += ENEMY_DOWN;
  }
  
}

/*
\brief Handles enemy movement
\param _thisObject the enemy that moves
\returns
Its current facing direction
*/
Vector2D EnemyMovement(GameObject* _thisObject, const float distanceToPlayer)
{
  Vector2D facingDirection = _thisObject->physics->velocity;
  EnemyContainer* enemyContainer = _thisObject->miscData;

  if (_thisObject->enemyAI->currentEnemyState == ENEMY_STATE_ATTACK)
  {
    _thisObject->physics->velocity.x = 0;
    _thisObject->physics->velocity.y = 0;
      facingDirection.x = _thisObject->target->physics->position.x - _thisObject->physics->position.x;
      facingDirection.y = _thisObject->target->physics->position.y - _thisObject->physics->position.y;
      Vector2DNormalize(&facingDirection, &facingDirection);
  }
  else if (distanceToPlayer <= enemyContainer->detectRange && distanceToPlayer >= enemyContainer->attackRange)
  {
    enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_IDLE;
    enemyContainer->enemyAnimationState = ENEMY_WALK;
    facingDirection.x = _thisObject->target->physics->position.x - _thisObject->physics->position.x;
    facingDirection.y = _thisObject->target->physics->position.y - _thisObject->physics->position.y;

    Vector2DNormalize(&facingDirection, &facingDirection);
    Vector2DScale(&(_thisObject->physics->velocity), &facingDirection, enemyContainer->chaseSpeed);
  }
  else
  {
    _thisObject->physics->velocity.x = 0;
    _thisObject->physics->velocity.y = 0;
    enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_WALK;
    
    if (distanceToPlayer <= enemyContainer->attackRange)
    {
      facingDirection.x = _thisObject->target->physics->position.x - _thisObject->physics->position.x;
      facingDirection.y = _thisObject->target->physics->position.y - _thisObject->physics->position.y;
      Vector2DNormalize(&facingDirection, &facingDirection);
    }
    else
    {
      facingDirection.x = -1;
      facingDirection.y = -1;
    }
    enemyContainer->enemyAnimationState = ENEMY_IDLE;
  }

  Vector2D worldFacing;
  worldFacing = IsoWorldToScreen(&facingDirection);
  EnemyChangeAnimationFlag(enemyContainer, &worldFacing);
  return facingDirection;
}

void EnemyAttackDetect(GameObject* _thisObject)
{
  EnemyContainer* enemyContainer = _thisObject->miscData;
  float distanceToPlayer = Vector2DSquareDistance(&(_thisObject->physics->position), &(_thisObject->target->physics->position));
  Vector2D attackDirection = EnemyMovement(_thisObject, distanceToPlayer);

  if (enemyContainer->animationCooldown <= 0 && _thisObject->enemyAI->currentEnemyState == ENEMY_STATE_ATTACK && distanceToPlayer <= enemyContainer->detectRange)
  {
    _thisObject->enemyAI->currentEnemyState = ENEMY_STATE_PATROL;
    enemyContainer->animationCooldown = .2f;
  }

  if (enemyContainer->animationCooldown > 0 && _thisObject->enemyAI->currentEnemyState == ENEMY_STATE_ATTACK)
  {
    enemyContainer->animationCooldown -= AEFrameRateControllerGetFrameTime();
  }

  if (distanceToPlayer <= enemyContainer->attackRange)
  {
    if (enemyContainer->attackWindup > 0)
    {
      enemyContainer->attackWindup -= AEFrameRateControllerGetFrameTime();
    }

    if (enemyContainer->attackWindup <= 0)
    {
      enemyContainer->attackCooldown -= AEFrameRateControllerGetFrameTime();
    }

    printf("%f,", enemyContainer->attackCooldown);

    if (enemyContainer->attackWindup < 0 && enemyContainer->attackCooldown < 0)
    {
      //printf("ENEMY ATTACK");
      enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
      enemyContainer->attackWindup = enemyContainer->attackWindupLength;
      enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~(ENEMY_WALK + ENEMY_IDLE);
      enemyContainer->enemyAnimationState = ENEMY_ATTACK;
      _thisObject->enemyAI->currentEnemyState = ENEMY_STATE_ATTACK;

      switch (enemyContainer->enemyType)
      {
      case ENEMY_TYPE_MELEE:
        EnemyMeleeAttack(_thisObject, attackDirection);
        break;
      case ENEMY_TYPE_RANGED:
        EnemyRangedAttack(_thisObject, attackDirection, enemyContainer->projectileSpeed);
        break;
      }
    }
  }
  else
  {
    enemyContainer->attackWindup = enemyContainer->attackWindupLength;
    enemyContainer->attackCooldown = 0;
  }
}

void EnemyMeleeAttack(GameObject* _thisObject, Vector2D attackDirection)
{
  GameObject* tracer = GameObjectCreate(
    PhysicsCreateObject(Vec2(_thisObject->physics->position.x + attackDirection.x, _thisObject->physics->position.y + attackDirection.y), 1),
    GCreateSprite(0, 40, tracerAnimation, 1), 0, entity_enemyProjectile);
  tracer->syncSpritePhysics = 1;
  //tracer->simulate = &TracerSimulate;
  tracer->physics->onCollision = &EnemyTracerProjectileCollision;
}

void EnemyRangedAttack(GameObject* _thisObject, Vector2D attackDirection, float projectileSpeed)
{
  GameObject* tracer = GameObjectCreate(
    PhysicsCreateObject(Vec2(_thisObject->physics->position.x + attackDirection.x, _thisObject->physics->position.y + attackDirection.y), 1),
    GCreateSprite(0, 40, tracerAnimation, 1), 0, entity_enemyProjectile);

  //Vector2DNormalize(&(attackDirection), &(attackDirection));

  tracer->physics->velocity.x = attackDirection.x * projectileSpeed;
  tracer->physics->velocity.y = attackDirection.y * projectileSpeed;

  tracer->syncSpritePhysics = 1;
  tracer->simulate = &EnemyTracerSimulate;
  tracer->physics->onCollision = &EnemyTracerProjectileCollision;
}

void EnemyTracerProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  //EnemyContainer* enemyContainer = _thisObject->miscData;
  if (_otherObject && _otherObject->type == entity_player && (_otherObject->entity && _otherObject->entity->canBeDamaged))
  {
    printf("ENEMY HIT PLAYER %i DAMAGE\n", 10);
    EntityTakeDamage(&_otherObject->entity, 10);
    GameObjectDestroy(&_thisObject);
  }
}

void EnemyTracerSimulate(GameObject* _thisTracer)
{
  _thisTracer->projectileLifeTime -= (float)AEFrameRateControllerGetFrameTime();

  if (_thisTracer->projectileLifeTime < 0)
  {
    GameObjectDestroy(&_thisTracer);
  }
}

/*
*/
static void EnemyKnockBack(GameObject* _thisObject, GameObject* _otherObject)
{
  Vector2D knockbackVector; //first, create the vector
  EnemyContainer* enemyContainer = _thisObject->miscData;
  Vector2DSub(&knockbackVector, &_thisObject->physics->position, &_otherObject->physics->position); //based on difference between characters

                                                                                                    //scale vector according to ENEMY_KNOCKBACK_FORCE:
  Vector2DNormalize(&knockbackVector, &knockbackVector);
  Vector2DScale(&knockbackVector, &knockbackVector, -enemyContainer->knockback);

  //actually apply knockback:
  EntityApplyKnockback(_otherObject->entity, &knockbackVector);
}