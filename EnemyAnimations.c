/*!
Project (working title): Epoch
\file   EnemyAnimations.c
\author Harmonson Park
\par    email: h.noah\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
  Handles setting up enemy animations and switching them as well
*/
#include "EnemyAnimations.h"
#include "Graphics.h"
#include "AEEngine.h"
#include "EntityAnimation.h"
#include "Isometric.h"

static AnimationSet* AS_SpiderWolfIdle;
static AnimationSet* AS_SpiderWolfWalk;
static AnimationSet* AS_SpiderWolfAttack;

static AnimationSet* AS_ChargerTatoIdle;
static AnimationSet* AS_ChargerTatoWalk;
static AnimationSet* AS_ChargerTatoAttack;
static AnimationSet* AS_ChargerTatoCooldown;

static AnimationSet* AS_StabbyIdle;
static AnimationSet* AS_StabbyWalk;
static AnimationSet* AS_StabbyAttack;
static AnimationSet* AS_StabbyCooldown;

/*!
\struct AnimationDefinition
\brief 
  Animation definition, really holds the filenames for the locations of all our sprite sheets
*/
typedef struct
{
  const char *filename;
  Animation *animation;
} AnimationDefinition;

static AnimationDefinition AD_SpiderWolfIdle[] =
{
  { "animations/spiderwolf/Idle0.png",  NULL },
  { "animations/spiderwolf/Idle1.png",  NULL },
  { "animations/spiderwolf/Idle2.png",  NULL },
  { "animations/spiderwolf/Idle3.png",  NULL },
  { "animations/spiderwolf/Idle4.png",  NULL },
  { "animations/spiderwolf/Idle5.png",  NULL },
  { "animations/spiderwolf/Idle6.png",  NULL },
  { "animations/spiderwolf/Idle7.png",  NULL },
  { "animations/spiderwolf/Idle8.png",  NULL },
  { "animations/spiderwolf/Idle9.png",  NULL },
  { "animations/spiderwolf/Idle10.png",  NULL },
  { "animations/spiderwolf/Idle11.png",  NULL },
  { "animations/spiderwolf/Idle12.png",  NULL },
  { "animations/spiderwolf/Idle13.png",  NULL },
  { "animations/spiderwolf/Idle14.png",  NULL },
  { "animations/spiderwolf/Idle15.png",  NULL }
};

static AnimationDefinition AD_SpiderWolfWalk[] =
{
  { "animations/spiderwolf/Walk0.png",  NULL },
  { "animations/spiderwolf/Walk1.png",  NULL },
  { "animations/spiderwolf/Walk2.png",  NULL },
  { "animations/spiderwolf/Walk3.png",  NULL },
  { "animations/spiderwolf/Walk4.png",  NULL },
  { "animations/spiderwolf/Walk5.png",  NULL },
  { "animations/spiderwolf/Walk6.png",  NULL },
  { "animations/spiderwolf/Walk7.png",  NULL },
  { "animations/spiderwolf/Walk8.png",  NULL },
  { "animations/spiderwolf/Walk9.png",  NULL },
  { "animations/spiderwolf/Walk10.png",  NULL },
  { "animations/spiderwolf/Walk11.png",  NULL },
  { "animations/spiderwolf/Walk12.png",  NULL },
  { "animations/spiderwolf/Walk13.png",  NULL },
  { "animations/spiderwolf/Walk14.png",  NULL },
  { "animations/spiderwolf/Walk15.png",  NULL }
};

static AnimationDefinition AD_SpiderWolfAttack[] =
{
  { "animations/spiderwolf/Swipe0.png",  NULL },
  { "animations/spiderwolf/Swipe1.png",  NULL },
  { "animations/spiderwolf/Swipe2.png",  NULL },
  { "animations/spiderwolf/Swipe3.png",  NULL },
  { "animations/spiderwolf/Swipe4.png",  NULL },
  { "animations/spiderwolf/Swipe5.png",  NULL },
  { "animations/spiderwolf/Swipe6.png",  NULL },
  { "animations/spiderwolf/Swipe7.png",  NULL },
  { "animations/spiderwolf/Swipe8.png",  NULL },
  { "animations/spiderwolf/Swipe9.png",  NULL },
  { "animations/spiderwolf/Swipe10.png",  NULL },
  { "animations/spiderwolf/Swipe11.png",  NULL },
  { "animations/spiderwolf/Swipe12.png",  NULL },
  { "animations/spiderwolf/Swipe13.png",  NULL },
  { "animations/spiderwolf/Swipe14.png",  NULL },
  { "animations/spiderwolf/Swipe15.png",  NULL }
};

