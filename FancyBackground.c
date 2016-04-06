/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  Compass.c
Created       :  4/5/16
Description   :  Contains the functionality for doing fancy menu backgrounds.
ChangeLog



**************************************************************************************************/

#include <stdlib.h>

#include "AEEngine.h"

#include "Vector2D.h"
#include "Matrix2D.h"
#include "MyRandom.h"

#define true 1
#define false 0

//#define PI 3.14159265359

#define NUM_SPRITES 110
#define NUM_TEXTURES 5

#define MAX_POS_X 100.f
#define MAX_POS_Y 100.f

#define MIN_DIST 100.f
#define MAX_DIST 400.f

#define MIN_OSC_X 0.1f
#define MAX_OSC_X 0.2f

#define MIN_OSC_Y 0.1f
#define MAX_OSC_Y 0.5f

#define MAX_SIZE_X 150.f
#define MIN_SIZE_X 10.f

#define MAX_SIZE_Y 300.f
#define MIN_SIZE_Y 10.f

typedef struct BackgroundSprite
{
  AEGfxTexture *texture;
  float posX;
  float posY;
  float scaleX;
  float scaleY;
  float distance;
  float rotation;
  float time;
  float osc_speedX;
  float osc_speedY;
}BackgroundSprite;

/* Container for our background sprites. */
static BackgroundSprite *backgroundSprites[NUM_SPRITES];

/* Container for our textures. */
static AEGfxTexture *textures[NUM_TEXTURES];

/* The base mesh to be used by all background sprites. */
static AEGfxVertexList *mesh;

/* Flag if already initialized and loaded into memory. */
static int isLoaded = false;

/* Initializes the background visuals */
void Background_Init(void)
{
  /* If already loaded, don't initialize, just return. */
  if (isLoaded)
    return;

  /* Initialize the textures. */
  textures[0] = AEGfxTextureLoad("animations/world/basicWall2.png");
  textures[1] = AEGfxTextureLoad("animations/world/basicWall3.png");
  textures[2] = AEGfxTextureLoad("animations/world/cloudTemplate.png");
  textures[3] = AEGfxTextureLoad("animations/world/pixelFloor.png");
  textures[4] = AEGfxTextureLoad("animations/world/basicFloor.png");

  /* Create the mesh. */
  AEGfxMeshStart();

  AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0, 1,
    -0.5f, 0.5f, 0xFFFFFFFF, 0, 0,
    0.5f, -0.5f, 0xFFFFFFFF, 1, 1);
  AEGfxTriAdd(0.5f, 0.5f, 0xFFFFFFFF, 1, 0,
    -0.5f, 0.5f, 0xFFFFFFFF, 0, 0,
    0.5f, -0.5f, 0xFFFFFFFF, 1, 1);

  mesh = AEGfxMeshEnd();

  /* Initialize each background sprite. */
  for (int i = 0; i < NUM_SPRITES; ++i)
  {
    /* Allocate memory for the sprite */
    BackgroundSprite *pInst = backgroundSprites[i] = (BackgroundSprite *)malloc(sizeof(BackgroundSprite));

    /* Initialize data members of the sprite. */
    pInst->posX =  RandFloatRange(-MAX_POS_X, MAX_POS_X);
    pInst->posY =  RandFloatRange(-MAX_POS_Y, MAX_POS_Y);
    pInst->distance =  RandFloatRange(MIN_DIST, MAX_DIST);
    pInst->osc_speedX = RandFloatRange(MIN_OSC_X, MAX_OSC_X);
    pInst->osc_speedY = RandFloatRange(MIN_OSC_Y, MAX_OSC_Y);
    pInst->rotation = RandFloatRange(0, 2 * PI);
    pInst->scaleX =  RandFloatRange(MIN_SIZE_X, MAX_SIZE_X);
    pInst->scaleY =  RandFloatRange(MIN_SIZE_Y, MAX_SIZE_Y);
    pInst->texture = textures[ RandIntRange(0, NUM_TEXTURES)];
    pInst->time = RandFloatRange(0, (float)(2 * PI));
  }

  /* Set the initialized flag. */
  isLoaded = true;
}

