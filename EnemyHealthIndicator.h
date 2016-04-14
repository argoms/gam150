/*!
Project (working title): Epoch
\file   EnemyHealthIndicator.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Functionality for displaying hearts representing enemy health above enemies.
*/
#include "GameObject.h"
#include "GameLevel.h"
#include "Entity.h"
#include "Text.h"
#include "Isometric.h"

void AddEnemyHealthBar(GameObject* inst, Vector2D offset);
void HealthObjectSimulate(GameObject* inst);