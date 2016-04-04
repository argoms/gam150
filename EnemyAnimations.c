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

static AnimationDefinition animationChargetato[] =
{
  { ENEMY_WALK + ENEMY_DOWN, "animations/chargertato/Walk4.png",  NULL },
  { ENEMY_WALK + ENEMY_DOWN + ENEMY_LEFT, "animations/chargertato/Walk6.png",  NULL },
  { ENEMY_WALK + ENEMY_DOWN + ENEMY_RIGHT, "animations/chargertato/Walk2.png",  NULL },
  { ENEMY_WALK + ENEMY_LEFT, "animations/chargertato/Walk8.png",  NULL },
  { ENEMY_WALK + ENEMY_RIGHT, "animations/chargertato/Walk0.png",  NULL },
  { ENEMY_WALK + ENEMY_UP, "animations/chargertato/Walk2.png",  NULL },
  { ENEMY_WALK + ENEMY_UP + ENEMY_LEFT, "animations/chargertato/Walk10.png",  NULL },
  { ENEMY_WALK + ENEMY_UP + ENEMY_RIGHT, "animations/chargertato/Walk14.png",  NULL },

  { ENEMY_IDLE + ENEMY_DOWN, "animations/chargertato/Idle4.png",  NULL },
  { ENEMY_IDLE + ENEMY_DOWN + ENEMY_LEFT, "animations/chargertato/Idle6.png",  NULL },
  { ENEMY_IDLE + ENEMY_DOWN + ENEMY_RIGHT, "animations/chargertato/Idle2.png",  NULL },
  { ENEMY_IDLE + ENEMY_LEFT, "animations/chargertato/Idle8.png",  NULL },
  { ENEMY_IDLE + ENEMY_RIGHT, "animations/chargertato/Idle0.png",  NULL },
  { ENEMY_IDLE + ENEMY_UP, "animations/chargertato/Idle12.png",  NULL },
  { ENEMY_IDLE + ENEMY_UP + ENEMY_LEFT, "animations/chargertato/Idle10.png",  NULL },
  { ENEMY_IDLE + ENEMY_UP + ENEMY_RIGHT, "animations/chargertato/Idle14.png",  NULL },

  { ENEMY_ATTACK + ENEMY_DOWN, "animations/chargertato/Slam4.png",  NULL },
  { ENEMY_ATTACK + ENEMY_DOWN + ENEMY_LEFT, "animations/chargertato/Slam6.png",  NULL },
  { ENEMY_ATTACK + ENEMY_DOWN + ENEMY_RIGHT, "animations/chargertato/Slam2.png",  NULL },
  { ENEMY_ATTACK + ENEMY_LEFT, "animations/chargertato/Slam8.png",  NULL },
  { ENEMY_ATTACK + ENEMY_RIGHT, "animations/chargertato/Slam0.png",  NULL },
  { ENEMY_ATTACK + ENEMY_UP, "animations/chargertato/Slam12.png",  NULL },
  { ENEMY_ATTACK + ENEMY_UP + ENEMY_LEFT, "animations/chargertato/Slam10.png",  NULL },
  { ENEMY_ATTACK + ENEMY_UP + ENEMY_RIGHT, "animations/chargertato/Slam14.png",  NULL }
};

void EnemyAnimationInitialize(GameObject* enemy)
{
  EnemyContainer* enemyContainer = enemy->miscData;

  int walkFrames;
  int idleFrames;
  int attackFrames;

  if (enemyContainer->enemyType == ENEMY_TYPE_MELEE || enemyContainer->enemyType == ENEMY_TYPE_RANGED)
  {
    walkFrames = 16;
    idleFrames = 1;
    attackFrames = 18;

    AEGfxVertexList* walkMesh = GCreateMesh(256.f, 256.f, walkFrames, 1);
    AEGfxVertexList* idleMesh = GCreateMesh(256.f, 256.f, idleFrames, 1);
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
  if (enemyContainer->enemyType == ENEMY_TYPE_MELEE_BIG)
  {
    walkFrames = 17;
    idleFrames = 1;
    attackFrames = 19;

    AEGfxVertexList* walkMesh = GCreateMesh(516.f, 516.f, 1, walkFrames);
    AEGfxVertexList* idleMesh = GCreateMesh(516.f, 516.f, idleFrames, 1);
    AEGfxVertexList* attackMesh = GCreateMesh(516.f, 516.f, 1, attackFrames);

    int numAnimations = sizeof(animationChargetato) / sizeof(AnimationDefinition);

    int i = 0;
    for (i = 0; i < numAnimations; ++i)
    {
      int frames;
      AEGfxVertexList *mesh;
      unsigned int maskedFlag = animationChargetato[i].action & (ENEMY_IDLE + ENEMY_WALK + ENEMY_ATTACK);
      switch (maskedFlag)
      {
      case ENEMY_IDLE:
        frames = idleFrames;
        mesh = idleMesh;
        animationChargetato[i].animation = GCreateAnimation(frames, GCreateTexture(animationChargetato[i].filename), mesh, idleFrames);
        break;
      case ENEMY_WALK:
        frames = 1;
        mesh = walkMesh;
        animationChargetato[i].animation = GCreateAnimation(frames, GCreateTexture(animationChargetato[i].filename), mesh, walkFrames);
        break;
      case ENEMY_ATTACK:
        frames = 1;
        mesh = attackMesh;
        animationChargetato[i].animation = GCreateAnimation(frames, GCreateTexture(animationChargetato[i].filename), mesh, attackFrames);
      }
    }

    enemy->sprite->animation = animationChargetato[8].animation;
    enemy->sprite->offset.y = 150.0f;
  }
}

void EnemyAnimationStateManager(GameObject* enemy)
{
  EnemyContainer* enemyContainer = enemy->miscData;
  unsigned int animationFlag = enemyContainer->enemyAnimationState;
  int numAnimations;

  if (enemyContainer->enemyType == ENEMY_TYPE_MELEE || enemyContainer->enemyType == ENEMY_TYPE_RANGED)
  {
    numAnimations = sizeof(animationSpiderWolf) / sizeof(AnimationDefinition);
    for (int i = 0; i < numAnimations; ++i)
    {
      if ((animationFlag == animationSpiderWolf[i].action) && (animationSpiderWolf[i].animation != NULL))
      {
        enemy->sprite->animation = animationSpiderWolf[i].animation;
        enemy->sprite->frameDelay = 3.0f;
        enemy->sprite->offset.y = 60.0f;
        break;
      }
    }
  }

  if (enemyContainer->enemyType == ENEMY_TYPE_MELEE_BIG)
  {
    numAnimations = sizeof(animationChargetato) / sizeof(AnimationDefinition);
    for (int i = 0; i < numAnimations; ++i)
    {
      if ((animationFlag == animationChargetato[i].action) && (animationChargetato[i].animation != NULL))
      {
        enemy->sprite->animation = animationChargetato[i].animation;
        enemy->sprite->frameDelay = 9.0f;
        enemy->sprite->offset.y = 150.0f;
        break;
      }
    }
  }
}