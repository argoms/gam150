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
  if (enemy->enemyAI->currentEnemyState != enemy->enemyAI->newEnemyState)
  {
    ESMachineExit(enemy);
    enemy->enemyAI->currentEnemyState = enemy->enemyAI->newEnemyState;
    ESMachineStateChange(enemy);
    ESMachineStart(enemy);
  }

  ESMachineUpdate(enemy);
}

void ESMachineStart(GameObject* enemy)
{
  (enemy->enemyAI->EnemyStateStart)(enemy);
}

void ESMachineUpdate(GameObject* enemy)
{
  (enemy->enemyAI->EnemyStateUpdate)(enemy);
}

void ESMachineExit(GameObject* enemy)
{
  (enemy->enemyAI->EnemyStateExit)(enemy);
}

void ESMachineStateChange(GameObject* enemy)
{
  EnemyContainer* enemyContainer = enemy->miscData;
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
  }
}