static AnimationDefinition AD_ChargeTatoIdle[] =
{
  { "animations/chargertato/Idle4.png",  NULL },
  { "animations/chargertato/Idle5.png",  NULL },
  { "animations/chargertato/Idle6.png",  NULL },
  { "animations/chargertato/Idle7.png",  NULL },
  { "animations/chargertato/Idle8.png",  NULL },
  { "animations/chargertato/Idle9.png",  NULL },
  { "animations/chargertato/Idle10.png",  NULL },
  { "animations/chargertato/Idle11.png",  NULL },
  { "animations/chargertato/Idle12.png",  NULL },
  { "animations/chargertato/Idle13.png",  NULL },
  { "animations/chargertato/Idle14.png",  NULL },
  { "animations/chargertato/Idle15.png",  NULL },
  { "animations/chargertato/Idle0.png",  NULL },
  { "animations/chargertato/Idle1.png",  NULL },
  { "animations/chargertato/Idle2.png",  NULL },
  { "animations/chargertato/Idle3.png",  NULL }
};

static AnimationDefinition AD_ChargeTatoWalk[] =
{
  { "animations/chargertato/Walk4.png",  NULL },
  { "animations/chargertato/Walk5.png",  NULL },
  { "animations/chargertato/Walk6.png",  NULL },
  { "animations/chargertato/Walk7.png",  NULL },
  { "animations/chargertato/Walk8.png",  NULL },
  { "animations/chargertato/Walk9.png",  NULL },
  { "animations/chargertato/Walk10.png",  NULL },
  { "animations/chargertato/Walk11.png",  NULL },
  { "animations/chargertato/Walk12.png",  NULL },
  { "animations/chargertato/Walk13.png",  NULL },
  { "animations/chargertato/Walk14.png",  NULL },
  { "animations/chargertato/Walk15.png",  NULL },
  { "animations/chargertato/Walk0.png",  NULL },
  { "animations/chargertato/Walk1.png",  NULL },
  { "animations/chargertato/Walk2.png",  NULL },
  { "animations/chargertato/Walk3.png",  NULL }
};

static AnimationDefinition AD_ChargeTatoAttack[] =
{
  { "animations/chargertato/Slam4.png",  NULL },
  { "animations/chargertato/Slam5.png",  NULL },
  { "animations/chargertato/Slam6.png",  NULL },
  { "animations/chargertato/Slam7.png",  NULL },
  { "animations/chargertato/Slam8.png",  NULL },
  { "animations/chargertato/Slam9.png",  NULL },
  { "animations/chargertato/Slam10.png",  NULL },
  { "animations/chargertato/Slam11.png",  NULL },
  { "animations/chargertato/Slam12.png",  NULL },
  { "animations/chargertato/Slam13.png",  NULL },
  { "animations/chargertato/Slam14.png",  NULL },
  { "animations/chargertato/Slam15.png",  NULL },
  { "animations/chargertato/Slam0.png",  NULL },
  { "animations/chargertato/Slam1.png",  NULL },
  { "animations/chargertato/Slam2.png",  NULL },
  { "animations/chargertato/Slam3.png",  NULL }
};

static AnimationDefinition AD_ChargeTatoCooldown[] =
{
  { "animations/chargertato/Cooldown4.png",  NULL },
  { "animations/chargertato/Cooldown5.png",  NULL },
  { "animations/chargertato/Cooldown6.png",  NULL },
  { "animations/chargertato/Cooldown7.png",  NULL },
  { "animations/chargertato/Cooldown8.png",  NULL },
  { "animations/chargertato/Cooldown9.png",  NULL },
  { "animations/chargertato/Cooldown10.png",  NULL },
  { "animations/chargertato/Cooldown11.png",  NULL },
  { "animations/chargertato/Cooldown12.png",  NULL },
  { "animations/chargertato/Cooldown13.png",  NULL },
  { "animations/chargertato/Cooldown14.png",  NULL },
  { "animations/chargertato/Cooldown15.png",  NULL },
  { "animations/chargertato/Cooldown0.png",  NULL },
  { "animations/chargertato/Cooldown1.png",  NULL },
  { "animations/chargertato/Cooldown2.png",  NULL },
  { "animations/chargertato/Cooldown3.png",  NULL }
};

