/*!
Project (working title): Epoch
\file   EnemyAIMeleeChargerBehavior.h
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
*/
#pragma once
#include "EnemyStateMachine.h"

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