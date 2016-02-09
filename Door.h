/*!
\file   Door.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains functionality for doors to move between levels on contact.
*/
#pragma once
#include "GameLevel.h"
#include "GameObject.h"
#include "LevelManager.h"
void DoorDefaultOnCollision(GameObject* _thisObject, GameObject* _otherObject);