static AnimationDefinition AD_StabbyIdle[] =
{
  { "animations/stabby/Idle4.png",  NULL },
  { "animations/stabby/Idle5.png",  NULL },
  { "animations/stabby/Idle6.png",  NULL },
  { "animations/stabby/Idle7.png",  NULL },
  { "animations/stabby/Idle8.png",  NULL },
  { "animations/stabby/Idle9.png",  NULL },
  { "animations/stabby/Idle10.png",  NULL },
  { "animations/stabby/Idle11.png",  NULL },
  { "animations/stabby/Idle12.png",  NULL },
  { "animations/stabby/Idle13.png",  NULL },
  { "animations/stabby/Idle14.png",  NULL },
  { "animations/stabby/Idle15.png",  NULL },
  { "animations/stabby/Idle0.png",  NULL },
  { "animations/stabby/Idle1.png",  NULL },
  { "animations/stabby/Idle2.png",  NULL },
  { "animations/stabby/Idle3.png",  NULL }
};

static AnimationDefinition AD_StabbyWalk[] =
{
  { "animations/stabby/Walk4.png",  NULL },
  { "animations/stabby/Walk5.png",  NULL },
  { "animations/stabby/Walk6.png",  NULL },
  { "animations/stabby/Walk7.png",  NULL },
  { "animations/stabby/Walk8.png",  NULL },
  { "animations/stabby/Walk9.png",  NULL },
  { "animations/stabby/Walk10.png",  NULL },
  { "animations/stabby/Walk11.png",  NULL },
  { "animations/stabby/Walk12.png",  NULL },
  { "animations/stabby/Walk13.png",  NULL },
  { "animations/stabby/Walk14.png",  NULL },
  { "animations/stabby/Walk15.png",  NULL },
  { "animations/stabby/Walk0.png",  NULL },
  { "animations/stabby/Walk1.png",  NULL },
  { "animations/stabby/Walk2.png",  NULL },
  { "animations/stabby/Walk3.png",  NULL }
};

static AnimationDefinition AD_StabbyAttack[] =
{
  { "animations/stabby/Attack4.png",  NULL },
  { "animations/stabby/Attack5.png",  NULL },
  { "animations/stabby/Attack6.png",  NULL },
  { "animations/stabby/Attack7.png",  NULL },
  { "animations/stabby/Attack8.png",  NULL },
  { "animations/stabby/Attack9.png",  NULL },
  { "animations/stabby/Attack10.png",  NULL },
  { "animations/stabby/Attack11.png",  NULL },
  { "animations/stabby/Attack12.png",  NULL },
  { "animations/stabby/Attack13.png",  NULL },
  { "animations/stabby/Attack14.png",  NULL },
  { "animations/stabby/Attack15.png",  NULL },
  { "animations/stabby/Attack0.png",  NULL },
  { "animations/stabby/Attack1.png",  NULL },
  { "animations/stabby/Attack2.png",  NULL },
  { "animations/stabby/Attack3.png",  NULL }
};

static AnimationDefinition AD_StabbyCooldown[] =
{
  { "animations/stabby/Cooldown4.png",  NULL },
  { "animations/stabby/Cooldown5.png",  NULL },
  { "animations/stabby/Cooldown6.png",  NULL },
  { "animations/stabby/Cooldown7.png",  NULL },
  { "animations/stabby/Cooldown8.png",  NULL },
  { "animations/stabby/Cooldown9.png",  NULL },
  { "animations/stabby/Cooldown10.png",  NULL },
  { "animations/stabby/Cooldown11.png",  NULL },
  { "animations/stabby/Cooldown12.png",  NULL },
  { "animations/stabby/Cooldown13.png",  NULL },
  { "animations/stabby/Cooldown14.png",  NULL },
  { "animations/stabby/Cooldown15.png",  NULL },
  { "animations/stabby/Cooldown0.png",  NULL },
  { "animations/stabby/Cooldown1.png",  NULL },
  { "animations/stabby/Cooldown2.png",  NULL },
  { "animations/stabby/Cooldown3.png",  NULL }
};

