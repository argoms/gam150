/*!
Project (working title): Epoch
\file   EnvironmentalEffects.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Particle system declarations

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EnvironmentalEffects.h"
#include "GameObject.h"
#include <math.h>
#include "Vector2D.h"
#include "AEEngine.h"
#include "MyRandom.h"
#include "Isometric.h"
#include "AEGraphics.h"



static Animation* ParticleAnimation;


//internal value: particle system assigned lifetime lower than this number will count as immortal
static float IMMORTAL_PARTICLE_LIFETIME = -5.f;

/*!
\struct ParticleComponent
\brief Information used by individual particles within a particle system.
*/
struct ParticleComponent
{
  Vector2D velocity;
  float lifeCounter; /**<internal counter of current lifetime*/
  float maxLife; /**< base lifetime of particle*/
  int alive; /**< whether particle is currently being rendered, see ParticleStates enum*/
  float zVelocity;
  float zPosition;
  float damping;

  //random extra pointers because of how gameobject's memory management works 
  //(it doesn't like pointers to otherwise unreachable memory within "misc" components)
  float extrainfo1;
  float extrainfo2;
  int extrainfo3;


  int ownerDead; /**< flag set to true once owner system is destroyed*/
  void(*extraBehavior)(); /**< Pointer to any additional functionality (e.g. fading out)*/
};

//PROTOTYPES FOR INTERNALLY USED FUNCTIONS:
void ParticleSimulate(GameObject* inst);
void EffectSimulate(GameObject* inst);
void ParticleSimulate(GameObject* inst);
void ParticleInitialize(GameObject* inst);
static GameObject* ParticleCreate(Vector2D position, GameObject* parent, float lifeTime);

//PARTICLE BEHAVIORS:
void ParticleBehavior_FadeLinear(GameObject* inst);
void ParticleBehavior_DoorBehavior(GameObject* inst);


/*!
\brief Oh god, I've gone full Noah. Function used to create a new particle system.

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

\return Returns a pointer to the particle system GameObject
*/
GameObject* EffectCreate(Vector2D minVelocity, Vector2D maxVelocity, Vector2D position, int density, 
  float emitDelay, Vector2D zVelocityVariance, float damping, float particleLifeTime, 
  float zPosition, Vector2D positionVariance, 
  float zPositionVariance, Tint particleTint)
{
  //create new particle effect GameObject
  GameObject* newEffect = GameObjectCreate(0, 0, 0, entity_particle);
  newEffect->simulate = &EffectSimulate;

  //create the component specific to particle effect (and set values according to passed in parameters)
  EffectSource* newEffectComponent = (EffectSource*)(malloc(sizeof(EffectSource)));

  //x/y velocity:
  newEffectComponent->minVelocity = minVelocity;
  newEffectComponent->maxVelocity = maxVelocity;

  //x/y position:
  newEffectComponent->position = position;
  newEffectComponent->positionVariance = positionVariance;

  //state:
  newEffectComponent->state = particle_active;

  
  //Z-variance/position
  newEffectComponent->zPosition = zPosition;
  newEffectComponent->zVelocityVariance = zVelocityVariance;
  newEffectComponent->zPositionVariance = zPositionVariance;
  
  //particle emit rates:
  newEffectComponent->emitDelay = emitDelay;
  newEffectComponent->emitDelayCounter = 0;

  //misc:
  newEffectComponent->tint = particleTint;
  newEffectComponent->damping = damping;

  //lifetime defaults to infinite unless set after creation
  newEffectComponent->lifetime = IMMORTAL_PARTICLE_LIFETIME - 1;

  
  //instantiate particles:
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

  //assign created component to GameObject
  newEffect->miscData = newEffectComponent;

  return newEffect;
}


/*!
\brief Used internally during particle system initialization to instantiate particles.

\param parent The particle system's GameObject.
*/
static GameObject* ParticleCreate(Vector2D position, GameObject* parent, float lifeTime)
{
  //create new gameobject and parent it to the particle system
  GameObject* newParticle = GameObjectCreate(0, GCreateSprite(position.x, position.y, ParticleAnimation, 0), 0, entity_particle);
  newParticle->parent = parent;
  newParticle->simulate = &ParticleSimulate;

  //set up particle component:
  ParticleComponent* newParticleComponent = (ParticleComponent*)(malloc(sizeof(ParticleComponent)));
  newParticle->miscData = newParticleComponent;

  //initialize values:
  newParticleComponent->velocity.x = 0;
  newParticleComponent->velocity.y = 0;
  newParticleComponent->extraBehavior = NULL;
  newParticleComponent->lifeCounter = 0;
  newParticleComponent->alive = particle_inactive;
  newParticleComponent->ownerDead = 0;

  newParticleComponent->maxLife = lifeTime; 
  

  return newParticle;
  
}

/*!
\brief Function run every frame on every particle system
\param inst Pointer to GameObject being affected
*/
void EffectSimulate(GameObject* inst)
{
  EffectSource* instComponent = (EffectSource*)(inst->miscData); //grab pointer to particle system's component

  instComponent->emitDelayCounter += (float)AEFrameRateControllerGetFrameTime(); //update internal timer


  instComponent->lifetime -= (float)AEFrameRateControllerGetFrameTime(); //decrease internal lifetime counter

  //kill particle system if applicable (countdown timer < 0  and particle system not marked as immortal
  if (instComponent->lifetime < 0 && instComponent->lifetime > IMMORTAL_PARTICLE_LIFETIME)
  {
    EffectRemove(inst);
  }

  //for (int i = 0; i < instComponent->density; i++)
  //{
  //  ParticleSimulate(instComponent->particles[i]);
  //}
}

