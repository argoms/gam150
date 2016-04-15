/*!
Project (working title): Epoch
\file   EnemyStateMachine.c
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  State machine for Enemy AI
*/
#include "EnemyStateMachine.h"
#include "EnemyAIMeleeBehavior.h"
#include "EnemyAIRangedBehavior.h"
#include "EnemyAIMeleeBigBehavior.h"
#include "EnemyAIMeleeChargerBehavior.h"

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
    //case ENEMY_TYPE_RANGED:
    //  ESMachineRangedStateChange(enemy);
    //  break;
    case ENEMY_TYPE_MELEE_BIG:
      ESMachineMeleeBigStateChange(enemy);
      break;
    case ENEMY_TYPE_MELEE_CHARGE:
      ESMachineMeleeChargerStateChange(enemy);
      break;
  }
}

void ESMachineMeleeStateChange(GameObject* enemy)
{
  switch (enemy->enemyAI->currentEnemyState)
  {
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

//void ESMachineRangedStateChange(GameObject* enemy)
//{
//  switch (enemy->enemyAI->currentEnemyState)
//  {
//  case ENEMY_STATE_PATROL:
//    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_PatrolStart;
//    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_PatrolUpdate;
//    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_PatrolExit;
//    break;
//  case ENEMY_STATE_CHASE:
//    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_ChaseStart;
//    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_ChaseUpdate;
//    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_ChaseExit;
//    break;
//  case ENEMY_STATE_ATTACK:
//    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_AttackStart;
//    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_AttackUpdate;
//    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_AttackExit;
//    break;
//  case ENEMY_STATE_COOLDOWN:
//    enemy->enemyAI->EnemyStateStart = EnemyAI_Ranged_CooldownStart;
//    enemy->enemyAI->EnemyStateUpdate = EnemyAI_Ranged_CooldownUpdate;
//    enemy->enemyAI->EnemyStateExit = EnemyAI_Ranged_CooldownExit;
//    break;
//  }
//}

void ESMachineMeleeBigStateChange(GameObject* enemy)
{
  switch (enemy->enemyAI->currentEnemyState)
  {
  case ENEMY_STATE_PATROL:
    enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeBig_PatrolStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeBig_PatrolUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeBig_PatrolExit;
    break;
  case ENEMY_STATE_CHASE:
    enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeBig_ChaseStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeBig_ChaseUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeBig_ChaseExit;
    break;
  case ENEMY_STATE_ATTACK:
    enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeBig_AttackStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeBig_AttackUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeBig_AttackExit;
    break;
  case ENEMY_STATE_COOLDOWN:
    enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeBig_CooldownStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeBig_CooldownUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeBig_CooldownExit;
    break;
  }
}

void ESMachineMeleeChargerStateChange(GameObject* enemy)
{
  switch (enemy->enemyAI->currentEnemyState)
  {
  case ENEMY_STATE_PATROL:
    enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeCharger_PatrolStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeCharger_PatrolUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeCharger_PatrolExit;
    break;
  case ENEMY_STATE_CHASE:
    enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeCharger_ChaseStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeCharger_ChaseUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeCharger_ChaseExit;
    break;
  case ENEMY_STATE_ATTACK:
    enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeCharger_AttackStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeCharger_AttackUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeCharger_AttackExit;
    break;
  case ENEMY_STATE_COOLDOWN:
    enemy->enemyAI->EnemyStateStart = EnemyAI_MeleeCharger_CooldownStart;
    enemy->enemyAI->EnemyStateUpdate = EnemyAI_MeleeCharger_CooldownUpdate;
    enemy->enemyAI->EnemyStateExit = EnemyAI_MeleeCharger_CooldownExit;
    break;
  }
}