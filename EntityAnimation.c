/*!
Project (working title): Epoch
\file   EntityAnimation.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright � 2016 DigiPen (USA) Corporation.
\brief
Wrapper around graphics animations objects to allow for easier animation setting for game objects that face multiple directions (e.g. player).
Stores groups of animation objects and outputs the appropriate one given a direction vector.
*/
#include "EntityAnimation.h"
#include <stdlib.h>

//private list struct for storage of all active animations (and memory freeing)
typedef struct AnimationList AnimationList;
struct AnimationList
{
  AnimationSet* first;
  AnimationSet* last;
};

AnimationList animationList;

void EntityAnimationInit()
{
  animationList.first = NULL;
  animationList.last = NULL;
}

/*!
\brief Generates a new animation set based on a given bunch of animations
\param inputAnims an array of points to 16 animations.
\return Returns a pointer to the new animation set.
*/
AnimationSet* CreateAnimationSet(Animation* inputAnims[])
{
  EntityAnimationInit();
  AnimationSet* newAnimSet = (AnimationSet*)malloc(sizeof(AnimationSet));
  int i = -1;
  while (i++ < 16)
  {
    newAnimSet->animations[i] = inputAnims[i];
  }

  animationList.last = newAnimSet;
  if (animationList.first == NULL)
  {
    animationList.first = newAnimSet;
  }
  return newAnimSet;
}

/*!
\brief Calculates the appropriate animation index according to a given vector to get the correct facing direction

\param input Direction vector
\return Returns index of animationset to use
*/
int AnimationSetIndex(Vector2D* input)
{
  
  //get input dir
  float directionAngle = atan2f(input->y, input->x);

  directionAngle *= -1; //flip around since apparently my render script actually goes clockwise instead of ccw
  int index = 0;
  directionAngle *= 57.29578f;//shitty radians to degrees conversion
  directionAngle += 180; //add 180 degrees since atan2 gives from -180 to 180 intead of 0 to 360

  //printf("ANGLE: %f\n", directionAngle);
  while (directionAngle - index * 22.5f > -37.5f)//22.5f) //insert magic numbers until everything is better
  {
    index++;
  }

  return index < 16 ? index : index - 16; //want to return 0-15, so clamp value
}

/*!
\brief Calculates the animation for a sprite to play given its direction and the animation set to use.
\return Returns a pointer to the correct animation.
*/
Animation* AnimationPlay(AnimationSet* set, Vector2D* dir)
{
  //return set->animations[0];
  return set->animations[AnimationSetIndex(dir)];
}