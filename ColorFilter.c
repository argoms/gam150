


#include "AEEngine.h"
#include <Windows.h>
#include <math.h>

#define PI 3.14159265359f

static AEGfxTexture *Screen_Texture;
static AEGfxVertexList *Mesh;
static float Scale_X, Scale_Y;

static float R = 1, G = 1, B = 1;

static float time = 0;

static int loaded = 0;

static int fluctuate = 0;
static float fluc_R = 0, fluc_G = 0, fluc_B = 0;

/* Initializes the fader. */
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

/* Unloads the filter from memory */
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

/* Sets the color of the filter. */
void ColorFilter_Set(float r, float g, float b) 
{
  R = r;
  G = g;
  B = b;
}

/* 
  Sets the fluctuation flag of the filter.
  Keep values between 0 and 1. 
*/
void ColorFilter_Fluctuate(int useF, float r_amt, float g_amt, float b_amt)
{
  fluctuate = useF;
  fluc_R = r_amt;
  fluc_G = g_amt;
  fluc_B = b_amt;
}

/* Applies the filter. */
void ColorFilter_Draw(void)
{
  return;
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

  /* Draw the filter. */

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