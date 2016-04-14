/*!
Project (working title): Epoch
\file   Door.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Contains definitions for doors that between levels on contact.

*/
#pragma once
#include "GameLevel.h"
#include "GameObject.h"
#include "LevelManager.h"
void DoorDefaultOnCollision(GameObject* _thisObject, GameObject* _otherObject);
GameObject* DoorCreateDoorAt(Vector2D position);
GameObject* DoorGetDoor();