/*!
\brief
  Initializes all animations for all enemy's, stores them in static pointers for use later in the game cycle
*/
void EnemyAnimationInitialize()
{
  /*************** Spider *****************/

  int spiderWolfIdleFrames   = 1;
  int spiderWolfWalkFrames   = 17;
  int spiderWolfAttackFrames = 18;

  AEGfxVertexList* spiderWolfIdleMesh   = GCreateMesh(256.f, 256.f, (float)spiderWolfIdleFrames, 1.f);
  AEGfxVertexList* spiderWolfWalkMesh   = GCreateMesh(256.f, 256.f, (float)spiderWolfWalkFrames, 1.f);
  AEGfxVertexList* spiderWolfAttackMesh = GCreateMesh(256.f, 256.f, (float)spiderWolfAttackFrames, 1.f);

  Animation* array_spiderWolfIdle[16];
  Animation* array_spiderWolfWalk[16];
  Animation* array_spiderWolfAttack[16];

  /*************** Chargetato *****************/

  int chargerTatoIdleFrames     = 1;
  int chargerTatoWalkFrames     = 17;
  int chargerTatoAttackFrames   = 19;
  int chargerTatoCoolDownFrames = 19;

  AEGfxVertexList* chargerTatoIdleMesh     = GCreateMesh(516.f, 516.f, (float)chargerTatoIdleFrames, 1.f);
  AEGfxVertexList* chargerTatoWalkMesh     = GCreateMesh(516.f, 516.f, 1.f, (float)chargerTatoWalkFrames);
  AEGfxVertexList* chargerTatoAattackMesh  = GCreateMesh(516.f, 516.f, 1.f, (float)chargerTatoAttackFrames);
  AEGfxVertexList* chargerTatoCoolDownMesh = GCreateMesh(516.f, 516.f, 1.f, (float)chargerTatoCoolDownFrames);

  Animation* array_chargerTatoIdle[16];
  Animation* array_chargerTatoWalk[16];
  Animation* array_chargerTatoAttack[16];
  Animation* array_chargerTatoCoolDown[16];

  /*************** Stabby *****************/

  int stabbyIdleFrames = 1;
  int stabbyWalkFrames = 20;
  int stabbyAttackFrames = 31;
  int stabbyCooldownFrames = 11;

  AEGfxVertexList* mesh_stabbyIdle   = GCreateMesh(516.f, 516.f, (float)stabbyIdleFrames, 1.f);
  AEGfxVertexList* mesh_stabbyWalk   = GCreateMesh(516.f, 516.f, (float)stabbyWalkFrames, 1.f);
  AEGfxVertexList* mesh_stabbyAttack = GCreateMesh(516.f, 516.f, 4.f, 8.f);
  AEGfxVertexList* mesh_stabbyCooldown = GCreateMesh(516.f, 516.f, 1, (float)(stabbyCooldownFrames));

  Animation* array_stabbyIdle[16];
  Animation* array_stabbyWalk[16];
  Animation* array_stabbyAttack[16];
  Animation* array_stabbyCooldown[16];

  // Populate animation array
  int i = 0;
  for (i = 0; i < 16; ++i)
  {
    array_spiderWolfIdle[i]   = GCreateAnimation(spiderWolfIdleFrames, GCreateTexture(AD_SpiderWolfIdle[i].filename), spiderWolfIdleMesh,       1);
    array_spiderWolfWalk[i]   = GCreateAnimation(spiderWolfWalkFrames, GCreateTexture(AD_SpiderWolfWalk[i].filename), spiderWolfWalkMesh,       1);
    array_spiderWolfAttack[i] = GCreateAnimation(spiderWolfAttackFrames, GCreateTexture(AD_SpiderWolfAttack[i].filename), spiderWolfAttackMesh, 1);

    array_chargerTatoIdle[i]     = GCreateAnimation(1, GCreateTexture(AD_ChargeTatoIdle[i].filename), chargerTatoIdleMesh, 1);
    array_chargerTatoWalk[i]     = GCreateAnimation(1, GCreateTexture(AD_ChargeTatoWalk[i].filename), chargerTatoWalkMesh, chargerTatoWalkFrames);
    array_chargerTatoAttack[i]   = GCreateAnimation(1, GCreateTexture(AD_ChargeTatoAttack[i].filename), chargerTatoAattackMesh, chargerTatoAttackFrames);
    array_chargerTatoCoolDown[i] = GCreateAnimation(1, GCreateTexture(AD_ChargeTatoCooldown[i].filename), chargerTatoCoolDownMesh, chargerTatoCoolDownFrames);

    array_stabbyIdle[i]   = GCreateAnimation(1, GCreateTexture(AD_StabbyIdle[i].filename), mesh_stabbyIdle, 1);
    array_stabbyWalk[i]   = GCreateAnimation(stabbyWalkFrames, GCreateTexture(AD_StabbyWalk[i].filename), mesh_stabbyWalk, 1);
    array_stabbyAttack[i] = GCreateAnimation(1, GCreateTexture(AD_StabbyAttack[i].filename), mesh_stabbyAttack, 8);
    array_stabbyCooldown[i] = GCreateAnimation(1, GCreateTexture(AD_StabbyCooldown[i].filename), mesh_stabbyCooldown, stabbyCooldownFrames);
  }

  // Turn animation arrays into animation sets and give them to statics
  AS_SpiderWolfIdle   = CreateAnimationSet(array_spiderWolfIdle);
  AS_SpiderWolfWalk   = CreateAnimationSet(array_spiderWolfWalk);
  AS_SpiderWolfAttack = CreateAnimationSet(array_spiderWolfAttack);

  AS_ChargerTatoIdle     = CreateAnimationSet(array_chargerTatoIdle);
  AS_ChargerTatoWalk     = CreateAnimationSet(array_chargerTatoWalk);
  AS_ChargerTatoAttack   = CreateAnimationSet(array_chargerTatoAttack);
  AS_ChargerTatoCooldown = CreateAnimationSet(array_chargerTatoCoolDown);

  AS_StabbyIdle = CreateAnimationSet(array_stabbyIdle);
  AS_StabbyWalk = CreateAnimationSet(array_stabbyWalk);
  AS_StabbyAttack = CreateAnimationSet(array_stabbyAttack);
  AS_StabbyCooldown = CreateAnimationSet(array_stabbyCooldown);
}

