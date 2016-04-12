/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  Compass.h
Created       :  3/28/16
Description   :  Contains the functionality of the compass on the screen.
ChangeLog

4/1/16        :  Added oscillating behavior to compass

4/12/16       :  Added comments to code

© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
**************************************************************************************************/

#ifndef COMPASS_H
#define COMPASS_H

#include "AEEngine.h"
#include "Vector2D.h"
#include "Graphics.h"

/**************************************************************************************************
Function      : Compass_Load
Description   : Loads the compass into memory. Call in during level initialization.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Compass_Load(void);

/**************************************************************************************************
Function      : Compass_Init
Description   : Sets the compass for use. Call in during level initialization.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Compass_Init(void);

/**************************************************************************************************
Function      : Compass_Free
Description   : Frees the compass from use. Call when freeing level.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Compass_Free(void);

/**************************************************************************************************
Function      : Compass_Unload
Description   : Unloads compass from memory. Call when unloading level.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Compass_Unload(void);

/**************************************************************************************************
Function      : Compass_IsLoaded
Description   : Returns loaded state of compass.
Input         : No input.
Output        : (int) Returns 1 if compass is loaded into memory, 0 if not.
**************************************************************************************************/
int Compass_IsLoaded(void);

/**************************************************************************************************
Function      : Compass_IsActive
Description   : Returns active state of compass.
Input         : No input.
Output        : (int) Returns 1 if compass is active, 0 if not.
**************************************************************************************************/
int Compass_IsActive(void);

/**************************************************************************************************
Function      : Compass_Update
Description   : Updates transform of compass. Call during game update.
Input         : playerPos is position of player,
goal is position of goal.
Output        : No output.
**************************************************************************************************/
void Compass_Update(Vector2D *playerPos, Vector2D *goal);

/**************************************************************************************************
Function      : Compass_Draw
Description   : Draws the compass object. Call during game render.
Input         : No input
Output        : No output.
**************************************************************************************************/
void Compass_Draw(void);

#endif // !COMPASS_H
