/*!
Project (working title): Epoch
\file   EnemyAIRangedBehavior.h
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
*/
#pragma once
#include "EnemyStateMachine.h"

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