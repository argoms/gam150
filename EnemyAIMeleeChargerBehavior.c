#include "EnemyAIMeleeChargerBehavior.h"
#include "EnemyAnimations.h"
#include "Isometric.h"
#include "AEEngine.h"

/****************IDLE******************/

void EnemyAI_MeleeCharger_IdleStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
}

void EnemyAI_MeleeCharger_IdleUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{

}

void EnemyAI_MeleeCharger_IdleExit(GameObject* enemy, EnemyContainer* enemyContainer)
{

}

/****************PATROL******************/

void EnemyAI_MeleeCharger_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
}

void EnemyAI_MeleeCharger_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (distanceToPlayer <= enemyContainer->detectRange)
  {
    //printf("DETECTEDPLAYER\n");
    enemy->enemyAI->newEnemyState = ENEMY_STATE_CHASE;
  }
}

void EnemyAI_MeleeCharger_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_IDLE;
}

/****************CHASE******************/

void EnemyAI_MeleeCharger_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_WALK;
}

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

void EnemyAI_MeleeCharger_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_WALK;
}

/****************ATTACK******************/

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

void EnemyAI_MeleeCharger_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  float distanceToPlayer = Vector2DSquareDistance(&(enemy->physics->position), &(enemy->target->physics->position));

  if (enemyContainer->attackWindup > 0)
  {
    enemyContainer->attackWindup -= AEFrameRateControllerGetFrameTime();
  }

  if (enemyContainer->attackWindup <= .62f && enemyContainer->attackWindup > 0.6f)
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
    Vector2D attackDirection;
    attackDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
    attackDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;
    Vector2DNormalize(&attackDirection, &attackDirection);

    enemy->physics->velocity.x = 0;
    enemy->physics->velocity.y = 0;
    enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;

    EnemyMeleeAttack(enemy, attackDirection);
    enemy->enemyAI->newEnemyState = ENEMY_STATE_COOLDOWN;
  }
}

void EnemyAI_MeleeCharger_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~ENEMY_ATTACK;
  enemyContainer->attackWindup = enemyContainer->attackWindupLength;
  enemy->sprite->frame = 0;
}

//******************* COOL DOWN *********************//

void EnemyAI_MeleeCharger_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->enemyAnimationState = ENEMY_IDLE;
}

void EnemyAI_MeleeCharger_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemy->physics->velocity.x = 0;
  enemy->physics->velocity.y = 0;
  if (enemyContainer->attackCooldown > 0)
  {
    enemyContainer->attackCooldown -= AEFrameRateControllerGetFrameTime();
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

void EnemyAI_MeleeCharger_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  enemyContainer->attackCooldown = enemyContainer->attackCooldownLength;
  enemyContainer->enemyAnimationState = enemyContainer->enemyAnimationState & ~(ENEMY_IDLE);
}