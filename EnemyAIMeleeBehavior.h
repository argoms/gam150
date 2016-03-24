#pragma once
#include "EnemyStateMachine.h"

EnemyAI_Melee_IdleStart(GameObject* enemy);

EnemyAI_Melee_IdleUpdate(GameObject* enemy);

EnemyAI_Melee_IdleExit(GameObject* enemy);


EnemyAI_Melee_PatrolStart(GameObject* enemy);

EnemyAI_Melee_PatrolUpdate(GameObject* enemy);

EnemyAI_Melee_PatrolExit(GameObject* enemy);


EnemyAI_Melee_ChaseStart(GameObject* enemy);

EnemyAI_Melee_ChaseUpdate(GameObject* enemy);

EnemyAI_Melee_ChaseExit(GameObject* enemy);


EnemyAI_Melee_AttackStart(GameObject* enemy);

EnemyAI_Melee_AttackUpdate(GameObject* enemy);

EnemyAI_Melee_AttackExit(GameObject* enemy);