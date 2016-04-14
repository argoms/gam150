/*!
Project (working title): Epoch
\file   EnemyHealthIndicator.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functionality for displaying hearts representing enemy health above enemies.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "GameObject.h"
#include "GameLevel.h"
#include "Entity.h"
#include "Text.h"
#include "Isometric.h"

void AddEnemyHealthBar(GameObject* inst, Vector2D offset);
void HealthObjectSimulate(GameObject* inst);