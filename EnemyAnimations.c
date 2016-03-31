#pragma once
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
  { ENEMY_IDLE + ENEMY_UP + ENEMY_RIGHT, "animations/spiderwolf/idleUpRight.png",  NULL },

  { ENEMY_ATTACK + ENEMY_DOWN, "animations/spiderwolf/SwipeDown.png",  NULL },
  { ENEMY_ATTACK + ENEMY_DOWN + ENEMY_LEFT, "animations/spiderwolf/SwipeDownLeft.png",  NULL },
  { ENEMY_ATTACK + ENEMY_DOWN + ENEMY_RIGHT, "animations/spiderwolf/SwipeDownRight.png",  NULL },
  { ENEMY_ATTACK + ENEMY_LEFT, "animations/spiderwolf/SwipeLeft.png",  NULL },
  { ENEMY_ATTACK + ENEMY_RIGHT, "animations/spiderwolf/SwipeRight.png",  NULL },
  { ENEMY_ATTACK + ENEMY_UP, "animations/spiderwolf/SwipeUp.png",  NULL },
  { ENEMY_ATTACK + ENEMY_UP + ENEMY_LEFT, "animations/spiderwolf/SwipeUpLeft.png",  NULL },
  { ENEMY_ATTACK + ENEMY_UP + ENEMY_RIGHT, "animations/spiderwolf/SwipeUpRight.png",  NULL }
};

void EnemyAnimationInitialize(GameObject* enemy)
{
  int walkFrames = 16;
  int idleFrames = 1;
  int attackFrames = 18;
  AEGfxVertexList* walkMesh   = GCreateMesh(256.f, 256.f, walkFrames,   1);
  AEGfxVertexList* idleMesh   = GCreateMesh(256.f, 256.f, idleFrames,   1);
  AEGfxVertexList* attackMesh = GCreateMesh(256.f, 256.f, attackFrames, 1);

  int numAnimations = sizeof(animationSpiderWolf) / sizeof(AnimationDefinition);

  int i = 0;
  for (i = 0; i < numAnimations; ++i)
  {
    int frames;
    AEGfxVertexList *mesh;
    unsigned int maskedFlag = animationSpiderWolf[i].action & (ENEMY_IDLE + ENEMY_WALK + ENEMY_ATTACK);
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
    case ENEMY_ATTACK:
      frames = attackFrames;
      mesh = attackMesh;
    }
    animationSpiderWolf[i].animation = GCreateAnimation(frames, GCreateTexture(animationSpiderWolf[i].filename), mesh, 1);
  }

  enemy->sprite->animation = animationSpiderWolf[8].animation;
  enemy->sprite->offset.y = 60.0f;
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