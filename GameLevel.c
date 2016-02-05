#include "GameLevel.h"
#include "Physics.h"
#include "Graphics.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "Isometric.h"


static PhysicsObject* playerObject;
static Sprite* playerSprite;

void GameLevelInit()
{
  printf("game level init\n");
  PhysicsInit();
  IsoInit(16, 16);
  

  AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  //EXAMPLE ENDS HERE



  Animation* anim = GCreateAnimation(1, 
                  GCreateTexture("isotileGreen.png"),
                  GCreateMesh(128.f, 64.f, 1, 1),
                  1);

  playerSprite = GCreateSprite(0, 40, anim, 1);
  playerObject = PhysicsCreateObject(Vec2(2, 2), Vec2(1, 1));
}

void GameLevelRun()
{
  //shut up I debug how I want
  //printf("fuckoff");

  //Vector2D a = Vec2(2, 2);
  playerSprite->x = IsoWorldToScreen(&playerObject->position).x;
  playerSprite->y = IsoWorldToScreen(&playerObject->position).y;
  //printf("a %f, %f \n", IsoWorldToScreen(&playerObject->position).x, IsoWorldToScreen(&playerObject->position).y);

  InputHandle();
  PhysicsSimulate();
  AEGfxSetCamPosition(playerSprite->x, playerSprite->y);

  
}

void InputHandle()
{

  //if (AEInputCheckTriggered(VK_SPACE))
  Vector2D input = Vec2(AEInputCheckCurr(0x44) - AEInputCheckCurr(0x41),
                        AEInputCheckCurr(0x57) - AEInputCheckCurr(0x53));
  if (input.x != 0 || input.y != 0)
  {
    Vector2DNormalize(&input, &input);
    Vector2DScale(&input, &input, 5);
  }
  playerObject->velocity = IsoScreenToWorld(&input);
  GSortSprite(playerSprite, playerObject->velocity.y);
  
  
}