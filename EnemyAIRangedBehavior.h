#pragma once
#include "EnemyStateMachine.h"

EnemyAI_Ranged_IdleStart(GameObject* enemy);

EnemyAI_Ranged_IdleUpdate(GameObject* enemy);

EnemyAI_Ranged_IdleExit(GameObject* enemy);


EnemyAI_Ranged_PatrolStart(GameObject* enemy);

EnemyAI_Ranged_PatrolUpdate(GameObject* enemy);

EnemyAI_Ranged_PatrolExit(GameObject* enemy);


EnemyAI_Ranged_ChaseStart(GameObject* enemy);

EnemyAI_Ranged_ChaseUpdate(GameObject* enemy);

EnemyAI_Ranged_ChaseExit(GameObject* enemy);


EnemyAI_Ranged_AttackStart(GameObject* enemy);

EnemyAI_Ranged_AttackUpdate(GameObject* enemy);

EnemyAI_Ranged_AttackExit(GameObject* enemy);