/*!
Project (working title): Epoch
\file   EnemyAIMeleeBigBehavior.c
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  Enemy AI behavior for the Big melee enemy with club
*/
#include "EnemyAIMeleeBigBehavior.h"
#include "EnemyAnimations.h"
#include "Isometric.h"
#include "AEEngine.h"
#include "ScreenShake.h"

/****************PATROL******************/
/*!
\brief Enemy patrol start, should stay still on initial

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
}

/*!
\brief Constantly checks to detect the player

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (distanceToPlayer <= enemyContainer->detectRange)
  {
    enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
  }
}

/*!
\brief Strips the animation when exiting the state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_IDLE;
}

/****************CHASE******************/

/*!
\brief Sets the animation when entering the chasing state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_WALK;
}

/*!
\brief Chases after the player and detects when in attack range

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_ChaseUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
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
\brief When exiting chase state, strips the animation

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_WALK;
}

/****************ATTACK******************/

/*!
\brief Stays still when entering the attack state and sets the animation,
also utilizes the special lookdirection vector2d to orient specific animations

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->sprite->frame = 0;
  enemyContainer->enemyAnimationState = ENEMY_ATTACK;

  enemyContainer->lookDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
  enemyContainer->lookDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;
  Vector2DNormalize(&(enemyContainer->lookDirection), &(enemyContainer->lookDirection));
}

/*!
\brief Handles attackwindup when in the attack state, when attackwindup is done plays the attack

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
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
    enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;

    AddScreenShake(0.1f, 200);
    EnemyMeleeAttack(enemy, enemyContainer->lookDirection);
    enemy->enemyAI->newEnemyState = ENEMY_STATE_COOLDOWN;
  }
}

/*!
\brief When exiting the attack state, strips the animation and resets sprite frames and attackwindup

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_ATTACK;
  enemyContainer->attackWindup = enemyContainer->attackWindupLength;
  enemy->sprite->frame = 0;
}

//******************* COOL DOWN *********************//

/*!
\brief Sets the animation for entering the cooldown state

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_COOLDOWN;
}

/*!
\brief Handles cooldown state by checking against the cooldown timer,
switches back to correct states after cooldown is done

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
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
\brief Handles cooldown exit, resets cooldown timer and strips animation

\param enemy The enemy gameobject
\param enemyContainer EnemyContainer component with enemy information
*/
void EnemyAI_MeleeBig_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~(ENEMY_COOLDOWN);
}