/*!
\brief
  Handles animation states and switching between all of the various animations
\param enemy
  The enemy to change animations for
*/
void EnemyAnimationStateManager(GameObject* enemy)
{
  EnemyContainer* enemyContainer = enemy->miscData;
  Vector2D facingDirection;
  facingDirection.x = enemy->target->physics->position.x - enemy->physics->position.x;
  facingDirection.y = enemy->target->physics->position.y - enemy->physics->position.y;

  Vector2DNormalize(&facingDirection, &facingDirection);

  // Standard Spiders, Melee
  if (enemyContainer->enemyType == ENEMY_TYPE_MELEE)
  {
    switch (enemyContainer->enemyAnimationState)
    {
      case ENEMY_IDLE:
        enemy->sprite->animation = AnimationPlay(AS_SpiderWolfIdle, &facingDirection);
        break;
      case ENEMY_WALK:
        enemy->sprite->animation = AnimationPlay(AS_SpiderWolfWalk, &facingDirection);
        enemy->sprite->frameDelay = 3.0;
        break;
      case ENEMY_ATTACK:
        enemy->sprite->animation = AnimationPlay(AS_SpiderWolfAttack, &facingDirection);
        enemy->sprite->frameDelay = 3.0;
        break;
    }

    enemy->sprite->offset.y = 60.0f;
  }
  
  // Big enemy
  if (enemyContainer->enemyType == ENEMY_TYPE_MELEE_BIG)
  {
    switch (enemyContainer->enemyAnimationState)
    {
    case ENEMY_IDLE:
      enemy->sprite->animation = AnimationPlay(AS_ChargerTatoIdle, &facingDirection);
      break;
    case ENEMY_WALK:
      enemy->sprite->animation = AnimationPlay(AS_ChargerTatoWalk, &facingDirection);
      enemy->sprite->frameDelay = 3.0f;
      break;
    case ENEMY_ATTACK:
      enemy->sprite->animation = AnimationPlay(AS_ChargerTatoAttack, &(enemyContainer->lookDirection));
      enemy->sprite->frameDelay = 5.0f;
      break;
    case ENEMY_COOLDOWN:
      enemy->sprite->animation = AnimationPlay(AS_ChargerTatoCooldown, &(enemyContainer->lookDirection));
      enemy->sprite->frameDelay = 6.0f;
      break;
    }
    enemy->sprite->offset.y = 150.0f;
  }
  
  // Stabby charger thing
  if (enemyContainer->enemyType == ENEMY_TYPE_MELEE_CHARGE)
  {
    switch (enemyContainer->enemyAnimationState)
    {
    case ENEMY_IDLE:
      enemy->sprite->animation = AnimationPlay(AS_StabbyIdle, &facingDirection);
      enemy->sprite->offset.y = 180.0f;
      break;
    case ENEMY_WALK:
      enemy->sprite->animation = AnimationPlay(AS_StabbyWalk, &facingDirection);
      enemy->sprite->frameDelay = 3.0f;
      enemy->sprite->offset.y = 180.0f;
      break;
    case ENEMY_ATTACK:
      enemy->sprite->animation = AnimationPlay(AS_StabbyAttack, &(enemyContainer->lookDirection));
      enemy->sprite->frameDelay = 8.0f;
      enemy->sprite->offset.y = 180.0f;
      break;
    case ENEMY_COOLDOWN:
      enemy->sprite->animation = AnimationPlay(AS_StabbyCooldown, &(enemyContainer->lookDirection));
      enemy->sprite->frameDelay = 6.0f;
      enemy->sprite->offset.y = 180.0f;
    }
  }
}