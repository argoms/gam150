#include "EnemyStateMachine.h"
#include "EnemyAIMeleeBehavior.h"
#include "EnemyAIRangedBehavior.h"

/*
/brief
  Use this to run the ESM machine. CALL IT ONCE.
/param enemy
  Obvious
*/
void ESMachineRun(GameObject* enemy)
{
  EnemyContainer* enemyContainer = enemy->miscData;
  if (enemy->enemyAI->currentEnemyState != enemy->enemyAI->newEnemyState)
  {
    ESMachineExit(enemy, enemyContainer);
    enemy->enemyAI->currentEnemyState = enemy->enemyAI->newEnemyState;
    ESMachineStateChange(enemy, enemyContainer);
    ESMachineStart(enemy, enemyContainer);  
  }

  ESMachineUpdate(enemy, enemyContainer);
}

void ESMachineStart(GameObject* enemy, EnemyContainer* enemyContainer)
{
  (enemy->enemyAI->EnemyStateStart)(enemy, enemyContainer);
}

void ESMachineUpdate(GameObject* enemy, EnemyContainer* enemyContainer)
{
  (enemy->enemyAI->EnemyStateUpdate)(enemy, enemyContainer);
}

void ESMachineExit(GameObject* enemy, EnemyContainer* enemyContainer)
{
  (enemy->enemyAI->EnemyStateExit)(enemy, enemyContainer);
}

void ESMachineStateChange(GameObject* enemy, EnemyContainer* enemyContainer)
{
  switch (enemyContainer->enemyType)
  {
    case ENEMY_TYPE_MELEE:
      ESMachineMeleeStateChange(enemy);
      break;
    case ENEMY_TYPE_RANGED:
      ESMachineRangedStateChange(enemy);
      break;
  }
}

void ESMachineMeleeStateChange(GameObject* enemy)
{
  switch (enemy->enemyAI->currentEnemyState)
  {
  case ENEMY_STATE_IDLE:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Melee_IdleStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Melee_IdleUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Melee_IdleExit;
    break;
  case ENEMY_STATE_PATROL:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Melee_PatrolStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Melee_PatrolUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Melee_PatrolExit;
    break;
  case ENEMY_STATE_CHASE:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Melee_ChaseStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Melee_ChaseUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Melee_ChaseExit;
    break;
  case ENEMY_STATE_ATTACK:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Melee_AttackStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Melee_AttackUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Melee_AttackExit;
    break;
  case ENEMY_STATE_COOLDOWN:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Melee_CooldownStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Melee_CooldownUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Melee_CooldownExit;
    break;
  }
}

void ESMachineRangedStateChange(GameObject* enemy)
{
  switch (enemy->enemyAI->currentEnemyState)
  {
  case ENEMY_STATE_IDLE:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_IdleStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_IdleUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_IdleExit;
    break;
  case ENEMY_STATE_PATROL:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_PatrolStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_PatrolUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_PatrolExit;
    break;
  case ENEMY_STATE_CHASE:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_ChaseStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_ChaseUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_ChaseExit;
    break;
  case ENEMY_STATE_ATTACK:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_AttackStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_AttackUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_AttackExit;
    break;
  case ENEMY_STATE_COOLDOWN:
    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_CooldownStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_CooldownUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_CooldownExit;
    break;
  }
}