/*!
\brief Called to properly destroy a particle system (GameObjectDestroy doesn't cover everything for particles)
*/
void EffectRemove(GameObject* inst)
{
  if (!inst)
  {
    printf("EffectRemove: tried to remove null object\n");
    return;
  }
  EffectSource* instComponent = (EffectSource*)(inst->miscData);


  //Index through child particles and tell them that their parent is dead (lets them die naturally instead of instantly vanishing with their parent)
  //Huh, that phrasing.
  for (int i = 0; i < instComponent->density ; i++)
  {
    
    GameObject* current = ((GameObject*)instComponent->particles[i]);
    if ((ParticleComponent*)((current)->miscData))
    {
      ((ParticleComponent*)((current)->miscData))->ownerDead = 1;
    }
  }

  //After that, destroy the actual particle system object normally:
  GameObjectDestroy(&inst);
}

/*!
\brief Function called by every particle GameObject.
\param inst GameObject calling the function.
*/
void ParticleSimulate(GameObject* inst)
{
  ParticleComponent* instComponent = (ParticleComponent*)(inst->miscData);

  //if (!instComponent->ownerDead)
  {
    //grab a pointer to the parent particle system's component:
    EffectSource* ownerSystem = ((EffectSource*)(inst->parent->miscData));

    if (instComponent->alive == particle_active) //if the particle (not the system) is currently active, run this:
    {

      //update position based on velocity:
      inst->sprite->x += instComponent->velocity.x;
      inst->sprite->y += instComponent->velocity.y;
      inst->sprite->offset.y += instComponent->zVelocity;
      
      if (instComponent->extraBehavior) //any extra behavior if applicable
      {
        instComponent->extraBehavior(inst);
      }

      //apply damping effect (scale velocity by damping value):
      Vector2DScale(&instComponent->velocity, &instComponent->velocity, instComponent->damping);
      instComponent->zVelocity *= instComponent->damping;

      //printf("ERM %f\n, ownerSystem->damping", ownerSystem->damping);
      GSortSprite(inst->sprite, 0);

      //update lifetime counter:
      instComponent->lifeCounter += (float)AEFrameRateControllerGetFrameTime();

      //kill the thing if time has run out:
      if (instComponent->lifeCounter > instComponent->maxLife)
      {
        instComponent->alive = particle_inactive;
        instComponent->lifeCounter = 0.f;

        if (instComponent->ownerDead) //if the parent is dead, destroy the particle instead of only recycling
        {
          GameObjectDestroy(&inst);
        }
      }
    }
    else
    {
      //if the particle is inactive (not currently being rendered), do the following:

      if (instComponent->ownerDead) //if the parent is dead, destroy the object
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

        inst->sprite->tint.alpha = 0.f; //if parent is marked as inactive, go transparent
      }
    }
  }
}

/*!
\brief Called to initialize a particle system based on its parent's values. Seriously, the phrasing in this c file!
*/
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

/*!
\brief Makes all following particle animations use the given input as their particle.

Why? Because there were so many parameters in the particle system creation function that it was easier to split this stuff up.
*/
void SetParticleAnim(Animation* input)
{
  ParticleAnimation = input;
}

/*!
\brief Used to set lifetime of a particle system (make them non-immortal)
*/
void ParticleSetLifetime(GameObject* inst, float life)
{
  EffectSource* instComponent = (EffectSource*)(inst->miscData);
  instComponent->lifetime = life;
}

/*!
\brief Particle behavior for a linear fade (alpha reduces at a fixed rate)

*/
void ParticleBehavior_FadeLinear(GameObject* inst)
{
  ParticleComponent* particleComponent = (ParticleComponent*)(inst->miscData);
  inst->sprite->tint.alpha -= 0.05f;
}

/*!
\brief Particle behavior for the door particles
*/
void ParticleBehavior_DoorBehavior(GameObject* inst)
{
  ParticleComponent* particleComponent = (ParticleComponent*)(inst->miscData);
  inst->sprite->tint.alpha += 0.05f;
  inst->sprite->tint.green = RandFloat();
  inst->sprite->blendMode = AE_GFX_BM_ADD;
}

/*!
Applies a nonstandard particle behavior to the given particle system.

\param behavior See ParticleBehaviors enum
\param inst Pointer to particle system to modify
*/
void ParticleApplyBehavior(int behavior, GameObject* inst)
{
  void(*behaviorPointer)(); //pointer to behavior to apply
  EffectSource* effectComponent = (EffectSource*)(inst->miscData);;

  //set pointer according to input
  switch (behavior)
  {
  case particleBehavior_linearAlpha:
    behaviorPointer = &ParticleBehavior_FadeLinear;
    break;
  case particleBehavior_fadeIn:
    behaviorPointer = &ParticleBehavior_DoorBehavior;
    break;
  default:
    printf("ApplyParticleBehavior error: invalid particle input %i given \n", behavior);
    return;
  }
  //index through the given particle system and give them the new behavior

  ////////////////////////////////
  //NOTE:MAY NEED TO BE EffectComponent->density - 1
  //ONLY IN DEBUG MODE, THAT WON'T WORK PROPERLY IN RELEASE MODE. THIS MAKES NO SENSE I KNOW.
  //OR MAYBE IT'S RELEASE MODE NOW.
  //SERIOUSLY, IT JUST MAGICALLY STARTED WORKING IN BOTH MODES EARLIER, LET'S JUST ROLL WITH IT.
  ////////////////////////////////
  for (int i = 0; i < effectComponent->density; i++)
  {
    GameObject* pInst = effectComponent->particles[i];
    ParticleComponent* particleComponent = (ParticleComponent*)(pInst->miscData);
    particleComponent->extraBehavior = behaviorPointer;
  }
}