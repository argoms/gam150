/*!
Project: Epoch
\file   EnemyAIMeleeBehavior.c
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  AI Behavior for the Spider enemy
*/
#include "EnemyAIMeleeBehavior.h"
#include "EnemyAnimations.h"
#include "Isometric.h"
#include "AEEngine.h"

/****************PATROL******************/

/*!
\brief Called when entering the patrol state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
}

/*!
\brief Constantly called for the Patrol state, simply detects for the player's presence

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (distanceToPlayer <= enemyContainer->detectRange)
  {
    enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
  }
}

/*!
\brief When exiting the patrol state, sets the animation state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_IDLE;
}

/****************CHASE******************/

/*!
\brief Sets enemy walk animation when entering the chase state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_WALK;
}

/*!
\brief Constantly chases after the player

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_ChaseUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
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
\brief Strip the animaton when exiting chase state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_WALK;

}

/****************ATTACK******************/

/*!
\brief Assign new animation and reset the frames

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_ATTACK;
  enemy->sprite->frame = 0;
}

/*!
\brief Handles cooldown and windup when attacking

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (enemyContainer->attackWindup > 0)
  {
    enemyContainer->attackWindup -= (float)AEFrameRateControllerGetFrameTime();
  }

  if (enemyContainer->attackWindup <= 0)
  {
    Vector2D attackDirection;
    attackDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
    attackDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;
    Vector2DNormalize(&attackDirection, &attackDirection);

    attackDirection.x = attackDirection.x / 2.0f;
    attackDirection.y = attackDirection.y / 2.0f;

    enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;

    EnemyMeleeAttack(enemy, attackDirection);
    enemy->enemyAI->newEnemyState = ENEMY_STATE_COOLDOWN;
  }
}

/*!
\brief Resets the animation states and resets the attackwindup

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_ATTACK;
  enemyContainer->attackWindup = enemyContainer->attackWindupLength;
}

//******************* COOL DOWN *********************//s

/*!
\brief Sets the animation state when entering cooldown, which is idle and resets the frame

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
  enemy->sprite->frame = 0;
}

/*!
\brief Constantly checks against the cooldown timer, while in cooldown stays still

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
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
\brief Resets attackcooldown and strips the animation state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_Melee_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~(ENEMY_IDLE);
}