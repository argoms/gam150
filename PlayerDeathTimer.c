/*!
Project (working title): Epoch
\file   PlayerDeathTimer.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Creates the "you died" text and switches levels afer a set amount of time.
*/
#include "LevelManager.h"
#include "GameObject.h"
#include "AEEngine.h"
#include "Text.h"
#include "Graphics.h"
#include "Isometric.h"

typedef struct DeathTimer DeathTimer;

static float fade_length = 4.0f; //no defining floats

struct DeathTimer
{
  float time;
};

static GameObject* deathTimerObject;

static TextString* fadeText;

/*!
\param position World position of where player died.
*/
void DeathTimerStart(Vector2D position)
{
  deathTimerObject = GameObjectCreate(NULL, NULL, NULL, entity_room);

  deathTimerObject->simulate = &DeathTimerSimulate;
  DeathTimer* dtComponent = (DeathTimer*)(malloc(sizeof(DeathTimer)));
  deathTimerObject->miscData = dtComponent;

  dtComponent->time = fade_length;

  //TextCreateHUDString("YOU DIED", -100, 100);
  
  //fadesprite->tint = GTint(0, 0, 0, 1);
  Vector2D screenPos = IsoWorldToScreen(&position);
  fadeText = TextCreateString("YOU DIED", screenPos.x -100, screenPos.y);
  TextStringAddOffset(fadeText, Vec2(0, 100));
}

void DeathTimerSimulate(GameObject* inst)
{
  //TextRemoveString(fadeText);
  //fadeText = TextCreateHUDString("YOU DIED", -100, 100);

  

  //printf("%p\n", fadesprite);
  DeathTimer* dtComponent = (DeathTimer*)(inst->miscData);

  dtComponent->time -= (float)AEFrameRateControllerGetFrameTime();
  if (dtComponent->time < 0)
  {
    //TextRemoveString(fadeText);
    
    LevelSetNext(level_deathScreen);
    //GameObjectDestroy(deathTimerObject);
  }
}
