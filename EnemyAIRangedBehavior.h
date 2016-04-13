#pragma once
#include "EnemyStateMachine.h"

void EnemyAI_Ranged_IdleStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_IdleUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_IdleExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_Ranged_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_Ranged_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_ChaseUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_Ranged_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Ranged_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer);