#include "LevelManager.h"
#include "GameObject.h"
#include "AEEngine.h"
#include "Text.h"
#include "Graphics.h"

typedef struct DeathTimer DeathTimer;

static float fade_length = 4.0f; //no defining floats

struct DeathTimer
{
  float time;
};

static GameObject* deathTimerObject;
static Sprite* fadesprite;

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
  
  fadesprite = GCreateHudSprite(0, 0, GCreateAnimation(1, GCreateTexture("splash.png"), GCreateMesh(800, 600, 1, 1), 1), 1);
  fadesprite->tint = GTint(0, 0, 0, 1);
}

void DeathTimerSimulate(GameObject* inst)
{

  //printf("%p\n", fadesprite);
  DeathTimer* dtComponent = (DeathTimer*)(inst->miscData);

  dtComponent->time -= AEFrameRateControllerGetFrameTime();
  if (dtComponent->time < 0)
  {
    LevelSetNext(level_deathScreen);
  }
}
