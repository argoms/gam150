/*!
Project (working title): Epoch
\file   Enemy.c
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  Handles enemy creation, importing enemy data for txt files, collision for enemies, enemy attacks,
  and enemy death
*/
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
#include "EnemyAIMeleeBehavior.h"
#include "EnemyAIMeleeBigBehavior.h"
#include "EnemyAIMeleeChargerBehavior.h"
#include "Audio.h"

// To hold the hitbox animation, which is supposed to be invisible
static Animation* tracerAnimation;

// Static that holds onto all enemy information for the game session
static EnemyInfo enemyInfo[] =
{
  { ENEMY_TYPE_MELEE        , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }, //0
  { ENEMY_TYPE_MELEE_BIG    , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }, //1
  { ENEMY_TYPE_MELEE_CHARGE , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }, //2
  { ENEMY_TYPE_RANGED       , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }, //3
  { ENEMY_TYPE_HEALER       , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }  //4
};

/*!
\brief Setup of hitbox used in enemy attacks
*/
void EnemyHitBoxInitialize()
{
  tracerAnimation = GCreateAnimation(1,
    GCreateTexture("animations/isotilePlaceholder1.png"),
    GCreateMesh(128.f, 64.f, 10, 1),
    1);
}

/*!
\brief Imports data from txt files for enemies

\param enemyType Should take an enum that specifies what the enemy type is
\param file The name of the text file
*/
void EnemyImportInfo(int enemyType, const char *file)
{
  FILE *infile;
  fopen_s(&infile, file, "r");
  if (infile)
  {
    while (!feof(infile))
    {
      int enemyTypeNumber;    /* Enemy type, refer to enum in GameObject.h | REMEMBER THIS TAKES AN INTEGER NUMBER NOT A STRING */
      float size;       /* Size */

      int health;         /* Duh */
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

      fscanf_s(infile, "ENEMY - %i\n", &enemyTypeNumber);
      fscanf_s(infile, "  SIZE     - %f\n", &size);
      fscanf_s(infile, "  HEALTH   - %i\n", &health);

      fscanf_s(infile, "  CHASE SPEED     - %f\n", &chaseSpeed);
      fscanf_s(infile, "  PATROL SPEED    - %f\n", &patrolSpeed);
      fscanf_s(infile, "  DETECT RANGE    - %f\n", &detectRange);
      fscanf_s(infile, "  KNOCKBACK FORCE - %f\n", &knockbackForce);

      fscanf_s(infile, "  ENEMY ATTACK COOLDOWN        - %f\n", &attackCooldown);
      fscanf_s(infile, "  ENEMY ATTACK COOLDOWN LENGTH - %f\n", &attackCooldownLength);
      fscanf_s(infile, "  ENEMY ATTACK WINDUP          - %f\n", &attackWindup);
      fscanf_s(infile, "  ENEMY ATTACK WINDUP LENGTH   - %f\n", &attackWindupLength);
      fscanf_s(infile, "  ENEMY ATTACK RANGE           - %f\n", &attackRange);
      fscanf_s(infile, "  ENEMY ATTACK DAMAGE          - %i\n", &attackDamage);
      fscanf_s(infile, "  ENEMY ATTACK KNOCKBACK FORCE - %f\n", &attackKnockback);
      fscanf_s(infile, "  ENEMY PROJECTILE SPEED       - %f\n", &enemyProjectileSpeed);

      int arraySize = sizeof(enemyInfo) / sizeof(EnemyInfo);
      for (int i = 0; i < arraySize; i++)
      {
        if (enemyInfo[i].enemyType == enemyType)
        {
          enemyInfo[i].size = size;
          enemyInfo[i].health = health;
          enemyInfo[i].chaseSpeed = chaseSpeed;
          enemyInfo[i].patrolSpeed = patrolSpeed;
          enemyInfo[i].detectRange = detectRange;
          enemyInfo[i].knockbackForce = knockbackForce;

          enemyInfo[i].attackCooldown = attackCooldown;
          enemyInfo[i].attackCooldownLength = attackCooldownLength;
          enemyInfo[i].attackWindup = attackWindup;
          enemyInfo[i].attackWindupLength = attackWindupLength;
          enemyInfo[i].attackRange = attackRange;
          enemyInfo[i].attackDamage = attackDamage;
          enemyInfo[i].attackKnockback = attackKnockback;
          enemyInfo[i].enemyProjectileSpeed = enemyProjectileSpeed;
        }
      }
    }
    fclose(infile);
    printf("CLOSED FILE\n");
  }
}

