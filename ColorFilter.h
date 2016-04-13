/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  ColorFilter.h
Created       :  4/6/16
Description   :  Contains the interface for working with ColorFilter.c.
ChangeLog

© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
**************************************************************************************************/

/*-------------------------------------------------------------------------------------------------
DEFINES
-------------------------------------------------------------------------------------------------*/

#ifndef COLOR_FILTER_H
#define COLOR_FILTER_H

/**************************************************************************************************
Function      : ColorFilter_Init
Description   : Initializes the color filter. Call during level initialization.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void ColorFilter_Init(void);

/**************************************************************************************************
Function      : ColorFilter_Unload
Description   : Unloads the filter from memory. Call during level unload.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void ColorFilter_Unload(void);

/**************************************************************************************************
Function      : ColorFilter_Set
Description   : Sets colors to filter out. Set values from 0 to 2. 1 retains original color.
Input         : r is amount of red to filter,
g is amount of green to filter,
b is amount of blue to filter.
Output        : No output.
**************************************************************************************************/
void ColorFilter_Set(float r, float g, float b);

/**************************************************************************************************
Function      : ColorFilter_Fluctuate
Description   : Sets oscillating behavior of filter.
Input         : useF is value to set flag to (use 1 to activate, 0 to deactivate),
r_amt is amount of red to oscillate (0 to 2),
g_amt is amount of green to oscillate (0 to 2),
b_amt is amount of blue to oscillate (0 to 2).
Output        : No output.
**************************************************************************************************/
void ColorFilter_Fluctuate(int useF, float r_amt, float g_amt, float b_amt);

/**************************************************************************************************
Function      : ColorFilter_Draw
Description   : Draws the filter.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void ColorFilter_Draw(void);

#endif // !COLOR_FILTER_H

