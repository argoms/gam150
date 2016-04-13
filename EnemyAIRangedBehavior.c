#include "EnemyAIRangedBehavior.h"
#include "EnemyAnimations.h"
#include "Isometric.h"
#include "AEEngine.h"

/****************IDLE******************/

void EnemyAI_Ranged_IdleStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
}

void EnemyAI_Ranged_IdleUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{

}

void EnemyAI_Ranged_IdleExit(GameObject* enemy, EnemyContainer* enemyContainer)
{

}

/****************PATROL******************/

void EnemyAI_Ranged_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
}

void EnemyAI_Ranged_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (distanceToPlayer <= enemyContainer->detectRange)
  {
    enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
  }
}

void EnemyAI_Ranged_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_IDLE;
}

/****************CHASE******************/

void EnemyAI_Ranged_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_WALK;
}

void EnemyAI_Ranged_ChaseUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  Vector2D facingDirection;
  facingDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
  facingDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;

  Vector2DNormalize(&facingDirection, &facingDirection);
  Vector2DScale(&(enemy->physics->velocity), &facingDirection, enemyContainer->chaseSpeed);

  Vector2D worldFacing;
  worldFacing = IsoWorldToScreen(&facingDirection);
  //EnemyChangeAnimationFlag(enemyContainer, &worldFacing);

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

void EnemyAI_Ranged_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_WALK;
}

/****************ATTACK******************/

void EnemyAI_Ranged_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_ATTACK;
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
}

void EnemyAI_Ranged_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (distanceToPlayer > enemyContainer->attackRange)
  {
    enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
    return;
  }

  if (enemyContainer->attackWindup > 0)
  {
    enemyContainer->attackWindup -= AEFrameRateControllerGetFrameTime();
  }

  Vector2D attackDirection;
  attackDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
  attackDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;
  Vector2DNormalize(&attackDirection, &attackDirection);

  Vector2D worldFacing;
  worldFacing = IsoWorldToScreen(&attackDirection);

  //EnemyChangeAnimationFlag(enemyContainer, &worldFacing);

  if (enemyContainer->attackWindup <= 0)
  {
    enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
    EnemyMeleeAttack(enemy, attackDirection);
    enemy->enemyAI->newEnemyState = ENEMY_STATE_COOLDOWN;
  }
}

void EnemyAI_Ranged_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_ATTACK;
  enemyContainer->attackWindup = enemyContainer->attackWindupLength;
}

/*****************COOLDOWN***************/

void EnemyAI_Ranged_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
}

void EnemyAI_Ranged_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  Vector2D attackDirection;
  attackDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
  attackDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;
  Vector2DNormalize(&attackDirection, &attackDirection);

  Vector2D worldFacing;
  worldFacing = IsoWorldToScreen(&attackDirection);

  //EnemyChangeAnimationFlag(enemyContainer, &worldFacing);
  if (enemyContainer->attackCooldown > 0)
  {
    enemyContainer->attackCooldown -= AEFrameRateControllerGetFrameTime();
  }

  if (enemyContainer->attackCooldown <= 0)
  {
    enemy->enemyAI->newEnemyState = ENEMY_STATE_ATTACK;
  }
}

void EnemyAI_Ranged_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~(ENEMY_IDLE);
}