/*
\brief
Creates an enemy

\param _physics 
  Physics component
\param _sprite 
  Sprite component
\param _entity 
  Entity component
\param _type 
  Enum for the entity type
\param enemyType 
  enum for the enemy type (SEPARATE FROM ENTITY TYPE)
\param chaseSpeed 
  Chase speed of the enemy
\param detectRange 
  detection range of the enemy
\param knockback 
  Knockback of enemy
\param attackCooldown 
  Internal timer for attackcooldown
\param attackCooldownLength 
  The actual length of the attackcooldown
\param attackWindup 
  Internal timer for attackwindup
\param attackRange 
  Attack range of enemy
\param attackKnockbackForce 
  Knockback force of enemy
\param projectileSpeed 
  Unused
\param health 
  Health of enemy
*/
GameObject* EnemyCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type,
  int enemyType, float chaseSpeed, float detectRange, float knockback, float attackCooldown, float attackCooldownLength, float attackWindup, float attackWindupLength,
  float attackRange, float attackKnockbackForce, int attackDamage, float projectileSpeed, int health)
{
  GameObject* enemy = GameObjectCreate(_physics, _sprite, _entity, _type);

  enemy->enemyAI = (EnemyAI*)malloc(sizeof(EnemyAI));
  enemy->enemyAI->currentEnemyState = ENEMY_STATE_PATROL;

  switch (enemyType)
  {
    case ENEMY_TYPE_MELEE:
      enemy->enemyAI->EnemyStateStart = EnemyAI_Melee_PatrolStart;
      enemy->enemyAI->EnemyStateUpdate = EnemyAI_Melee_PatrolUpdate;
      enemy->enemyAI->EnemyStateExit = EnemyAI_Melee_PatrolExit;
    break;
    case ENEMY_TYPE_MELEE_BIG:
      enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeBig_PatrolStart;
      enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeBig_PatrolUpdate;
      enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeBig_PatrolExit;
    break;
    case ENEMY_TYPE_MELEE_CHARGE:
      enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeCharger_PatrolStart;
      enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeCharger_PatrolUpdate;
      enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeCharger_PatrolExit;
    break;
  }

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
  
  Vector2D lookDirection;
  lookDirection.x = 0;
  lookDirection.y = 0;
  enemyContainer->lookDirection = lookDirection;

  //enemyContainer->animationCooldown = .2f;

  enemy->miscData = enemyContainer;

  return enemy;
}

