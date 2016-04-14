/*!
Project (working title): Epoch
\file   GameLevel.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Contains functions for 'in-dungeon' game levels.
*/
#pragma once
#include "GameObject.h"
void GameLevelRun();
void GameLevelInit();
void InputHandle();
void OnTouchDoor();
//void OnPlayerKilled();
GameObject* GetPlayerObject(void);