/*!
Project (working title): Epoch
\file   EnemyStateMachine.h
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  Header file for EnemyStateMachine.c
*/

#pragma once
#include "GameObject.h"
#include "Enemy.h"

void ESMachineRun(GameObject* enemy);

void ESMachineStart(GameObject* enemy, EnemyContainer* enemyContainer);

void ESMachineUpdate(GameObject* enemy, EnemyContainer* enemyContainer);

void ESMachineExit(GameObject* enemy, EnemyContainer* enemyContainer);

void ESMachineStateChange(GameObject* enemy, EnemyContainer* enemyContainer);

void ESMachineMeleeStateChange(GameObject* enemy);

//void ESMachineRangedStateChange(GameObject* enemy);

void ESMachineMeleeBigStateChange(GameObject* enemy);

void ESMachineMeleeChargerStateChange(GameObject* enemy);