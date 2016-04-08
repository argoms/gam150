


#include "AEEngine.h"
#include <Windows.h>

#define PI 3.14159265359f

static AEGfxTexture *Screen_Texture;
static AEGfxVertexList *Mesh;
static float Scale_X, Scale_Y;

static float R = 1, G = 1, B = 1;

static float time = 0;

static int loaded = 0;

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
  Scale_X = GetSystemMetrics(SM_CXSCREEN);
  Scale_Y = GetSystemMetrics(SM_CYSCREEN);

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

/* Applies the filter. */
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

  /* Draw the filter. */

  AEGfxSetFullTransform(CameraX, CameraY, 0, Scale_X, Scale_Y);

  AEGfxTextureSet(Screen_Texture, 0, 0);

  AEGfxSetTintColor(R, G, B, 1);

  AEGfxSetTransparency(1);

  AEGfxSetBlendMode(AE_GFX_BM_MULTIPLY);

  AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
}