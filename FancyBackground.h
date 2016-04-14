/**************************************************************************************************
Project       :  EPOCH
File          :  FancyBackground.h
Author        :  Matt Chasengnou
Created       :  4/5/16
Description   :  Contains the functionality for doing fancy menu backgrounds.
ChangeLog

4/6/16           Added in a base background

4/10/16          Added in different behaviour. Now has horizontal scrolling and swirly behaviour.

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
**************************************************************************************************/

#ifndef FANCY_BACKGROUND_H
#define FANCY_BACKGROUND_H

/* Determines how background behaves. */
enum BACKGROUND_BEHAVIOR
{
  BACKGROUND_BH_SWIRLY,
  BACKGROUND_BH_HORIZONTAL
};

/* Determines sprites that background uses. */
enum BACKGROUND_SPRITE_MODE
{
  BACKGROUND_MD_ENERGY,
  BACKGROUND_MD_BLOCKS
};

/* Initializes the background visuals */
void Background_Init(unsigned int behavior, unsigned int sprite_mode);

/* Unloads the background visuals from memory */
void Background_Unload(void);

/* Updates the transform information about the sprites. */
void Background_Update();

/* Draw the background. */
void Background_Draw();

#endif // !FANCY_BACKGROUND_H

