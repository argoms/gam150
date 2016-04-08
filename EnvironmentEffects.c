#include "EnvironmentalEffects.h"
#include "GameObject.h"
#include <math.h>
#include "Vector2D.h"
#include "AEEngine.h"
#include "MyRandom.h"
#include "Isometric.h"



static Animation* ParticleAnimation;




struct ParticleComponent
{
  Vector2D velocity;
  float offsetY;
  float lifeCounter;
  float maxLife;
  int alive;
  float zVelocity;
  float zPosition;
  float damping;


  int ownerDead; //flag set to true once owner system is destroyed
  void(*extraBehavior)();
};
void ParticleSimulate(GameObject* inst);
GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density);
void EffectSimulate(GameObject* inst);
void ParticleSimulate(GameObject* inst);
void ParticleInitialize(GameObject* inst);
static GameObject* ParticleCreate(Vector2D position, GameObject* parent, float lifeTime);



/*!
\brief Oh god, I've gone full Noah.

\param minVelocity base velocity value
\param maxVelocity added velocity value (0-100% of value added randomly)

\param position position to spawn system at (screen coordinates)
\param density number of instantiated particles
\param emitDelay time between particle emits

\param zVelocityVariance min/max values of z velocity
\param damping Velocity multiplier to apply every frame
\param particleLifeTime how long each particle lasts

\param zPosition starting z position (y offset independent of layering)

\param positionVariance a random scalar between -50% and 50% (axis independent) of x and y are added to position on spawning, centered on position
\param zPositionVariance random scalar between -50% and 50% of this number is added to z position on spawning, centered on position
*/
GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density, 
  float emitDelay, Vector2D zVelocityVariance, float damping, float particleLifeTime, 
  float zPosition, Vector2D positionVariance, 
  float zPositionVariance, Tint particleTint)
{
  /*
  if (convertType == transform_screenToWorld)
  {
    Vector2D minVel = Vec2(minVelocity.x, minVelocity.y);
    minVelocity = IsoScreenToWorld(&minVel);
    
    Vector2D maxVel = Vec2(maxVelocity.x, maxVelocity.y);
    maxVelocity = IsoScreenToWorld(&maxVel);
    //printf("pretrapos: %f, %f", position.x, position.y);


    Vector2D pos = Vec2(position.x, position.y);
    position = IsoScreenToWorld(&pos);
    //printf("postrapos: %f, %f\n", position.x, position.y);
  }
  else if (convertType == transform_worldToScreen)
  {
    Vector2D minVel = Vec2(minVelocity.x, minVelocity.y);
    minVelocity = IsoWorldToScreen(&minVel);

    Vector2D maxVel = Vec2(maxVelocity.x, maxVelocity.y);
    maxVelocity = IsoWorldToScreen(&maxVel);
    //printf("pretrapos: %f, %f", position.x, position.y);


    Vector2D pos = Vec2(position.x, position.y);
    position = IsoWorldToScreen(&pos);
  }*/

  GameObject* newEffect = GameObjectCreate(0, 0, 0, entity_particle);
  newEffect->simulate = &EffectSimulate;

  EffectSource* newEffectComponent = (EffectSource*)(malloc(sizeof(EffectSource)));
  newEffectComponent->minVelocity = minVelocity;
  newEffectComponent->maxVelocity = maxVelocity;
  newEffectComponent->position = position;
  newEffectComponent->zPosition = zPosition;
  newEffectComponent->state = particle_active;
  newEffectComponent->damping = damping;
  newEffectComponent->zVelocityVariance = zVelocityVariance;

  newEffectComponent->zVelocityVariance = zVelocityVariance;
  newEffectComponent->zPositionVariance = zPositionVariance;
  newEffectComponent->positionVariance = positionVariance;

  printf("%f %f \n", newEffectComponent->positionVariance.x, newEffectComponent->positionVariance.y);

  newEffectComponent->emitDelay = emitDelay;
  newEffectComponent->emitDelayCounter = 0;
  newEffectComponent->tint = particleTint;

  

  newEffectComponent->density = density;
  for (int i = 0; i < density; i++)
  {
    if (i > MAX_PARTICLES_PER_EFFECT)
    {
      printf("DENSITY TOO HIGH");
      abort();
    }

    newEffectComponent->particles[i] = ParticleCreate(position, newEffect, particleLifeTime);
    //printf("asd asd %p", newEffectComponent->particles[i]);
  }

  printf("asd asd %p", newEffectComponent->particles[0]);
  newEffect->miscData = newEffectComponent;
  return newEffect;
}



