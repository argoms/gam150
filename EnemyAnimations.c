#include "EnemyAnimations.h"
#include "Graphics.h"
#include "AEEngine.h"

typedef struct
{
  unsigned int action;
  const char *filename;
  Animation *animation;
} AnimationDefinition;

static AnimationDefinition animationSpiderWolf[] =
{
  { ENEMY_WALK + ENEMY_DOWN, "animations/spiderwolf/walkDown.png",  NULL },
  { ENEMY_WALK + ENEMY_DOWN + ENEMY_LEFT, "animations/spiderwolf/walkDownLeft.png",  NULL },
  { ENEMY_WALK + ENEMY_DOWN + ENEMY_RIGHT, "animations/spiderwolf/walkDownRight.png",  NULL },
  { ENEMY_WALK + ENEMY_LEFT, "animations/spiderwolf/walkLeft.png",  NULL },
  { ENEMY_WALK + ENEMY_RIGHT, "animations/spiderwolf/walkRight.png",  NULL },
  { ENEMY_WALK + ENEMY_UP, "animations/spiderwolf/walkUp.png",  NULL },
  { ENEMY_WALK + ENEMY_UP + ENEMY_LEFT, "animations/spiderwolf/walkUpLeft.png",  NULL },
  { ENEMY_WALK + ENEMY_UP + ENEMY_RIGHT, "animations/spiderwolf/walkUpRight.png",  NULL },

  { ENEMY_IDLE + ENEMY_DOWN, "animations/spiderwolf/idleDown.png",  NULL },
  { ENEMY_IDLE + ENEMY_DOWN + ENEMY_LEFT, "animations/spiderwolf/idleDownLeft.png",  NULL },
  { ENEMY_IDLE + ENEMY_DOWN + ENEMY_RIGHT, "animations/spiderwolf/idleDownRight.png",  NULL },
  { ENEMY_IDLE + ENEMY_LEFT, "animations/spiderwolf/idleLeft.png",  NULL },
  { ENEMY_IDLE + ENEMY_RIGHT, "animations/spiderwolf/idleRight.png",  NULL },
  { ENEMY_IDLE + ENEMY_UP, "animations/spiderwolf/idleUp.png",  NULL },
  { ENEMY_IDLE + ENEMY_UP + ENEMY_LEFT, "animations/spiderwolf/idleUpLeft.png",  NULL },
  { ENEMY_IDLE + ENEMY_UP + ENEMY_RIGHT, "animations/spiderwolf/idleUpRight.png",  NULL }
};

void EnemyAnimationInitialize(GameObject* enemy)
{
  int walkFrames = 16;
  int idleFrames = 1;
  AEGfxVertexList* walkMesh = GCreateMesh(256.f, 256.f, walkFrames, 1);
  AEGfxVertexList* idleMesh = GCreateMesh(256.f, 256.f, idleFrames, 1);

  int numAnimations = sizeof(animationSpiderWolf) / sizeof(AnimationDefinition);

  int i = 0;
  for (i = 0; i < numAnimations; ++i)
  {
    int frames;
    AEGfxVertexList *mesh;
    unsigned int maskedFlag = animationSpiderWolf[i].action & (ENEMY_IDLE + ENEMY_WALK);
    switch (maskedFlag)
    {
    case ENEMY_IDLE:
      frames = idleFrames;
      mesh = idleMesh;
      break;
    case ENEMY_WALK:
      frames = walkFrames;
      mesh = walkMesh;
      break;
    }
    animationSpiderWolf[i].animation = GCreateAnimation(frames, GCreateTexture(animationSpiderWolf[i].filename), mesh, 1);
  }

  enemy->sprite->animation = animationSpiderWolf[8].animation;
  enemy->sprite->offset.y = 60.0f;
  /* Test code
  enemy->sprite->animation = GCreateAnimation(idleFrames,
  GCreateTexture("animations/spiderwolf/walkDown.png"),
  idleMesh,
  1);
  */
}

void EnemyAnimationStateManager(GameObject* enemy)
{
  EnemyContainer* enemyContainer = enemy->miscData;
  unsigned int animationFlag = enemyContainer->enemyAnimationState;
  int numAnimations = sizeof(animationSpiderWolf) / sizeof(AnimationDefinition);

  for (int i = 0; i < numAnimations; ++i)
  {
    if ((animationFlag == animationSpiderWolf[i].action) && (animationSpiderWolf[i].animation != NULL))
    {
      enemy->sprite->animation = animationSpiderWolf[i].animation;
      enemy->sprite->offset.y = 60.0f;
      break;
    }
  }
}