/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  MyRandom.h
Created       :  3/1/16
Description   :  Provides functions for creating random values
ChangeLog
**************************************************************************************************/
#ifndef MY_RANDOM_H
#define MY_RANDOM_H

/**************************************************************************************************
Function      : RandFloatRange
Description   : Returns a random floating point number between two values.
Input         : min is the minimum value,
max is the maximum value.
Output        : Returns a float between min and max.
**************************************************************************************************/
float RandFloatRange(float min, float max);

/**************************************************************************************************
Function      : RandIntRange
Description   : Returns a random integer between two values.
Input         : min is the minimum value,
max is the maximum value.
Output        : Returns an int between min and max.
**************************************************************************************************/
int RandIntRange(int min, int max);

/**************************************************************************************************
Function      : RandSeed
Description   : Sets the seed for the random number generator.
Input         : seed is the number to use as the seed.
Output        : No return.
**************************************************************************************************/
void RandSeed(unsigned int seed);

#endif // !MY_RANDOM_H