static GameObject* ParticleCreate(Vector2D position, GameObject* parent, float lifeTime)
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
  newParticleComponent->ownerDead = 0;

  newParticleComponent->maxLife = lifeTime; 

  return newParticle;
  
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
  if (!inst)
  {
    printf("EffectRemove: tried to remove null object\n");
    return;
  }
  //printf("AAAAA");
  EffectSource* instComponent = (EffectSource*)(inst->miscData);


  //printf("%i asdasdADs", instComponent->density);
  for (int i = 0; i < instComponent->density ; i++)
  {
    
    GameObject* current = ((GameObject*)instComponent->particles[i]);

    
   // ->parent = NULL;
   // ((GameObject*)instComponent->particles[i])->simulate = NULL;

    printf("%p \n", instComponent->particles[i]);
    if ((ParticleComponent*)((current)->miscData))
    {
      //printf("%p \n", 0);//(ParticleComponent*)((current)->miscData));
      //continue;
      ((ParticleComponent*)((current)->miscData))->ownerDead = 1;
    }
    //printf("YUNONUL %p", ((GameObject*)instComponent->particles[i])->parent);
    //GameObjectDestroy(&(instComponent->particles[i]));
  }
  GameObjectDestroy(&inst);
}

void ParticleSimulate(GameObject* inst)
{
  ParticleComponent* instComponent = (ParticleComponent*)(inst->miscData);
  //if (!instComponent->ownerDead)
  {
    //printf("IMOK, %p", inst->parent);
    //ParticleComponent* instComponent = (ParticleComponent*)(inst->miscData);
    EffectSource* ownerSystem = ((EffectSource*)(inst->parent->miscData));

    if (instComponent->alive == particle_active)
    {

      if (instComponent->extraBehavior)
      {
        instComponent->extraBehavior();
      }
      inst->sprite->x += instComponent->velocity.x;
      inst->sprite->y += instComponent->velocity.y;
      inst->sprite->offset.y += instComponent->zVelocity;

      Vector2DScale(&instComponent->velocity, &instComponent->velocity, instComponent->damping);
      instComponent->zVelocity *= instComponent->damping;

      //printf("ERM %f\n, ownerSystem->damping", ownerSystem->damping);
      GSortSprite(inst->sprite, 0);


      instComponent->lifeCounter += AEFrameRateControllerGetFrameTime();

      //kill the thing:
      if (instComponent->lifeCounter > instComponent->maxLife)
      {
        instComponent->alive = particle_inactive;
        instComponent->lifeCounter = 0.f;
        if (instComponent->ownerDead)
        {
          GameObjectDestroy(&inst);
        }
      }
    }
    else
    {
      //printf("AAAA");
      if (instComponent->ownerDead)
      {
        inst->sprite->tint.alpha = 0.f;
        GameObjectDestroy(&inst);
        return;
        
      }
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
}

void ParticleInitialize(GameObject* inst)
{
  inst->sprite->tint.alpha = 1.f;
  ParticleComponent* particleComponent = (ParticleComponent*)(inst->miscData);
  EffectSource* effectComponent = (EffectSource*)(inst->parent->miscData);

  particleComponent->ownerDead = 0;
  particleComponent->alive = particle_active;
  particleComponent->damping = effectComponent->damping;

  //start pos       center of particle system      plus random number up to variance                     minus half variance to center at position
  inst->sprite->x = effectComponent->position.x + (RandFloat() * effectComponent->positionVariance.x) - (effectComponent->positionVariance.x / 2);
  inst->sprite->y = effectComponent->position.y + (RandFloat() * effectComponent->positionVariance.y) - (effectComponent->positionVariance.x / 2);

  //net starting velocity         base velocity                    random number up to max velocity added
  particleComponent->velocity.x = effectComponent->minVelocity.x + (RandFloat() * effectComponent->maxVelocity.x);
  particleComponent->velocity.y = effectComponent->minVelocity.y + (RandFloat() * effectComponent->maxVelocity.y);

  particleComponent->zVelocity = effectComponent->zVelocityVariance.x + (RandFloat() * effectComponent->zVelocityVariance.y);

  inst->sprite->offset.y = effectComponent->zPosition +(RandFloat() * effectComponent->zPositionVariance) - (effectComponent->zPositionVariance / 2);
  inst->sprite->tint = effectComponent->tint;

  
}

void SetParticleAnim(Animation* input)
{
  ParticleAnimation = input;
}