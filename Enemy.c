#include "Enemy.h"
#include "GameLevel.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "AEEngine.h"
#include <stdio.h>


void EnemyInitialize()
{
  attackCooldown = 0;
  attackCooldownLength = 1.0;
  attackDamage = 10;
  /*Melee attack visual effect*/
  tracerAnimation = GCreateAnimation(1,
    GCreateTexture("isotilePlaceholder1.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);
}

void EnemySimulate(GameObject* _thisObject)
{
  attackCooldown -= AEFrameRateControllerGetFrameTime();
  EnemySimulateAI(_thisObject);
}

/*
\brief 
Handles enemy AI

\param _thisObject
The game object we want to simulate ai
*/
void EnemySimulateAI(GameObject* _thisObject)
{
  /* This is all basically modified CS230 stuff*/
  float distanceToPlayer = Vector2DSquareDistance(&(_thisObject->physics->position), &(_thisObject->target->physics->position));

  if (distanceToPlayer <= ENEMY_DETECT_RANGE)
  {
    Vector2D enemyToPlayer;
    enemyToPlayer.x = _thisObject->target->physics->position.x - _thisObject->physics->position.x;
    enemyToPlayer.y = _thisObject->target->physics->position.y - _thisObject->physics->position.y;

    Vector2D normalVelocityVector;
    normalVelocityVector.x = _thisObject->physics->velocity.y * -1.0f;
    normalVelocityVector.y = _thisObject->physics->velocity.x;

    float dotProduct = Vector2DDotProduct(&(normalVelocityVector), &(enemyToPlayer));

    if (dotProduct > 0)
    {
      _thisObject->physics->angle -= ENEMY_ROTATION_SPEED * (float)AEFrameRateControllerGetFrameTime();
      //printf("ROTATION");
    }
    else if (dotProduct < 0)
    {
      _thisObject->physics->angle += ENEMY_ROTATION_SPEED * (float)AEFrameRateControllerGetFrameTime();
      //printf("ROTATION");
    }

    Vector2D newVelocityVector;
    Vector2DFromAngleRad(&newVelocityVector, _thisObject->physics->angle);
    Vector2D attackDirection = newVelocityVector;
    Vector2DScale(&newVelocityVector, &newVelocityVector, ENEMY_CHASE_SPEED);

    _thisObject->physics->velocity.x = newVelocityVector.x;
    _thisObject->physics->velocity.y = newVelocityVector.y;
    //printf("DETECTED PLAYER");

    if (distanceToPlayer <= ENEMY_MELEE_ATTACK_RANGE)
    {
      if (attackCooldown < 0)
        EnemyMeleeAttack(_thisObject, attackDirection);
    }
  }
}

void EnemyMeleeAttack(GameObject* _thisObject, Vector2D attackDirection)
{
  attackCooldown = attackCooldownLength;
  GameObject* tracer = GameObjectCreate(
    PhysicsCreateObject(Vec2(_thisObject->physics->position.x + attackDirection.x, _thisObject->physics->position.y + attackDirection.y), 1),
    GCreateSprite(0, 40, tracerAnimation, 1), 0, entity_friendlyProjectile);
  tracer->syncSpritePhysics = 1;
  //tracer->simulate = &TracerSimulate;
  tracer->physics->onCollision = &EnemyTracerFriendlyProjectileCollision;
}

void EnemyTracerFriendlyProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  //printf("%i", _otherObject->type);

  if (_otherObject && _otherObject->type == entity_player)
  {
    printf("ENEMY HIT PLAYER %i DAMAGE\n", attackDamage);
    EntityTakeDamage(&_otherObject->entity, attackDamage);
    GameObjectDestroy(&_thisObject);
  }
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
    
    EntityTakeDamage(&(_otherObject->entity), 2);

    EnemyKnockBack(_thisObject, _otherObject);

    printf("PLAYER TAKING DAMAGE \n");
  }
}

/*!
\brief called on enemy's death

\param _self pointer to enemy object being killed
*/
void EnemyOnKilled(GameObject* _self)
{
  printf("\n *** \n ENEMY DIED WOO \n *** \n");
  GameObjectDestroy(&_self);
  printf("\n dflag: %i", (_self)->destroyFlag);
  printf("type: %i \n", (_self)->type);
}


/*
*/
static void EnemyKnockBack(GameObject* _thisObject, GameObject* _otherObject)
{
  Vector2D knockbackVector; //first, create the vector
  Vector2DSub(&knockbackVector, &_thisObject->physics->position, &_otherObject->physics->position); //based on difference between characters

                                                                                                    //scale vector according to ENEMY_KNOCKBACK_FORCE:
  Vector2DNormalize(&knockbackVector, &knockbackVector);
  Vector2DScale(&knockbackVector, &knockbackVector, -ENEMY_KNOCKBACK_FORCE);

  //actually apply knockback:
  EntityApplyKnockback(_otherObject->entity, &knockbackVector);
}