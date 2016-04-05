#include "EnvironmentalEffects.h"
#include "GameObject.h"
#include <math.h>
#include "Vector2D.h"
#include "AEEngine.h"
#include "MyRandom.h"

#define MAX_PARTICLES_PER_EFFECT 32

typedef struct EffectSource EffectSource;
typedef struct ParticleComponent ParticleComponent;

static Animation* ParticleAnimation;

enum ParticleStates
{
  particle_active,
  particle_inactive
};

struct EffectSource
{
  GameObject* particles[MAX_PARTICLES_PER_EFFECT];

  Vector2D minVelocity;
  Vector2D maxVelocity;
  Vector2D position;
  Vector2D pLifeTime;

  float emitDelayCounter;
  float emitDelay;
  int density;
  int state;
};


struct ParticleComponent
{
  Vector2D velocity;
  float offsetY;
  float lifeCounter;
  float maxLife;
  int alive;

  void(*extraBehavior)();
};
void ParticleSimulate(GameObject* inst);
GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density);
void EffectSimulate(GameObject* inst);
void EffectRemove(GameObject* inst);
void ParticleSimulate(GameObject* inst);
void ParticleInitialize(GameObject* inst);
static GameObject* ParticleCreate(Vector2D position);



GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density, float emitDelay)
{
  GameObject* newEffect = GameObjectCreate(0, 0, 0, entity_particle);
  newEffect->simulate = &EffectSimulate;

  EffectSource* newEffectComponent = (EffectSource*)(malloc(sizeof(EffectSource)));
  newEffectComponent->minVelocity = minVelocity;
  newEffectComponent->maxVelocity = maxVelocity;
  newEffectComponent->position = position;
  newEffectComponent->state = particle_active;

  newEffectComponent->emitDelay = emitDelay;
  newEffectComponent->emitDelayCounter = 0;

  newEffect->miscData = newEffectComponent;

  for (int i = 0; i < density; i++)
  {
    if (i > MAX_PARTICLES_PER_EFFECT)
    {
      printf("DENSITY TOO HIGH");
      abort();
    }

    newEffectComponent->particles[i] = ParticleCreate(position, newEffect);
  }
}



static GameObject* ParticleCreate(Vector2D position, GameObject* parent)
{
  GameObject* newParticle = GameObjectCreate(0, GCreateSprite(position.x, position.y, ParticleAnimation, 0), 0, entity_particle);
  newParticle->parent = parent;
  newParticle->simulate = &ParticleSimulate;

  //set up particle component:
  ParticleComponent* newParticleComponent = (ParticleComponent*)(malloc(sizeof(ParticleComponent)));
  newParticle->miscData = newParticleComponent;

  newParticleComponent->velocity.x = 0;
  newParticleComponent->velocity.y = 0;
  newParticleComponent->extraBehavior = NULL;
  newParticleComponent->lifeCounter = 0;
  newParticleComponent->alive = particle_inactive;

  newParticleComponent->maxLife = 1.f; //TEMPORARY DEBUG VALUE
  
}

void EffectSimulate(GameObject* inst)
{
  EffectSource* instComponent = (EffectSource*)(inst->miscData);
  instComponent->emitDelayCounter += AEFrameRateControllerGetFrameTime();
  //for (int i = 0; i < instComponent->density; i++)
  //{
  //  ParticleSimulate(instComponent->particles[i]);
  //}
}

void EffectRemove(GameObject* inst)
{
  EffectSource* instComponent = (EffectSource*)(inst->miscData);
  for (int i = 0; i < instComponent->density; i++)
  {
    GameObjectDestroy(&(instComponent->particles[i]));
  }
  GameObjectDestroy(&inst);
}

void ParticleSimulate(GameObject* inst)
{
  
  ParticleComponent* instComponent = (ParticleComponent*)(inst->miscData);

  if (instComponent->alive == particle_active)
  {

    if (instComponent->extraBehavior)
    {
      instComponent->extraBehavior();
    }
    inst->sprite->x += instComponent->velocity.x;
    inst->sprite->y += instComponent->velocity.y;

    

    instComponent->lifeCounter += AEFrameRateControllerGetFrameTime();

    //kill the thing:
    if (instComponent->lifeCounter > instComponent->maxLife)
    {
      instComponent->alive = particle_inactive;
      instComponent->lifeCounter = 0.f;
    }
  }
  else
  {
    EffectSource* ownerSystem = ((EffectSource*)(inst->parent->miscData));
    //go full transparent if inactive (recycle if parent system is still active)
    if (ownerSystem->state == particle_active)
    {

      //printf("ISFINE");
      inst->sprite->tint.alpha = 0.f;
      if (ownerSystem->emitDelayCounter > ownerSystem->emitDelay)
      {
        ParticleInitialize(inst);
        ownerSystem->emitDelayCounter = 0.f;
        
      }
    }
    else
    {

      inst->sprite->tint.alpha = 0.f;
    }
  }
}

void ParticleInitialize(GameObject* inst)
{
  inst->sprite->tint.alpha = 1.f;
  ParticleComponent* particleComponent = (ParticleComponent*)(inst->miscData);
  EffectSource* effectComponent = (EffectSource*)(inst->parent->miscData);

  particleComponent->alive = particle_active;


  inst->sprite->x = effectComponent->position.x;
  inst->sprite->y = effectComponent->position.y;

  particleComponent->velocity.x = effectComponent->minVelocity.x + (RandFloat() * effectComponent->maxVelocity.x);
  particleComponent->velocity.y = effectComponent->minVelocity.y + (RandFloat() * effectComponent->maxVelocity.y);

}

void SetParticleAnim(Animation* input)
{
  ParticleAnimation = input;
}