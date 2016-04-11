/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  Compass.h
Created       :  3/28/16
Description   :  Contains the functionality of the compass on the screen.
ChangeLog

4/1/16        :  Added oscillating behavior to compass

© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
**************************************************************************************************/

#ifndef COMPASS_H
#define COMPASS_H

#include "AEEngine.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Graphics.h"

typedef struct Compass Compass;

struct Compass
{
  int loaded;
  int active;
  AEGfxTexture *texture;
  AEGfxVertexList *mesh;
  Vector2D position;
  float angle;
  float distance;
};

/* Loads the compass into memory. */
void Compass_Load();

/* Updates transform of compass. */
void Compass_Update(Vector2D *playerPos, Vector2D *goal);

/* Sets the compass for use. */
void Compass_Init();

/* Frees the compass from use. */
void Compass_Free();

/* Unloads compass assets from memory. */
void Compass_Unload();

int Compass_IsLoaded();

/* Returns active state of compass */
int Compass_IsActive();

/* Draws the compass object. */
void Compass_Draw();

#endif // !COMPASS_H
