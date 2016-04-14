/*!
Project (working title): Epoch
\file   PlayerDeathTimer.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Headers for the "you died" text and switches levels afer a set amount of time.
*/
#pragma once
void DeathTimerStart(Vector2D position);
void DeathTimerSimulate(GameObject* inst);