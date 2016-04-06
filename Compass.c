/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  Compass.c
Created       :  3/28/16
Description   :  Contains the functionality of the compass on the screen.
ChangeLog



**************************************************************************************************/

#include "AEEngine.h"
#include "Vector2D.h"
#include "Graphics.h"
#include "Compass.h"
#include "MyRandom.h"
#include "Matrix2D.h"
#include "Isometric.h"
#include <math.h>

#define COMPASS_DISTANCE 300
#define COMPASS_SIZE 70
#define COMPASS_SPEED 3

//#define PI 3.14159265359

#define true 1
#define false 0

static Compass compass = { 0 };
static double time;

/* Loads the compass into memory. */
void Compass_Load()
{
  if (compass.loaded)
    return;

  /* Draw the mesh for the compass. */
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

/* Sets the compass for use. */
void Compass_Init()
{
  compass.active = 1;
  time = 0;
}

/* Frees the compass from use. */
void Compass_Free()
{
  compass.active = 0;
}

void Compass_Unload()
{
  if (compass.loaded)
  {
    AEGfxMeshFree(compass.mesh);
    AEGfxTextureUnload(compass.texture);
    compass.loaded = 0;
  }
}

int Compass_IsLoaded()
{
  return compass.loaded;
}

int Compass_IsActive()
{
  return compass.active;
}

/* Updates transform of compass. */
void Compass_Update(Vector2D *playerPos, Vector2D *goal)
{
  /* Create local copies of the position vectors so we don't mess them up. */
  Vector2D pPosCopy = *playerPos;
  Vector2D goalCopy = *goal;

  /* Calculate the angle between the player and the goal. */
  Vector2D direction;
  Vector2DSub(&direction, &goalCopy, &pPosCopy);
  //direction.y /= 2.f;
  //Vector2DNormalize(&direction, &direction);

  compass.angle = atan2f(direction.y, direction.x) - PI / 2.f + PI / 4.f;

  /* Camera position. */
  //float camX, camY;

  /* Update transform of compass. */
  //Vector2DScaleAdd(&compass.position, &direction, &pPosCopy, COMPASS_DISTANCE);

  /* Update distance from player to goal. */
  compass.distance = Vector2DLength(&direction);
  if (compass.distance > COMPASS_DISTANCE)
    compass.distance = COMPASS_DISTANCE;

  /* Update time */
  time += AEFrameRateControllerGetFrameTime() * COMPASS_SPEED;
  if (time >= 2 * PI)
    time = 0;

  //Doesn't work. ignore
  ///* If player is close to goal, then deactivate compass. */
  //if (compass.distance < 5)
  //  compass.active = false;
  //else
  //  compass.active = true;
}

/* Draws the compass object. */
void Compass_Draw()
{
  if (!compass.loaded)
    return;

  float CameraX;
  float CameraY;
  AEGfxGetCamPosition(&CameraX, &CameraY);

  //AEGfxSetPosition(compass.position.x + CameraX, compass.position.y + CameraY);//set draw position
  //AEGfxSetFullTransform(compass.position.x + CameraX, compass.position.y + CameraY, compass.angle, COMPASS_SIZE, COMPASS_SIZE);

  Matrix2D transform, scale, rot, trans, trans_local, iso_scale;

  //Matrix2DSet(&transform, &compass.position);


  Matrix2DIdentity(&scale);
  Matrix2DScale(&scale, COMPASS_SIZE + (COMPASS_SIZE * 0.1f * ((float)sin(time))), COMPASS_SIZE + (COMPASS_SIZE * 0.1f * ((float)sin(time))));

  Matrix2DIdentity(&rot);
  Matrix2DRotRad(&rot, compass.angle);

  //f32 targetPosX = 0.9f;
  //f32 targetPosY = 0.9f;

  //Vector2D drawPos;
  //AEGfxConvertScreenCoordinatesToWorld(targetPosX, targetPosY, &drawPos.x, &drawPos.y);

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

  //Matrix2DConcat(&transform, &trans_local, &transform);

  Matrix2DConcat(&transform, &iso_scale, &transform);

  Matrix2DConcat(&transform, &trans, &transform);

  //Matrix2DConcat(&iso_rotate, &iso_rotate, &rot);
  //Matrix2DConcat(&iso_scale, &iso_scale, &iso_rotate);

  //Matrix2DConcat(&transform, &, &trans_local);

  AEGfxSetTransform(transform.m);

  /* Preparing for draw */

  AEGfxTextureSet(compass.texture, 0, 0);

  AEGfxSetTintColor(1, 1, RandFloatRange(.5f, 2), 1);

  AEGfxSetTransparency(1.0f);

  AEGfxSetBlendMode(AE_GFX_BM_BLEND);

  AEGfxMeshDraw(compass.mesh, AE_GFX_MDM_TRIANGLES);
}