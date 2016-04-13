#include "EnemyAIMeleeBehavior.h"
#include "EnemyAnimations.h"
#include "Isometric.h"
#include "AEEngine.h"

/****************IDLE******************/

void EnemyAI_Melee_IdleStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
}

void EnemyAI_Melee_IdleUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  
}

void EnemyAI_Melee_IdleExit(GameObject* enemy, EnemyContainer* enemyContainer)
{

}

/****************PATROL******************/

void EnemyAI_Melee_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
}

void EnemyAI_Melee_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (distanceToPlayer <= enemyContainer->detectRange)
  {
    enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
  }
}

void EnemyAI_Melee_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_IDLE;
}

/****************CHASE******************/

void EnemyAI_Melee_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_WALK;
}

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

void EnemyAI_Melee_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_WALK;

}

/****************ATTACK******************/

void EnemyAI_Melee_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_ATTACK;
  enemy->sprite->frame = 0;
}

void EnemyAI_Melee_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  //if (distanceToPlayer > enemyContainer->attackRange)
  //{
  //  enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
  //  return;
  //}

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

    enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;

    EnemyMeleeAttack(enemy, attackDirection);
    enemy->enemyAI->newEnemyState = ENEMY_STATE_COOLDOWN;
  }
}

void EnemyAI_Melee_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_ATTACK;
  enemyContainer->attackWindup = enemyContainer->attackWindupLength;
}

//******************* COOL DOWN *********************//s

void EnemyAI_Melee_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
  enemy->sprite->frame = 0;
}

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

void EnemyAI_Melee_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~(ENEMY_IDLE);
}