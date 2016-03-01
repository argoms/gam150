/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  MyRandom.c
Created       :  3/1/16
Description   :  Provides functions for creating random values
ChangeLog
**************************************************************************************************/

#include "AEEngine.h"

/**************************************************************************************************
Function      : RandFloatRange
Description   : Returns a random floating point number between two values.
Input         : min is the minimum value,
                max is the maximum value.
Output        : Returns a float between min and max.
**************************************************************************************************/
float RandFloatRange(float min, float max)
{
  return ((AERandFloat() * (max - min)) + min);
}

/**************************************************************************************************
Function      : RandIntRange
Description   : Returns a random integer between two values.
Input         : min is the minimum value,
                max is the maximum value.
Output        : Returns an int between min and max.
**************************************************************************************************/
int RandIntRange(int min, int max)
{
  return (int)RandFloatRange((float)min, (float)max);
}