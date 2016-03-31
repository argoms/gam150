/*!
Project (working title): Epoch
\file   EntityAnimation.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Interface for EntityAnimation.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Graphics.h"
typedef struct AnimationSet AnimationSet;



struct AnimationSet
{
  Animation* animations[16];
  AnimationSet* next;
};

void EntityAnimationInit();
AnimationSet* CreateAnimationSet(Animation* inputAnims[]);
int AnimationSetIndex(Vector2D* input);
Animation* AnimationPlay(AnimationSet* set, Vector2D* dir);