/* Unloads the background visuals from memory */
void Background_Unload(void)
{
  if (isLoaded)
  {
    int i;  /* Iterator */

    /* Free the mesh. */
    AEGfxMeshFree(mesh);

    /* Free all the textures. */
    for (i = 0; i < NUM_TEXTURES; ++i)
    {
      AEGfxTextureUnload(textures[i]);
    }

    /* Free all the background sprites. */
    for (i = 0; i < NUM_SPRITES; ++i)
    {
      free(backgroundSprites[i]);
    }

    /* Set the flag. */
    isLoaded = false;
  }
}

/* Updates the transform information about the sprites. */
void Background_Update()
{
  if (!isLoaded)
    return;

  int i;  /* Iterator. */

  /* Camera position. */
  float camX, camY;

  /* Frame time. */
  double deltaTime = AEFrameRateControllerGetFrameTime();

  /* Get camera position. */
  AEGfxGetCamPosition(&camX, &camY);

  /* Update each background sprite. */
  for (i = 0; i < NUM_SPRITES; ++i)
  {
    /* Get the sprite. */
    BackgroundSprite *pInst = backgroundSprites[i];

    /* Update the sprite information. */

    /* Scale the sprite. */
    pInst->scaleX *= RandFloatRange(0.1f, 1.7f);// *(float)deltaTime;
    pInst->scaleY *= RandFloatRange(0.1f, 1.7f);// *(float)deltaTime;

    /* Clamp the scale. */
    pInst->scaleX = AEClamp(pInst->scaleX, MIN_SIZE_X, MAX_SIZE_X);
    pInst->scaleY = AEClamp(pInst->scaleY, MIN_SIZE_Y, MAX_SIZE_Y);

    /* Rotate the sprite. */
    pInst->rotation += (float)deltaTime * pInst->osc_speedX;

    /* Update the time. */
    pInst->time += (float)deltaTime;

    /* Clamp time to be within 2 * PI. */
    if (pInst->time >= 2 * PI)
      pInst->time = 0;
  }
}

/* Draw the background. */
void Background_Draw()
{
  if (!isLoaded)
    return;

  int i;

  float CameraX;
  float CameraY;
  AEGfxGetCamPosition(&CameraX, &CameraY);

  /* Set blend mode. */
  AEGfxSetBlendMode(AE_GFX_BM_ADD);

  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

  /* Draw each background sprite. */
  for (i = 0; i < NUM_SPRITES; ++i)
  {
    /* The current sprite. */
    BackgroundSprite * pInst = backgroundSprites[i];

    /* Transform matrices */
    Matrix2D transform, scale, rot, trans, trans_local, iso_scale;

    Matrix2DIdentity(&scale);
    Matrix2DScale(&scale, pInst->scaleX, pInst->scaleY);

    Matrix2DIdentity(&rot);
    Matrix2DRotRad(&rot, pInst->rotation);

    Matrix2DIdentity(&trans_local);
    Matrix2DTranslate(&trans_local, 0, pInst->distance + (pInst->distance * 0.5 * ((float)sin(pInst->time))));//compass.distance);

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

    AEGfxTextureSet(pInst->texture, 0, 0);

    float osc_valueR = 0.5f - 0.5f * fabsf(((float)sin(pInst->time)));
    float osc_valueG = 1 - 0.5f * fabsf(((float)sin(pInst->time - 1)));
    float osc_valueB = 1 - 0.2f * fabsf(((float)sin(pInst->time - 2)));
    float osc_valueA = fabsf(((float)sin(pInst->time - 3)));

    AEGfxSetTintColor(osc_valueR, osc_valueG, osc_valueB, osc_valueA);

    AEGfxSetTransparency(osc_valueA);

    AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

  }

}
