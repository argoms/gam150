/*!
Project (working title): Epoch
\file   EntityAnimation.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Interface for EntityAnimation.
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