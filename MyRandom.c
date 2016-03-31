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

/*!
\brief Written by James. Float between 0 and 1 in increments of 0.01.
*/
float RandFloat()
{
  float a = (float)(rand() % 100);
  a /= 100;
  return a;
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
  return ((rand() % (max - min)) + min);
  
}

/**************************************************************************************************
Function      : RandSeed
Description   : Sets the seed for the random number generator.
Input         : seed is the number to use as the seed.
Output        : No return.
**************************************************************************************************/
void RandSeed(unsigned int seed)
{
  srand(seed);
}