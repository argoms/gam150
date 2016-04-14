/*!
Project (working title): Epoch
\file   EnemyAIMeleeBehavior.h
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
*/
#pragma once
#include "EnemyStateMachine.h"

void EnemyAI_Melee_PatrolStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_PatrolUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_PatrolExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_Melee_ChaseStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_ChaseUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_ChaseExit(GameObject* enemy, EnemyContainer* enemyContainer);


void EnemyAI_Melee_AttackStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_AttackUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_AttackExit(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_CooldownStart(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_CooldownUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void EnemyAI_Melee_CooldownExit(GameObject* enemy, EnemyContainer* enemyContainer);