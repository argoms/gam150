/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  Compass.c
Created       :  3/28/16
Description   :  Contains the functionality of the compass on the screen.
ChangeLog

4/1/16        :  Added oscillating behavior to compass

4/12/16       :  Added comments to code

© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
**************************************************************************************************/
#include "AEEngine.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Compass.h"
#include "MyRandom.h"
#include "Matrix2D.h"
#include "Physics.h"
#include <math.h>

/*-------------------------------------------------------------------------------------------------
DEFINES AND STRUCTS
-------------------------------------------------------------------------------------------------*/

#define COMPASS_DISTANCE 300
#define COMPASS_SIZE 70
#define COMPASS_SPEED 3

//#define PI 3.14159265359

#define true 1
#define false 0


typedef struct Compass Compass;

/* Contains compass information */
struct Compass
{
  int loaded;               /* If the compass is loaded into memory. */
  int active;               /* If the compass is active. */
  AEGfxTexture *texture;    /* The texture used by the compass. */
  AEGfxVertexList *mesh;    /* The mesh used by the compass. */
  //Vector2D position;        /* The position of the compass. */
  float angle;              /* The angle of the compass. */
  float distance;           /* The distance from the compass to the player. */
};
/*-------------------------------------------------------------------------------------------------
END DEFINES AND STRUCTS
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
STATIC VARIABLES
-------------------------------------------------------------------------------------------------*/

static Compass compass = { 0 };
static double time;
/*-------------------------------------------------------------------------------------------------
END STATIC VARIABLES
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
FUNCTIONS
-------------------------------------------------------------------------------------------------*/

/**************************************************************************************************
Function      : Compass_Load
Description   : Loads the compass into memory. Call during level initialization.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Compass_Load(void)
{
  if (compass.loaded)
    return;

  /* Create the mesh for the compass. */
  AEGfxMeshStart();

  AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0, 1,
    -0.5f, 0.5f, 0xFFFFFFFF, 0, 0,
    0.5f, -0.5f, 0xFFFFFFFF, 1, 1);
  AEGfxTriAdd(0.5f, 0.5f, 0xFFFFFFFF, 1, 0,
    -0.5f, 0.5f, 0xFFFFFFFF, 0, 0,
    0.5f, -0.5f, 0xFFFFFFFF, 1, 1);

  compass.mesh = AEGfxMeshEnd();

  /* Load the texture for the compass. */
  compass.texture = AEGfxTextureLoad("animations/compass/compass2.png");

  compass.loaded = 1;
}

/**************************************************************************************************
Function      : Compass_Init
Description   : Sets the compass for use. Call in during level initialization.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Compass_Init(void)
{
  compass.active = 1;
  time = 0;
}

/**************************************************************************************************
Function      : Compass_Free
Description   : Frees the compass from use. Call when freeing level.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Compass_Free(void)
{
  compass.active = 0;
}

/**************************************************************************************************
Function      : Compass_Unload
Description   : Unloads compass from memory. Call when unloading level.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void Compass_Unload(void)
{
  if (compass.loaded)
  {
    AEGfxMeshFree(compass.mesh);
    AEGfxTextureUnload(compass.texture);
    compass.loaded = 0;
  }
}

/**************************************************************************************************
Function      : Compass_IsLoaded
Description   : Returns loaded state of compass.
Input         : No input.
Output        : (int) Returns 1 if compass is loaded into memory, 0 if not.
**************************************************************************************************/
int Compass_IsLoaded(void)
{
  return compass.loaded;
}

/**************************************************************************************************
Function      : Compass_IsActive
Description   : Returns active state of compass.
Input         : No input.
Output        : (int) Returns 1 if compass is active, 0 if not.
**************************************************************************************************/
int Compass_IsActive(void)
{
  return compass.active;
}

/**************************************************************************************************
Function      : Compass_Update
Description   : Updates transform of compass. Call during game update.
Input         : playerPos is position of player,
                goal is position of goal.
Output        : No output.
**************************************************************************************************/
void Compass_Update(Vector2D *playerPos, Vector2D *goal)
{
  /* Create local copies of the position vectors so we don't mess them up. */
  Vector2D pPosCopy = *playerPos;
  Vector2D goalCopy = *goal;

  /* Calculate the angle between the player and the goal. */
  Vector2D direction;
  Vector2DSub(&direction, &goalCopy, &pPosCopy);

  /* Update the angle. */
  compass.angle = atan2f(direction.y, direction.x) - PI / 2.f + PI / 4.f;

  /* Update distance from player to goal. */
  compass.distance = Vector2DLength(&direction);
  if (compass.distance > COMPASS_DISTANCE)
    compass.distance = COMPASS_DISTANCE;

  /* Update time */
  time += AEFrameRateControllerGetFrameTime() * COMPASS_SPEED;
  if (time >= 2 * PI)
    time = 0;
}

/**************************************************************************************************
Function      : Compass_Draw
Description   : Draws the compass object. Call during game render.
Input         : No input
Output        : No output.
**************************************************************************************************/
void Compass_Draw(void)
{
  if (!compass.loaded)
    return;

  float CameraX;
  float CameraY;
  AEGfxGetCamPosition(&CameraX, &CameraY);

  Matrix2D transform, scale, rot, trans, trans_local, iso_scale;

  Matrix2DIdentity(&scale);
  Matrix2DScale(&scale, COMPASS_SIZE + (COMPASS_SIZE * 0.1f * ((float)sin(time))), COMPASS_SIZE + (COMPASS_SIZE * 0.1f * ((float)sin(time))));

  Matrix2DIdentity(&rot);
  Matrix2DRotRad(&rot, compass.angle);

  Vector2D direction;
  Vector2DFromAngleRad(&direction, compass.angle);

  Matrix2DIdentity(&trans_local);
  Matrix2DTranslate(&trans_local, 0, COMPASS_DISTANCE + ( COMPASS_DISTANCE * 0.1f * ((float)sin(time))));//compass.distance);

  Matrix2DIdentity(&trans);
  Matrix2DTranslate(&trans, CameraX, CameraY);
 
  Matrix2DIdentity(&iso_scale);
  Matrix2DScale(&iso_scale, 1, 0.5f);

  /* Combine matrices. */

  Matrix2DConcat(&transform, &trans_local, &scale);

  Matrix2DConcat(&transform, &rot, &transform);

  Matrix2DConcat(&transform, &iso_scale, &transform);

  Matrix2DConcat(&transform, &trans, &transform);

  AEGfxSetTransform(transform.m);

  /* Preparing for draw */

  AEGfxTextureSet(compass.texture, 0, 0);

  AEGfxSetTintColor(1, 1, 1, 1);

  AEGfxSetTransparency(1.0f);

  AEGfxSetBlendMode(AE_GFX_BM_BLEND);

  /* Draw. */

  AEGfxMeshDraw(compass.mesh, AE_GFX_MDM_TRIANGLES);
}
/*-------------------------------------------------------------------------------------------------
END FUNCTIONS
-------------------------------------------------------------------------------------------------*/