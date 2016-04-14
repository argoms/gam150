#pragma once
#include "EnemyStateMachine.h"

void EnemyAI_MeleeBig_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_MeleeBig_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_ChaseUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_MeleeBig_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_MeleeBig_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer);