/*!
Project (working title): Epoch
\file   ScreenShake.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright � 2016 DigiPen (USA) Corporation.
\brief
Simple implementation of screenshake based on sine waves.

*/
#include "ScreenShake.h"
static float timeElapsed = 0;
static float effectDuration = 0;
static Vector2D screenOffset;
static float effectStrengthModifier;

/*!
\brief Called internally by player's HUD to update screen shake. Don't worry too much about this.
*/
void ScreenShakeSimulate()
{
  screenOffset.x = 0;
  timeElapsed += (float)AEFrameRateControllerGetFrameTime();

  //if effect is currently active (duration > 0)
  if (effectDuration > 0)
  {
    //shake is based on a sine wave with progressively decreasing magnitude
    screenOffset.y = sinf(timeElapsed * effectStrengthModifier) * effectDuration * effectStrengthModifier;
    effectDuration -= (float)AEFrameRateControllerGetFrameTime();
  }
  else
  {
    screenOffset.y = 0;
  }
}

/*!
\brief Adds screen shake.
\param duration Amount of time to shake for.
\param magnitude Multiplier to strength of shake effect.
*/
void AddScreenShake(float duration, float magnitude)
{
  effectDuration = duration;
  effectStrengthModifier = magnitude;
}
Vector2D GetScreenShakeOffset()
{
  return screenOffset;
}