/*
\brief
  SPAWNS an enemy. Uses EnemyCreate and initializes necessary values to spawn an enemy at a desired location

\param x
  X position to put the enemy
\param y
  Y position to put the enemy
\param enemyType
  Enemy type which is an enum
\param player
  Pointer to the player
*/
GameObject* EnemySpawn(float x, float y, int enemyType, GameObject* player)
{
  float size;       /* Size */

  int health;         /* Duh */
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
  float enemyProjectileSpeed;

  int arraySize = sizeof(enemyInfo) / sizeof(EnemyInfo);
  for (int i = 0; i < arraySize; i++)
  {
    if (enemyInfo[i].enemyType == enemyType)
    {
      size = enemyInfo[i].size;
      health = enemyInfo[i].health;
      chaseSpeed = enemyInfo[i].chaseSpeed;
      patrolSpeed = enemyInfo[i].patrolSpeed;
      detectRange = enemyInfo[i].detectRange;
      knockbackForce = enemyInfo[i].knockbackForce;

      attackCooldown = enemyInfo[i].attackCooldown;
      attackCooldownLength = enemyInfo[i].attackCooldownLength;
      attackWindup = enemyInfo[i].attackWindup;
      attackWindupLength = enemyInfo[i].attackWindupLength;
      attackRange = enemyInfo[i].attackRange;
      attackDamage = enemyInfo[i].attackDamage;
      attackKnockback = enemyInfo[i].attackKnockback;
      enemyProjectileSpeed = enemyInfo[i].enemyProjectileSpeed;
    }
  }

  Entity* enemyEntity = malloc(sizeof(Entity));
  enemyEntity->maxHealth = health;
  EntityInit(&enemyEntity);

/*  Animation* enemyAnimation = GCreateAnimation(1,
    GCreateTexture("animations/isotilePlaceholder1.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);   */  
  GameObject* newEnemy = EnemyCreate(PhysicsCreateObject(Vec2(x, y), size), GCreateSprite(0, 40, NULL, 1), enemyEntity, entity_enemy, enemyType,
    chaseSpeed, detectRange, knockbackForce, attackCooldown, attackCooldownLength,
    attackWindup, attackWindupLength, attackRange, attackKnockback, attackDamage, enemyProjectileSpeed, health);

  //Now we initialize function pointers and give our enemy a target
  newEnemy->physics->onCollision = &EnemyOnCollision;
  newEnemy->simulate = &EnemySimulate;
  newEnemy->initialize = &EnemyInitialize;
  newEnemy->entity->onEntityKilled = &EnemyOnKilled;
  newEnemy->target = GetPlayerObject();
  newEnemy->initialize(newEnemy);
  newEnemy->type = entity_enemy;

  newEnemy->entity->health = health;

  if (newEnemy->enemyAI)
  {
    printf("ENEMY SPAWNED\n");
  }
  else
  {
    printf("WHY NO AI\n");
  }

  return newEnemy;
}


/*
\brief
  Enemy's initialize. Really it's a safety check to make sure we're initializing an enemy
\param _thisObject
  Hopefully this is an enemy object
*/
void EnemyInitialize(GameObject* _thisObject)
{

  if (_thisObject->miscData == NULL)
  {
    printf("Why no enemy, sucka");
    return;
  }


}

/*
\brief
  Runs the AI state machine and the Animation state machine
\param _thisObject
  Hopefully this is an enemy object
*/
void EnemySimulate(GameObject* _thisObject)
{
  ESMachineRun(_thisObject);
  EnemyAnimationStateManager(_thisObject);
}


/*!
\brief example/default collision event function

\param _thisObject pointer to gameobject of obect 1
\param _otherObject pointer to gameobject of object 2
*/
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  if (_thisObject->type == entity_enemy && (_otherObject->type == entity_player || _otherObject->type == entity_enemy))
  {
    EnemyKnockBack(_thisObject, _otherObject);
    //EntityTakeDamage(&(_otherObject->entity), 2);
  }
}

/*!
\brief called on enemy's death

\param _self pointer to enemy object being killed
*/
void EnemyOnKilled(GameObject* _self)
{
  Audio_PlaySoundSample("SwordScrape.ogg", 0);
  GameObjectDestroy(&_self);
}

/*
\brief
  Melee attack for enemy's, spawns a hitbox 
\param _thisObject
  Hopefully this is an enemy object that is using this
\param attackDirection
  The direction to spawn the hitbox
*/
void EnemyMeleeAttack(GameObject* _thisObject, Vector2D attackDirection)
{
  GameObject* tracer = GameObjectCreate(
    PhysicsCreateObject(Vec2(_thisObject->physics->position.x + attackDirection.x, _thisObject->physics->position.y + attackDirection.y), 1),
    GCreateSprite(0, 40, tracerAnimation, 1), 0, entity_enemyProjectile);
  tracer->syncSpritePhysics = 1;
  tracer->simulate = &EnemyTracerSimulate;
  tracer->projectileLifeTime = 0;
  tracer->physics->onCollision = &EnemyTracerProjectileCollision;
  tracer->parent = _thisObject;
}

