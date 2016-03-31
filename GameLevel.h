/*!
Project (working title): Epoch
\file   GameLevel.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains functions for 'in-dungeon' game levels.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "GameObject.h"
void GameLevelRun();
void GameLevelInit();
void InputHandle();
void OnTouchDoor();
void OnPlayerKilled();
GameObject* GetPlayerObject(void);