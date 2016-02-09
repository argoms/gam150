/*!
\file   Entity.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains functionality for entities- objects that can take/receive damage.
*/
#pragma once
#include "GameObject.h"
void EnemySimulate();
void EnemyOnCollision(GameObject* _thisObject, GameObject* _otherObject);