/*
\brief
  Ranged attack for enemy's, spawns a hitbox
\param _thisObject
  Hopefully this is an enemy object that is using this
\param attackDirection
  The direction to spawn the hitbox
\param projectileSpeed
  Projectile speed of the projectile to spawn
*/
void EnemyRangedAttack(GameObject* _thisObject, Vector2D attackDirection, float projectileSpeed)
{
  GameObject* tracer = GameObjectCreate(
    PhysicsCreateObject(Vec2(_thisObject->physics->position.x + attackDirection.x, _thisObject->physics->position.y + attackDirection.y), 1),
    GCreateSprite(0, 40, tracerAnimation, 1), 0, entity_enemyProjectile);

  tracer->physics->velocity.x = attackDirection.x * projectileSpeed;
  tracer->physics->velocity.y = attackDirection.y * projectileSpeed;

  tracer->syncSpritePhysics = 1;
  tracer->simulate = &EnemyTracerSimulate;
  tracer->physics->onCollision = &EnemyTracerProjectileCollision;
  tracer->parent = _thisObject;
}

/*
\brief
  The collision logic for hitbox which handles damaging the player when they are hit by it
\param _thisObject
  Hopefully the hitbox
\param _otherObject
  Otherobject to resolve a collision check
*/
void EnemyTracerProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  if (_otherObject && _otherObject->type == entity_player && (_otherObject->entity && _otherObject->entity->canBeDamaged))
  {
    EnemyContainer* enemyContainer = _thisObject->parent->miscData;
    EntityTakeDamage(&_otherObject->entity, enemyContainer->attackDamage);
    GameObjectDestroy(&_thisObject);
  }
}

/*
\brief
  Despawns the hitbox in case the enemy misses
\param _thisTracer
  The hitbox to handle lifetime of
*/
void EnemyTracerSimulate(GameObject* _thisTracer)
{
  if (_thisTracer->projectileLifeTime < 0)
  {
    GameObjectDestroy(&_thisTracer);
  }
  _thisTracer->projectileLifeTime -= (float)AEFrameRateControllerGetFrameTime();
}

/*
\brief
  Used when resolve collisions between enemies and player
\param _thisObject
  Hopefully an enemy object
\param _otherObject
  Other object to resolve collision check against
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

// DEPRECATED OLD ANIMATION CODE
// NEW ANIMATION CODE HOOKS INTO JAMES' ANIMATION SWITCHER
void EnemyChangeAnimationFlag(EnemyContainer* enemyContainer, Vector2D* worldFacingDirection)
{
  //double worldFacingAngle = atan2(worldFacingDirection->y, worldFacingDirection->x) * (180.0f / PI);
  ////printf("%f\n", worldFacingAngle);
  ////enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & (~(ENEMY_LEFT + ENEMY_RIGHT + ENEMY_UP + ENEMY_DOWN));

  //if (worldFacingAngle < 67.5 && worldFacingAngle > -67.5)
  //{
  //  //printf("right");
  //  //printf("%f\n", worldFacingAngle);
  //  enemyContainer->enemyAnimationState += ENEMY_RIGHT;
  //}

  //if (worldFacingAngle > 112.5 || worldFacingAngle < -112.5)
  //{
  //  //printf("left\n");
  //  enemyContainer->enemyAnimationState += ENEMY_LEFT;
  //}

  //if (worldFacingAngle > 22.5 && worldFacingAngle < 157.5)
  //{
  //  //printf("up\n");
  //  enemyContainer->enemyAnimationState += ENEMY_UP;
  //}
  //if (worldFacingAngle < -22.5 && worldFacingAngle > -157.5)
  //{
  //  //printf("down\n");
  //  enemyContainer->enemyAnimationState += ENEMY_DOWN;
  //}

}