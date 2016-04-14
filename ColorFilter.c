/*!
Project (working title): Epoch
\file   ColorFilter.c
\author Matt Chasengnou
\par    email: chasengnou.m@digipen.edu
\par    Course: GAM150
\par    Created:  4/6/16
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Provides functionality for color filtering behavior.

\ChangeLog

*/

#include "AEEngine.h"
//#include <Windows.h>
#include <math.h>

/*-------------------------------------------------------------------------------------------------
DEFINES
-------------------------------------------------------------------------------------------------*/

//#define PI 3.14159265359f
/*-------------------------------------------------------------------------------------------------
END DEFINES
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
STATIC VARIABLES
-------------------------------------------------------------------------------------------------*/

static AEGfxTexture *Screen_Texture;    /* Texture used for filter. */
static AEGfxVertexList *Mesh;           /* Mesh used for filter. */

static float Scale_X, Scale_Y;          /* Draw size. */
static float R = 1, G = 1, B = 1;       /* Filter color values. */

static float time = 0;                  /* Cumulative time. Used to oscillate values. */

static int loaded = 0;                  /* Loaded state of texture and mesh. */

static int fluctuate = 0;                         /* Flag for fluctuation to oscillate color values. */
static float fluc_R = 0, fluc_G = 0, fluc_B = 0;  /* Amounts to offset each color value by. */
/*-------------------------------------------------------------------------------------------------
END STATIC VARIABLES
-------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------
FUNCTIONS
-------------------------------------------------------------------------------------------------*/

/**************************************************************************************************
Function      : ColorFilter_Init
Description   : Initializes the color filter. Call during level initialization.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void ColorFilter_Init(void)
{
  if (loaded)
    return;

  /* Get the texture. */
  Screen_Texture = AEGfxTextureLoad("animations/cameraFX/filter_texture.png");

  /* Create the mesh. */
  AEGfxMeshStart();

  AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0, 1,
    -0.5f, 0.5f, 0xFFFFFFFF, 0, 0,
    0.5f, -0.5f, 0xFFFFFFFF, 1, 1);
  AEGfxTriAdd(0.5f, 0.5f, 0xFFFFFFFF, 1, 0,
    -0.5f, 0.5f, 0xFFFFFFFF, 0, 0,
    0.5f, -0.5f, 0xFFFFFFFF, 1, 1);

  Mesh = AEGfxMeshEnd();

  /* Set size of the filter to cover entire screen. */
  Scale_X = AEGfxGetWinMaxX() - AEGfxGetWinMinX();// GetSystemMetrics(SM_CXSCREEN);
  Scale_Y = AEGfxGetWinMaxY() - AEGfxGetWinMinY();// GetSystemMetrics(SM_CYSCREEN);

  /* Fluctuation is disabled by default. */
  fluctuate = 0;
  fluc_R = 0;
  fluc_G = 0;
  fluc_B = 0;

  loaded = 1;
}

/**************************************************************************************************
Function      : ColorFilter_Unload
Description   : Unloads the filter from memory. Call during level unload.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void ColorFilter_Unload(void)
{
  if (loaded)
  {
    /* Free the mesh. */
    AEGfxMeshFree(Mesh);

    /* Free the texture. */
    AEGfxTextureUnload(Screen_Texture);

    loaded = 0;
  }
}

/**************************************************************************************************
Function      : ColorFilter_Set
Description   : Sets colors to filter out. Set values from 0 to 2. 1 retains original color.
Input         : r is amount of red to filter,
                g is amount of green to filter,
                b is amount of blue to filter.
Output        : No output.
**************************************************************************************************/
void ColorFilter_Set(float r, float g, float b) 
{
  R = r;
  G = g;
  B = b;
}

/**************************************************************************************************
Function      : ColorFilter_Fluctuate
Description   : Sets oscillating behavior of filter.
Input         : useF is value to set flag to (use 1 to activate, 0 to deactivate),
                r_amt is amount of red to oscillate (0 to 2),
                g_amt is amount of green to oscillate (0 to 2),
                b_amt is amount of blue to oscillate (0 to 2).
Output        : No output.
**************************************************************************************************/
void ColorFilter_Fluctuate(int useF, float r_amt, float g_amt, float b_amt)
{
  fluctuate = useF;
  fluc_R = r_amt;
  fluc_G = g_amt;
  fluc_B = b_amt;
}

/**************************************************************************************************
Function      : ColorFilter_Draw
Description   : Draws the filter.
Input         : No input.
Output        : No output.
**************************************************************************************************/
void ColorFilter_Draw(void)
{
  if (!loaded)
    return;

  /* Update the time. */
  time += (float)AEFrameRateControllerGetFrameTime();
  if (time >= 2 * PI)
    time = 0;

  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
  
  /* Camera position */
  float CameraX;
  float CameraY;
  AEGfxGetCamPosition(&CameraX, &CameraY);

  /* Prepare for draw. */

  AEGfxSetFullTransform(CameraX, CameraY, 0, Scale_X, Scale_Y);

  AEGfxTextureSet(Screen_Texture, 0, 0);

  /* Copies of color values so they can be modified. */
  float rCopy = R, gCopy = G, bCopy = B;

  /* Fluctuation code. */
  {
    if (fluctuate)
    {
      rCopy += fluc_R * fabsf((sinf(time)));
      gCopy += fluc_G * fabsf((sinf(time - 0.1f)));
      bCopy += fluc_B * fabsf((sinf(time - 0.2f)));
    }
  }

  /* Draw with multiply first to filter out undesired colors. */

  float r1 = min(rCopy, 1), g1 = min(gCopy, 1), b1 = min(bCopy, 1);

  AEGfxSetTintColor(r1, g1, b1, 1);

  AEGfxSetTransparency(1);

  AEGfxSetBlendMode(AE_GFX_BM_MULTIPLY);

  AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);

  /* Draw with add to enrich desired colors. */

  r1 = max(rCopy - 1, 0);
  g1 = max(gCopy - 1, 0);
  b1 = max(bCopy - 1, 0);


  AEGfxSetTintColor(r1, g1, b1, 1);

  AEGfxSetTransparency(1);

  AEGfxSetBlendMode(AE_GFX_BM_ADD);

  AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
}
/*-------------------------------------------------------------------------------------------------
END FUNCTIONS
-------------------------------------------------------------------------------------------------*/