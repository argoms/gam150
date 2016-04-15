/*!
Project (working title): Epoch
\file   EnemyAIMeleeChargerBehavior.c
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  Enemy AI behaior for the charging enemy with long stabby arms
*/
#include "EnemyAIMeleeChargerBehavior.h"
#include "EnemyAnimations.h"
#include "Isometric.h"
#include "AEEngine.h"

/****************PATROL******************/

/*!
\brief Handles entering patrol state, stays still and sets animation

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
}

/*!
\brief Enters the chase state if detects the player

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (distanceToPlayer <= enemyContainer->detectRange)
  {
    //printf("DETECTEDPLAYER\n");
    enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
  }
}

/*!
\brief When exiting the patrol state strips the animation

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_IDLE;
}

/****************CHASE******************/

/*!
\brief Sets the chase animation when entering this state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_WALK;
}

/*!
\brief Chases after the player and if within attack range switches to attack state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_ChaseUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  Vector2D facingDirection;
  facingDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
  facingDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;

  Vector2DNormalize(&facingDirection, &facingDirection);
  Vector2DScale(&(enemy->physics->velocity), &facingDirection, enemyContainer->chaseSpeed);

  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));
  if (distanceToPlayer <= enemyContainer->attackRange)
  {
    enemy->enemyAI->newEnemyState = ENEMY_STATE_ATTACK;
  }

  if (distanceToPlayer > enemyContainer->detectRange)
  {
    enemy->enemyAI->newEnemyState = ENEMY_STATE_PATROL;
  }
}

/*!
\brief Strips the animation state when exiting the chase state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_WALK;
}

/****************ATTACK******************/

/*!
\brief Sets attack animation when entering the attack state, makes the enemy stay still, and updates
the special lookDirection vector2d

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_ATTACK;
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  enemy->sprite->frame = 0;

  enemyContainer->lookDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
  enemyContainer->lookDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;
  Vector2DNormalize(&(enemyContainer->lookDirection), &(enemyContainer->lookDirection));
}

/*!
\brief Handles the "jumping" attack behavior of the charger enemy

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (enemyContainer->attackWindup > 0)
  {
    enemyContainer->attackWindup -= (float)AEFrameRateControllerGetFrameTime();
  }

  if (enemyContainer->attackWindup <= .62f && enemyContainer->attackWindup > 0.6f && (enemy->physics->velocity.x == 0 && enemy->physics->velocity.y == 0))
  {
    Vector2D attackDirection;
    attackDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
    attackDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;
    Vector2DNormalize(&attackDirection, &attackDirection);

    enemy->physics->velocity.x = attackDirection.x;
    enemy->physics->velocity.y = attackDirection.y;
    Vector2DScale(&(enemy->physics->velocity), &(enemy->physics->velocity), .1f);
  }

  if (enemyContainer->attackWindup <= 0)
  {

    enemy->physics->velocity.x = 0;
    enemy->physics->velocity.y = 0;
    enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;

    EnemyMeleeAttack(enemy, enemyContainer->lookDirection);
    enemy->enemyAI->newEnemyState = ENEMY_STATE_COOLDOWN;
  }
}

/*!
\brief Strips animation state when exiting attack, resets sprite to 0, resets attackwindup timer

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_ATTACK;
  enemyContainer->attackWindup = enemyContainer->attackWindupLength;
  enemy->sprite->frame = 0;
}

//******************* COOL DOWN *********************//

/*!
\brief When entering the cool down state sets appropriate animation

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_COOLDOWN;
}

/*!
\brief Handles cooldown state, to handle the "cooldown" state of this enemy's attack

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  if (enemyContainer->attackCooldown > 0)
  {
    enemyContainer->attackCooldown -= (float)AEFrameRateControllerGetFrameTime();
  }

  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (enemyContainer->attackCooldown <= 0)
  {
    if (distanceToPlayer > enemyContainer->attackRange)
    {
      enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
      return;
    }

    enemy->enemyAI->newEnemyState = ENEMY_STATE_ATTACK;
  }
}

/*!
\brief Resets the attack cooldown timer and strips the animation when exiting the cooldown state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeCharger_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~(ENEMY_COOLDOWN);
}