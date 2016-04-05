#pragma once
#include "GameObject.h"
#include "Vector2D.h"

GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density, float emitDelay);
void SetParticleAnim(Animation* input); //temporary debug for now;