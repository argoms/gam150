#include "Enemy.h"
#include "EnemyAnimations.h"
#include "GameLevel.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "Isometric.h"
#include "AEEngine.h"
#include <stdio.h>
#include <math.h>

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
  enemy->enemyAI->enemyState = ENEMY_STATE_PATROL;

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
  //printf("\n *** \n ENEMY DIED WOO \n *** \n");
  GameObjectDestroy(&_self);
  //printf("\n dflag: %i", (_self)->destroyFlag);
  //printf("type: %i \n", (_self)->type);
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

  if (distanceToPlayer <= enemyContainer->attackRange)
  {
    enemyContainer->enemyAnimationState = ENEMY_WALK;
    facingDirection.x = _thisObject->target->physics->position.x - _thisObject->physics->position.x;
    facingDirection.y = _thisObject->target->physics->position.y - _thisObject->physics->position.y;

    Vector2D normalizedFacing;
    Vector2DNormalize(&normalizedFacing, &facingDirection);
    Vector2D velocityVector;
    Vector2DScale(&velocityVector, &normalizedFacing, enemyContainer->chaseSpeed);

    _thisObject->physics->velocity.x = velocityVector.x;
    _thisObject->physics->velocity.y = velocityVector.y;
   
    Vector2D worldFacing;
    worldFacing = IsoWorldToScreen(&facingDirection);
    EnemyChangeAnimationFlag(enemyContainer, &worldFacing);

    facingDirection = normalizedFacing;
    Vector2D enemyToPlayer;
    enemyToPlayer.x = _thisObject->target->physics->position.x - _thisObject->physics->position.x;
    enemyToPlayer.y = _thisObject->target->physics->position.y - _thisObject->physics->position.y;

    Vector2D normalVelocityVector;
    normalVelocityVector.x = _thisObject->physics->velocity.y * -1.0f;
    normalVelocityVector.y = _thisObject->physics->velocity.x;

    float dotProduct = Vector2DDotProduct(&(normalVelocityVector), &(enemyToPlayer));
    //printf("%f", dotProduct);

    if (dotProduct > 0)
    {
      _thisObject->physics->angle += ENEMY_ROTATION_SPEED * (float)AEFrameRateControllerGetFrameTime();
      //printf("ROTATION");
    }
    else if (dotProduct < 0)
    {
      _thisObject->physics->angle -= ENEMY_ROTATION_SPEED * (float)AEFrameRateControllerGetFrameTime();
      //printf("ROTATION");
    }

    Vector2D newVelocityVector;
    Vector2DFromAngleRad(&newVelocityVector, _thisObject->physics->angle);
    facingDirection = newVelocityVector;
    Vector2DScale(&newVelocityVector, &newVelocityVector, enemyContainer->chaseSpeed);

    _thisObject->physics->velocity.x = newVelocityVector.x;
    _thisObject->physics->velocity.y = newVelocityVector.y;
  }
  return facingDirection;
}

void EnemyAttackDetect(GameObject* _thisObject)
{
  float distanceToPlayer = Vector2DSquareDistance(&(_thisObject->physics->position), &(_thisObject->target->physics->position));
  Vector2D attackDirection = EnemyMovement(_thisObject, distanceToPlayer);
  EnemyContainer* enemyContainer = _thisObject->miscData;

  if (distanceToPlayer <= enemyContainer->attackRange)
  {
    if (enemyContainer->attackWindup > 0)
    {
      enemyContainer->attackWindup -= AEFrameRateControllerGetFrameTime();
    }

    if (enemyContainer->attackWindup < 0)
    {
      enemyContainer->attackCooldown -= AEFrameRateControllerGetFrameTime();
    }

    //printf("ATTACKWINDUP: %F \n", enemyContainer->attackWindup);
    //printf("ATTACKCOOLDOWN: %F \n", enemyContainer->attackCooldown);
    if (enemyContainer->attackWindup < 0 && enemyContainer->attackCooldown < 0)
    {
      //printf("ENEMY ATTACK");
      enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
      enemyContainer->attackWindup = enemyContainer->attackWindupLength;
      switch (enemyContainer->enemyType)
      {
      case ENEMY_TYPE_MELEE:
        EnemyMeleeAttack(_thisObject, attackDirection);
        //printf("%i, %i", attackDirection.x, attackDirection.y);
        break;
      case ENEMY_TYPE_RANGED:
        EnemyRangedAttack(_thisObject, attackDirection, enemyContainer->projectileSpeed);
        //printf("%i, %i", attackDirection.x, attackDirection.y);
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
  //tracer->simulate = &TracerSimulate;
  tracer->physics->onCollision = &EnemyTracerProjectileCollision;
}

void EnemyTracerProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  //EnemyContainer* enemyContainer = _thisObject->miscData;
  if (_otherObject && _otherObject->type == entity_player && (_otherObject->entity->canBeDamaged))
  {
    printf("ENEMY HIT PLAYER %i DAMAGE\n", 10);
    EntityTakeDamage(&_otherObject->entity, 10);
    GameObjectDestroy(&_thisObject);
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


// IGNORE THIS CRAP
/*
Vector2D enemyToPlayer;
enemyToPlayer.x = _thisObject->target->physics->position.x - _thisObject->physics->position.x;
enemyToPlayer.y = _thisObject->target->physics->position.y - _thisObject->physics->position.y;

Vector2D normalVelocityVector;
normalVelocityVector.x = _thisObject->physics->velocity.y * -1.0f;
normalVelocityVector.y = _thisObject->physics->velocity.x;

float dotProduct = Vector2DDotProduct(&(normalVelocityVector), &(enemyToPlayer));
//printf("%f", dotProduct);

if (dotProduct > 0)
{
_thisObject->physics->angle += ENEMY_ROTATION_SPEED * (float)AEFrameRateControllerGetFrameTime();
//printf("ROTATION");
}
else if (dotProduct < 0)
{
_thisObject->physics->angle -= ENEMY_ROTATION_SPEED * (float)AEFrameRateControllerGetFrameTime();
//printf("ROTATION");
}

Vector2D newVelocityVector;
Vector2DFromAngleRad(&newVelocityVector, _thisObject->physics->angle);
facingDirection = newVelocityVector;
Vector2DScale(&newVelocityVector, &newVelocityVector, enemyContainer->chaseSpeed);
_thisObject->physics->velocity.x = newVelocityVector.x;
_thisObject->physics->velocity.y = newVelocityVector.y;
*/
