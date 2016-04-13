/*!
Project (working title): Epoch
\file   ScreenShake.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Simple implementation of screenshake

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
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