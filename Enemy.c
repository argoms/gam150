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

static Animation* tracerAnimation;

static EnemyInfo enemyInfo[] =
{
  { ENEMY_TYPE_MELEE        , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }, //0
  { ENEMY_TYPE_MELEE_BIG    , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }, //1
  { ENEMY_TYPE_MELEE_CHARGE , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }, //2
  { ENEMY_TYPE_RANGED       , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }, //3
  { ENEMY_TYPE_HEALER       , 1, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL }  //4
};

//call once
void EnemyImportInfo(int enemyType, const char *file)
{
  //GameObject* newEnemy;
  FILE *infile = fopen(file, "r");
  if (infile)
  {
    printf("OPENED FILE\n");
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

                                  //Animation* enemyAnimation; /* Forward declaration for the enemy animation */

      fscanf(infile, "ENEMY - %i\n", &enemyTypeNumber);
      /*
      // Handles unchangable enemy information
      // Each enemy will only have one animation set specific to it anyways
      switch (enemyType)
      {
      case 1:
      enemyType = ENEMY_TYPE_MELEE;
      // REPLACE THE BELOW WITH PROPER ENEMY SPRITE/ANIMATION INFORMATION
      enemyAnimation = GCreateAnimation(1,
      GCreateTexture("isotilePlaceholder1.png"),
      GCreateMesh(128.f, 64.f, 1, 1),
      1);
      break;
      case 2:
      enemyType = ENEMY_TYPE_MELEE_BIG;
      enemyAnimation = GCreateAnimation(1,
      GCreateTexture("isotilePlaceholder1.png"),
      GCreateMesh(128.f, 64.f, 1, 1),
      1);
      break;
      case 3:
      enemyType = ENEMY_TYPE_MELEE_CHARGE;
      break;
      case 4:
      enemyType = ENEMY_TYPE_RANGED;
      enemyAnimation = GCreateAnimation(1,
      GCreateTexture("isotilePlaceholder1.png"),
      GCreateMesh(128.f, 64.f, 1, 1),
      1);
      break;
      case 5:
      enemyType = ENEMY_TYPE_HEALER;
      break;
      }
      */
      fscanf(infile, "  SIZE     - %f\n", &size);
      fscanf(infile, "  HEALTH   - %i\n", &health);

      fscanf(infile, "  CHASE SPEED     - %f\n", &chaseSpeed);
      fscanf(infile, "  PATROL SPEED    - %f\n", &patrolSpeed);
      fscanf(infile, "  DETECT RANGE    - %f\n", &detectRange);
      fscanf(infile, "  KNOCKBACK FORCE - %f\n", &knockbackForce);

      fscanf(infile, "  ENEMY ATTACK COOLDOWN        - %f\n", &attackCooldown);
      fscanf(infile, "  ENEMY ATTACK COOLDOWN LENGTH - %f\n", &attackCooldownLength);
      fscanf(infile, "  ENEMY ATTACK WINDUP          - %f\n", &attackWindup);
      fscanf(infile, "  ENEMY ATTACK WINDUP LENGTH   - %f\n", &attackWindupLength);
      fscanf(infile, "  ENEMY ATTACK RANGE           - %f\n", &attackRange);
      fscanf(infile, "  ENEMY ATTACK DAMAGE          - %f\n", &attackDamage);
      fscanf(infile, "  ENEMY ATTACK KNOCKBACK FORCE - %f\n", &attackKnockback);
      fscanf(infile, "  ENEMY PROJECTILE SPEED       - %f\n", &enemyProjectileSpeed);

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
      //printf("%f                      ------\n", enemyInfo[0].chaseSpeed);
    }
    fclose(infile);
    printf("CLOSED FILE\n");
  }
}

/*
\brief
Creates an enemy

Don't actually call this method. Use EnemySpawn function instead, which calls this anyways

//KILL ME
\param
a metrik fuk ton
*/
GameObject* EnemyCreate(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _type,
  int enemyType, float chaseSpeed, float detectRange, float knockback, float attackCooldown, float attackCooldownLength, float attackWindup, float attackWindupLength,
  float attackRange, float attackKnockbackForce, int attackDamage, float projectileSpeed, int health)
{
  GameObject* enemy = GameObjectCreate(_physics, _sprite, _entity, _type);

  enemy->enemyAI = (EnemyAI*)malloc(sizeof(EnemyAI));
  enemy->enemyAI->currentEnemyState = ENEMY_STATE_PATROL;
  enemy->enemyAI->EnemyStateStart = EnemyAI_Melee_PatrolStart;
  enemy->enemyAI->EnemyStateUpdate = EnemyAI_Melee_PatrolUpdate;
  enemy->enemyAI->EnemyStateExit = EnemyAI_Melee_PatrolExit;

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

GameObject* EnemySpawn(float x, float y, int enemyType, GameObject* player)
{
  //printf("ENEMY SPAWNED HERE: %f, %f \n", x, y);
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

  Animation* enemyAnimation = GCreateAnimation(1,
    GCreateTexture("isotilePlaceholder1.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);
  GameObject* newEnemy = EnemyCreate(PhysicsCreateObject(Vec2(x, y), size), GCreateSprite(0, 40, enemyAnimation, 1), enemyEntity, entity_enemy, enemyType,
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

  return newEnemy;
}



void EnemyInitialize(GameObject* _thisObject)
{

  if (_thisObject->miscData == NULL)
  {
    printf("Why no enemy, sucka");
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
  if (_thisObject->type == entity_enemy && _otherObject->type == entity_player)
  {
    EnemyKnockBack(_thisObject, _otherObject);
    EntityTakeDamage(&(_otherObject->entity), 2);
  }
}

/*!
\brief called on enemy's death

\param _self pointer to enemy object being killed
*/
void EnemyOnKilled(GameObject* _self)
{
  GameObjectDestroy(&_self);
  //pPS_B->PS_Burst->StartPosX = _self->physics->position.x;
  //pPS_B->PS_Burst->StartPosY = _self->physics->position.y;
  //Start_PS(pPS_B);
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

void EnemyTracerProjectileCollision(GameObject* _thisObject, GameObject* _otherObject)
{
  if (_otherObject && _otherObject->type == entity_player && (_otherObject->entity && _otherObject->entity->canBeDamaged))
  {
    //EnemyContainer* enemyContainer = _thisObject->parent->miscData;
    EntityTakeDamage(&_otherObject->entity, 10);
    GameObjectDestroy(&_thisObject);
  }
}

void EnemyTracerSimulate(GameObject* _thisTracer)
{
  if (_thisTracer->projectileLifeTime < 0)
  {
    GameObjectDestroy(&_thisTracer);
  }
  _thisTracer->projectileLifeTime -= (float)AEFrameRateControllerGetFrameTime();

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