#pragma once
#include "EnemyStateMachine.h"

void EnemyAI_MeleeCharger_IdleStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_IdleUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_IdleExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_MeleeCharger_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_MeleeCharger_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_ChaseUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_MeleeCharger_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeCharger_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer);