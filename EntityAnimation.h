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