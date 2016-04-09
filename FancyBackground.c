/**************************************************************************************************
Author        :  Matt Chasengnou
File          :  Compass.c
Created       :  4/5/16
Description   :  Contains the functionality for doing fancy menu backgrounds.
ChangeLog

4/6/16           Adding in a base background


**************************************************************************************************/

#include <stdlib.h>
#include <Windows.h>

#include "AEEngine.h"

#include "Vector2D.h"
#include "Matrix2D.h"
#include "MyRandom.h"

#define true 1
#define false 0

//#define PI 3.14159265359

#define NUM_SPRITES 5000
#define NUM_TEXTURES 5

#define MAX_POS_X 100.f
#define MAX_POS_Y 100.f

#define MIN_DIST 50.f
#define MAX_DIST 500.f

#define MIN_OSC_X 0.05f
#define MAX_OSC_X 0.2f

#define MIN_OSC_Y 0.05f
#define MAX_OSC_Y 0.5f

#define MAX_SIZE_X 10.f
#define MIN_SIZE_X 5.f

#define MAX_SIZE_Y 100.f
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
  float direction;
}BackgroundSprite;

/* Background base sprite */
static BackgroundSprite *backbase;

/* Texture for the background. */
static AEGfxTexture *backtexture;

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

  /* Initialize the base sprite. */
  
  backtexture = AEGfxTextureLoad("animations/cameraFX/vignette_texture3.png");
  
  backbase = (BackgroundSprite *)calloc(1, sizeof(BackgroundSprite));

  backbase->scaleX = GetSystemMetrics(SM_CXSCREEN);
  backbase->scaleY = GetSystemMetrics(SM_CYSCREEN);
  backbase->texture = backtexture;
  backbase->time = RandFloatRange(0, (float)(2 * PI));

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
    pInst->direction = RandFloatRange(-1, 1);

    if (pInst->direction < 0)
      pInst->direction = -1;
    else
      pInst->direction = 1;
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

    /* Free the items for the back area. */
    AEGfxTextureUnload(backtexture);
    free(backbase);

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

  /* Update the background layer. */
  backbase->time += (float)deltaTime;

  /* Clamp time to be within 2 * PI. */
  if (backbase->time >= 2 * PI)
    backbase->time = 0;

  /* Update each background sprite. */
  for (i = 0; i < NUM_SPRITES; ++i)
  {
    /* Get the sprite. */
    BackgroundSprite *pInst = backgroundSprites[i];

    /* Update the sprite information. */

    /* Scale the sprite. */
    pInst->scaleX *= RandFloatRange(0.1f, 1.2f);// *(float)deltaTime;
    pInst->scaleY *= RandFloatRange(0.1f, 1.7f);// *(float)deltaTime;

    /* Clamp the scale. */
    pInst->scaleX = AEClamp(pInst->scaleX, MIN_SIZE_X, MAX_SIZE_X);
    pInst->scaleY = AEClamp(pInst->scaleY, MIN_SIZE_Y, MAX_SIZE_Y);

    /* Rotate the sprite. */
    pInst->rotation += (float)deltaTime * pInst->osc_speedX * pInst->direction;

    /* Update the time. */
    pInst->time += (float)deltaTime * pInst->osc_speedY;

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

  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

  /* Draw the back. */

  AEGfxSetFullTransform(CameraX, CameraY, 0, backbase->scaleX, backbase->scaleY);
  
  AEGfxTextureSet(backbase->texture, 0, 0);

  float osc_valueR = 0.5f - 0.3f * fabsf(((float)sin(backbase->time)));
  float osc_valueG = 0.2f - 0.2f * fabsf(((float)sin(backbase->time - 0.1f)));
  float osc_valueB = 0.5f - 0.2f * fabsf(((float)sin(backbase->time - 0.2f)));
  float osc_valueA = 1 - 0.5f * fabsf(((float)sin(backbase->time - 0.3f)));

  AEGfxSetBackgroundColor(osc_valueR, osc_valueG, osc_valueB);
  //AEGfxSetBackgroundColor(0, 0, 0);

  osc_valueR = 1 - 0.3f * fabsf(((float)sin(backbase->time)));
  osc_valueG = 1 - 0.5f * fabsf(((float)sin(backbase->time - 1)));
  osc_valueB = 1 - 0.2f * fabsf(((float)sin(backbase->time - 2)));
  osc_valueA = 1 - 0.5f * fabsf(((float)sin(backbase->time - 3)));

  AEGfxSetTintColor(osc_valueR, osc_valueG, osc_valueB, osc_valueA);

  AEGfxSetTransparency(osc_valueA);

  AEGfxSetBlendMode(AE_GFX_BM_ADD);

  AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);


  // DEPRECATED CODE. REPLACED WITH OPTIMIZED VERSION
  ///* Set blend mode. */
  //AEGfxSetBlendMode(AE_GFX_BM_ADD);

  ///* Draw each background sprite. */
  //for (i = 0; i < NUM_SPRITES; ++i)
  //{
  //  /* The current sprite. */
  //  BackgroundSprite * pInst = backgroundSprites[i];

  //  /* Transform matrices */
  //  Matrix2D transform, scale, rot, trans, trans_local, iso_scale;

  //  Matrix2DIdentity(&scale);
  //  Matrix2DScale(&scale, pInst->scaleX, pInst->scaleY);

  //  Matrix2DIdentity(&rot);
  //  Matrix2DRotRad(&rot, pInst->rotation);

  //  Matrix2DIdentity(&trans_local);
  //  Matrix2DTranslate(&trans_local, 0, pInst->distance + (pInst->distance * 0.9 * ((float)sin(pInst->time))));//compass.distance);

  //  Matrix2DIdentity(&trans);
  //  Matrix2DTranslate(&trans, (float)cos(pInst->time) * 50, (float)sin(pInst->time) * 50);// CameraX, CameraY);

  //  Matrix2DIdentity(&iso_scale);
  //  Matrix2DScale(&iso_scale, 1, 0.5f);

  //  /* Combine matrices. */

  //  Matrix2DConcat(&transform, &trans_local, &scale);

  //  Matrix2DConcat(&transform, &rot, &transform);

  //  //Matrix2DConcat(&transform, &trans_local, &transform);

  //  Matrix2DConcat(&transform, &iso_scale, &transform);

  //  Matrix2DConcat(&transform, &trans, &transform);

  //  //Matrix2DConcat(&iso_rotate, &iso_rotate, &rot);
  //  //Matrix2DConcat(&iso_scale, &iso_scale, &iso_rotate);

  //  //Matrix2DConcat(&transform, &, &trans_local);

  //  AEGfxSetTransform(transform.m);

  //  /* Preparing for draw */

  //  //AEGfxTextureSet(pInst->texture, 0, 0);

  //  //float osc_valueR = 1 - 0.3f * fabsf(((float)sin(pInst->time)));
  //  //float osc_valueG = 1 - 0.5f * fabsf(((float)sin(pInst->time - 1)));
  //  //float osc_valueB = 1 - 0.2f * fabsf(((float)sin(pInst->time - 2)));
  //  //float osc_valueA = fabsf(((float)sin(pInst->time - 3)));

  //  //AEGfxSetTintColor(osc_valueR, osc_valueG, osc_valueB, osc_valueA);

  //  //AEGfxSetTransparency(osc_valueA);

  //  AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

  //}

  /* Draw each background sprite. */

  /* Batch draws by texture. */
  for (i = 0; i < NUM_TEXTURES; ++i)
  {
    int j;

    /* Tint values to make different colors. */
    float osc_valueR = 1 - 0.3f * fabsf(((float)sin(backgroundSprites[NUM_SPRITES / (i + 1)]->time)));
    float osc_valueG = 1 - 0.5f * fabsf(((float)sin(backgroundSprites[NUM_SPRITES / (i + 2)]->time - 1)));
    float osc_valueB = 1 - 0.2f * fabsf(((float)sin(backgroundSprites[NUM_SPRITES / (i + 3)]->time - 2)));
    float osc_valueA = fabsf(((float)sin(backgroundSprites[NUM_SPRITES / (i + 4)]->time - 3)));

    AEGfxSetTintColor(osc_valueR, osc_valueG, osc_valueB, osc_valueA);

    /* Set the texture. */
    AEGfxTextureSet(textures[i], 0, 0);

    /* Go through the first half of the sprites with one color. */
    for (j = 0; j < NUM_SPRITES / 2; ++j)
    {
      /* The current sprite. */
      BackgroundSprite * pInst = backgroundSprites[j];
      
      /* Transform matrices */
      Matrix2D transform, scale, rot, trans, trans_local, iso_scale;

      /* If the sprite's texture matches, then draw it. */
      if (pInst->texture == textures[i])
      {
        /* Calculate transform matrices. */
        
        Matrix2DIdentity(&scale);
        Matrix2DScale(&scale, pInst->scaleX, pInst->scaleY);

        Matrix2DIdentity(&rot);
        Matrix2DRotRad(&rot, pInst->rotation);

        Matrix2DIdentity(&trans_local);
        Matrix2DTranslate(&trans_local,(float)cos(pInst->time), pInst->distance + (pInst->distance * 0.9 * (float)(sin(2 * cos(pInst->time)))));

        Matrix2DIdentity(&trans);
        Matrix2DTranslate(&trans, (float)cos(pInst->time) * 50, (float)sin(pInst->time) * 50);// CameraX, CameraY);

        Matrix2DIdentity(&iso_scale);
        Matrix2DScale(&iso_scale, 1, 0.5f);

        /* Combine matrices. */

        Matrix2DConcat(&transform, &trans_local, &scale);

        Matrix2DConcat(&transform, &rot, &transform);

        Matrix2DConcat(&transform, &iso_scale, &transform);

        Matrix2DConcat(&transform, &trans, &transform);

        AEGfxSetTransform(transform.m);

        /* Draw the sprite. */

        AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
      }
    }

    /* Tint values to make different colors */
    osc_valueR = 1 - 0.3f * fabsf(((float)sin(backgroundSprites[NUM_SPRITES / (i + 5)]->time - 4)));
    osc_valueG = 1 - 0.5f * fabsf(((float)sin(backgroundSprites[NUM_SPRITES / (i + 6)]->time - 5)));
    osc_valueB = 1 - 0.2f * fabsf(((float)sin(backgroundSprites[NUM_SPRITES / (i + 7)]->time - 6)));
    osc_valueA = fabsf(((float)sin(backgroundSprites[NUM_SPRITES / (i + 8)]->time - 7)));

    AEGfxSetTintColor(osc_valueR, osc_valueG, osc_valueB, osc_valueA);

    /* Go through the second half with a different color. */
    for (j = 0; j < NUM_SPRITES / 2; ++j)
    {
      /* The current sprite. */
      BackgroundSprite * pInst = backgroundSprites[j];

      /* Transform matrices */
      Matrix2D transform, scale, rot, trans, trans_local, iso_scale;

      /* If the sprite's texture matches, then draw it. */
      if (pInst->texture == textures[i])
      {
        /* Calculate transform matrices. */

        Matrix2DIdentity(&scale);
        Matrix2DScale(&scale, pInst->scaleX, pInst->scaleY);

        Matrix2DIdentity(&rot);
        Matrix2DRotRad(&rot, pInst->rotation);

        Matrix2DIdentity(&trans_local);
        Matrix2DTranslate(&trans_local, (float)cos(pInst->time), pInst->distance + (pInst->distance * 0.9 * ((float)sin(pInst->time))));

        Matrix2DIdentity(&trans);
        Matrix2DTranslate(&trans, (float)cos(pInst->time) * 50, (float)sin(pInst->time) * 50);// CameraX, CameraY);

        Matrix2DIdentity(&iso_scale);
        Matrix2DScale(&iso_scale, 1, 0.5f);

        /* Combine matrices. */

        Matrix2DConcat(&transform, &trans_local, &scale);

        Matrix2DConcat(&transform, &rot, &transform);

        Matrix2DConcat(&transform, &iso_scale, &transform);

        Matrix2DConcat(&transform, &trans, &transform);

        AEGfxSetTransform(transform.m);

        /* Draw the sprite. */

        AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
      }
    }

  }

  AEGfxSetFullTransform(CameraX, CameraY, 0, backbase->scaleX, backbase->scaleY);

  AEGfxTextureSet(backbase->texture, 0, 0);

  osc_valueR = 1 - 0.3f * fabsf(((float)sin(backbase->time)));
  osc_valueG = 1 - 0.5f * fabsf(((float)sin(backbase->time - 0.1f)));
  osc_valueB = 1 - 0.2f * fabsf(((float)sin(backbase->time - 0.2f)));
  osc_valueA = 1 - 0.5f * fabsf(((float)sin(backbase->time - 0.3f)));

  AEGfxSetTintColor(osc_valueR, osc_valueG, osc_valueB, osc_valueA);

  AEGfxSetTransparency(osc_valueA);

  AEGfxSetBlendMode(AE_GFX_BM_MULTIPLY);

  AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

}
