/*!
Project (working title): Epoch
\file   ScreenShake.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Simple implementation of screenshake
*/
#pragma once
#include "Graphics.h"
#include "PlayerEntity.h"
#include "GameLevel.h"
#include <math.h>
#include "AEEngine.h"

void ScreenShakeSimulate();
void AddScreenShake(float duration, float magnitude);
Vector2D GetScreenShakeOffset();