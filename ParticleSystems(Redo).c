/*****************************************************************************/
/*!
\file   ParticleSystems(Redo).c
\author Jack Ile
\par    email: jack.i\@digipen.edu
\brief
This is the c source code file that contains the implementation of the particle effect system
*/
/*****************************************************************************/

#include "AEEngine.h"
#include "ParticleSystems(Redo).h"
#include <math.h>
#include <stdio.h>
#include "Graphics.h"
#include "GameObject.h"

//based on float of pi * 2 = 6.2831853
#define RANDOM_ANGLE (float) ((rand() / (float)RAND_MAX) * 6.2831853)

//array of PS instances
PS_Instance		goiaPS_ObjectInstanceList[PS_OBJ_INST_MAX];

AEGfxVertexList *pTempParticleMesh;

//DEFAULT PS FUNCTIONS ARE ONLY HERE FOR SAFETY DEFAULTS AND SAMPLES FOR HOW TO WRITE YOUR OWN PS FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
\brief 
	creates a particle for a continuous PS when called.
\param i
	int for the ID of the particle.
\param pPS_Inst
	PS_Instance * for the owner continuous PS.
*/
void Default_Particle_Create_Continuous(int i, PS_Instance *pPS_Inst)
{
	//you need to create and initialize these to make the particles work properly
	float RandomAngle = RANDOM_ANGLE;
	Vector2D ParticlePosition;
	Vector2D InitialVelocity;
	PhysicsObject *ParticlePhysics;
	Animation *ParticleAnimation;
	Sprite *ParticleSprite;
	GameObject *NewParticle;

	//spawn particles at the position of the PS.
	ParticlePosition.x = pPS_Inst->PS_Continuous->StartPosX;
	ParticlePosition.y = pPS_Inst->PS_Continuous->StartPosY;
	//launch particles in a random direction at a constant speed
	InitialVelocity.x = (float)(0.1 * cos(RandomAngle));
	InitialVelocity.y = (float)(0.1 * sin(RandomAngle));

	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("isotilePlaceholder1.png"), GCreateMesh(10, 10, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, 5);
	NewParticle->simulate = Default_Particle_Simulate;
	pPS_Inst->PS_Continuous->Particle[i] = NewParticle;
}

/*!
\brief
	creates a particle for a burst PS when called.
\param i
	int for the ID of the particle.
\param pPS_Inst
	PS_Instance * for the owner burst PS.
*/
void Default_Particle_Create_Burst(int i, PS_Instance *pPS_Inst)
{
	//you need to create and initialize these to make the particles work properly
	float RandomAngle = RANDOM_ANGLE;
	Vector2D ParticlePosition;
	Vector2D InitialVelocity;
	PhysicsObject *ParticlePhysics;
	Animation *ParticleAnimation;
	Sprite *ParticleSprite;
	GameObject *NewParticle;

	//spawn particles at the position of the PS.
	ParticlePosition.x = pPS_Inst->PS_Burst->StartPosX;
	ParticlePosition.y = pPS_Inst->PS_Burst->StartPosY;
	//launch particles in a random direction at a constant speed
	InitialVelocity.x = (float)(0.1 * cos(RandomAngle));
	InitialVelocity.y = (float)(0.1 * sin(RandomAngle));

	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("isotile.png"), GCreateMesh(10, 10, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	ParticleSprite->specialFX = Default_Particle_Special_FX_Burst;
	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, entity_particle);
	NewParticle->simulate = Default_Particle_Simulate;
	pPS_Inst->PS_Burst->Particle[i] = NewParticle;
}

/*!
\brief
	default update function for an owner continuous PS
\param pPS_Inst
	PS_Instance * for the owner continuous PS.
\param frameTime
	float for the time that haa passed per frame.
*/
void Default_Particle_System_Update_Continuous(PS_Instance *pPS_Inst, float frameTime)
{

}

/*!
\brief
	default update function for an owner burst PS
\param pPS_Inst
	PS_Instance * for the owner burst PS.
\param frameTime
	float for the time that haa passed per frame.
*/
void Default_Particle_System_Update_Burst(PS_Instance *pPS_Inst, float frameTime)
{

}

/*!
\brief
	default function for particle visual fx for PS_Burst particles
*/
void Default_Particle_Special_FX_Burst(void)
{
	if (pPS_B->PS_Burst->DeathTimer > 1.0f)
	{
		AEGfxSetTintColor(1.0f, pPS_B->PS_Burst->DeathTimer / pPS_B->PS_Burst->LifeTime, 0.0f, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, pPS_B->PS_Burst->DeathTimer / pPS_B->PS_Burst->LifeTime, 0.0f, 2.0f * pPS_B->PS_Burst->DeathTimer / pPS_B->PS_Burst->LifeTime);
	}
	//AEGfxSetTransparency(pPS_B->PS_Burst->DeathTimer/pPS_B->PS_Burst->LifeTime);
}

/*!
\brief
	default function for particle update behavior
*/
void Default_Particle_Simulate(void)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
\brief
	load and safety initialize all of the PS data
*/
void LoadAll_PS(void)
{
	int i; //loop counter
	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		goiaPS_ObjectInstanceList[i].PS_Continuous = NULL;
		goiaPS_ObjectInstanceList[i].PS_Burst = NULL;
		goiaPS_ObjectInstanceList[i].ExistFlag = 0;
	}

	//form a normalized square mesh
	AEGfxMeshStart();
	//Upper Right 45-45-90 triangle
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF0080FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF0080FF, 0.0f, 0.0f,
		0.5, 0.5f, 0xFF0080FF, 0.0f, 0.0f);
	//Lower Left 45-45-90 triangle
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF0080FF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFF0080FF, 0.0f, 0.0f,
		0.5, -0.5f, 0xFF0080FF, 0.0f, 0.0f);
	pTempParticleMesh = AEGfxMeshEnd();
}

/*!
\brief
	creates a continuous PS asset.
\param LifeTime
	the life time of each of the particles it spawns
\param EmitRate
	how fast the PS emits particles
\param EmitCount
	an optional limit for how many particles to spawn (set to -1 if you want no limit).
\return
	PS_Instance * to the PS that was just created
*/
PS_Instance * Create_PS_Continuous(float LifeTime, float EmitRate, int EmitCount)
{
	int i; //loop counters
	PS_Instance *pPS_Inst;
	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;  //walk through PS instance list

		//if space is not occupied, used space for new PS
		if (pPS_Inst->ExistFlag == 0)
		{
			if (pPS_Inst->PS_Continuous == NULL)
			{
				//allocate memory
				pPS_Inst->PS_Continuous = (PS_Continuous *)calloc(1, sizeof(PS_Continuous));
				if ((EmitCount > 0) && (EmitCount < (int)(EmitRate * LifeTime * 1.1f)))
				{
					pPS_Inst->PS_Continuous->Particle = (GameObject**)calloc(EmitCount, sizeof(GameObject*));
					pPS_Inst->PS_Continuous->RefreshTimer = (float *)calloc(EmitCount, sizeof(float));

					/*
					for (j = 0; j < EmitCount; j++)
					{
						pPS_Inst->PS_Continuous->Particle[j] = NULL;
					}
					*/
				}
				else
				{
					pPS_Inst->PS_Continuous->Particle = (GameObject**)calloc((int)(EmitRate * LifeTime * 1.1f), sizeof(GameObject*));
					pPS_Inst->PS_Continuous->RefreshTimer = (float *)calloc((int)(EmitRate * LifeTime * 1.1f), sizeof(float));

					/*
					for (j = 0; j < (int)(EmitRate * LifeTime * 1.1f); j++)
					{
						pPS_Inst->PS_Continuous->Particle[j] = NULL;
					}
					*/
				}
				//initialize everything else
				pPS_Inst->PS_Continuous->LifeTime = LifeTime;
				pPS_Inst->PS_Continuous->EmitRate = EmitRate;
				pPS_Inst->PS_Continuous->EmitCount = EmitCount;
				pPS_Inst->PS_Continuous->EmitCounter = EmitCount;
				pPS_Inst->PS_Continuous->ShutDown = 1;
				pPS_Inst->PS_Continuous->EmitTimer = 1.0f / EmitRate;
				pPS_Inst->ExistFlag = 1;

				pPS_Inst->PS_Continuous->vpParticle_Create = Default_Particle_Create_Continuous;
				pPS_Inst->PS_Continuous->vpParticle_System_Update = Default_Particle_System_Update_Continuous;
				break;
			}
		}
	}
	return pPS_Inst;
}

/*!
\brief
	creates a burst PS asset.
\param LifeTime
	the life time of each of the particles it spawns
\param EmitCount
	the amount of particles that will be spawned
\return
	PS_Instance * to the PS that was just created
*/
PS_Instance * Create_PS_Burst(float LifeTime, int EmitCount)
{
	int i;	//loop counters
	PS_Instance *pPS_Inst;
	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;	//walk through PS instance list

		//if space is not occupied, used space for new PS
		if (pPS_Inst->ExistFlag == 0)
		{
			if (pPS_Inst->PS_Burst == NULL)
			{
				//allocate memory
				pPS_Inst->PS_Burst = (PS_Burst *)calloc(1, sizeof(PS_Burst));
				
				pPS_Inst->PS_Burst->Particle = (GameObject**)calloc(EmitCount, sizeof(GameObject*));

				/*
				for (j = 0; j < EmitCount; j++)
				{
					pPS_Inst->PS_Burst->Particle[j] = NULL;
				}
				*/
				//initialize everything
				pPS_Inst->PS_Burst->LifeTime = LifeTime;
				pPS_Inst->PS_Burst->DeathTimer = LifeTime;
				pPS_Inst->PS_Burst->EmitCount = EmitCount;
				pPS_Inst->PS_Burst->ShutDown = 1;
				pPS_Inst->ExistFlag = 1;

				pPS_Inst->PS_Burst->vpParticle_Create = Default_Particle_Create_Burst;
				pPS_Inst->PS_Burst->vpParticle_System_Update = Default_Particle_System_Update_Burst;
				break;
			}
		}
	}
	return pPS_Inst;
}

/*!
\brief
	starts emission of particles for a given PS
\param pPS_Inst
	PS_Instance * for the PS to begin emission.
*/
void Start_PS(PS_Instance *pPS_Inst)
{
	if (pPS_Inst->ExistFlag)
	{
		if (pPS_Inst->PS_Continuous != NULL)
		{
			pPS_Inst->PS_Continuous->ShutDown = 0;
			pPS_Inst->PS_Continuous->EmitCounter = pPS_Inst->PS_Continuous->EmitCount;
		}
		else if (pPS_Inst->PS_Burst != NULL)
		{
			pPS_Inst->PS_Burst->ShutDown = 0;
		}
	}
}

/*!
\brief
	stops emission of particles for a given PS
\param pPS_Inst
	PS_Instance * for the PS to end emission.
*/
void Stop_PS(PS_Instance *pPS_Inst)
{
	if (pPS_Inst->ExistFlag)
	{
		if (pPS_Inst->PS_Continuous != NULL)
		{
			pPS_Inst->PS_Continuous->ShutDown = 1;
		}
		else if (pPS_Inst->PS_Burst != NULL)
		{
			int j;
			pPS_Inst->PS_Burst->DeathTimer = pPS_Inst->PS_Burst->LifeTime;
			pPS_Inst->PS_Burst->ShutDown = 1;
			//clean up data used by burst PS
			for (j = 0; j < pPS_Inst->PS_Burst->EmitCount; j++)
			{
				GameObjectDestroy(&(pPS_Inst->PS_Burst->Particle[j]));
			}
		}
	}
}

/*!
\brief
	deletes a given PS from memory
\param pPS_Inst
	PS_Instance * for the PS to be deleted
*/
void Destroy_PS(PS_Instance *pPS_Inst)
{
	if (pPS_Inst->ExistFlag)
	{
		if (pPS_Inst->PS_Continuous != NULL)
		{
			int i, ParticleCount;	//loop counter
			//set ParticleCount to be total number of particles
			if ((pPS_Inst->PS_Continuous->EmitCount > 0) && (pPS_Inst->PS_Continuous->EmitCount < (int)(pPS_Inst->PS_Continuous->EmitRate * pPS_Inst->PS_Continuous->LifeTime * 1.1f)))
			{
				ParticleCount = pPS_Inst->PS_Continuous->EmitCount;
			}
			else
			{
				ParticleCount = (int)(pPS_Inst->PS_Continuous->EmitRate * pPS_Inst->PS_Continuous->LifeTime * 1.1f);
			}
			//destroy all particle objects
			for (i = 0; i < ParticleCount; i++)
			{
				if (pPS_Inst->PS_Continuous->Particle[i] != NULL)
				{
					GameObjectDestroy(&(pPS_Inst->PS_Continuous->Particle[i]));
				}
			}
			//free arrays
			free(pPS_Inst->PS_Continuous->Particle);
			free(pPS_Inst->PS_Continuous->RefreshTimer);
			//free everything else
			free(pPS_Inst->PS_Continuous);
			pPS_Inst->PS_Continuous = NULL;
			pPS_Inst->ExistFlag = 0;
		}
		else if (pPS_Inst->PS_Burst != NULL)
		{
			int i;	//loop counter
			//destroy all particle objects
			for (i = 0; i < pPS_Inst->PS_Burst->EmitCount; i++)
			{
				if (pPS_Inst->PS_Burst->Particle[i] != NULL)
				{
					GameObjectDestroy(&(pPS_Inst->PS_Burst->Particle[i]));
				}
			}
			//free particle pointer array
			free(pPS_Inst->PS_Burst->Particle);
			//free everything else
			free(pPS_Inst->PS_Burst);
			pPS_Inst->PS_Burst = NULL;
			pPS_Inst->ExistFlag = 0;
		}
	}
}

/*!
\brief
	updates all of the PS instances
\param frameTime
	float for the time that haa passed per frame.
*/
void UpdateAllPS_Inst(float frameTime)
{
	int i, j, ParticleCount;	//loop counter and loop max
	PS_Instance *pPS_Inst;

	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;	//walk through PS instance list

		//if there's no PS in the space, move on to the next one
		if (pPS_Inst->ExistFlag == 0)
			continue;

		//update continuous PS
		if (pPS_Inst->PS_Continuous != NULL)
		{
			//cancel emission process if PS is shut down.
			if (!(pPS_Inst->PS_Continuous->ShutDown))
			{
				pPS_Inst->PS_Continuous->EmitTimer -= frameTime;
			}

			//Update the PS
			if (pPS_Inst->PS_Continuous->vpParticle_System_Update != NULL)
			{
				(pPS_Inst->PS_Continuous->vpParticle_System_Update)(pPS_Inst, frameTime);
			}

			//set loop max to total number of particles
			if ((pPS_Inst->PS_Continuous->EmitCount > 0) && (pPS_Inst->PS_Continuous->EmitCount < (int)(pPS_Inst->PS_Continuous->EmitRate * pPS_Inst->PS_Continuous->LifeTime * 1.1f)))
			{
				ParticleCount = pPS_Inst->PS_Continuous->EmitCount;
			}
			else
			{
				ParticleCount = (int)(pPS_Inst->PS_Continuous->EmitRate * pPS_Inst->PS_Continuous->LifeTime * 1.1f);
			}

			//loop through particle slots
			for (j = 0; j < ParticleCount; j++)
			{
				//if particle exists and timer is up, delete it.  else, continue particle's countdowntimer
				if (pPS_Inst->PS_Continuous->RefreshTimer[j] <= 0.0f)
				{
					if (pPS_Inst->PS_Continuous->Particle[j] != NULL)
					{
						GameObjectDestroy(&(pPS_Inst->PS_Continuous->Particle[j]));
					}
					pPS_Inst->PS_Continuous->Particle[j] = NULL;
				}
				else
				{
					pPS_Inst->PS_Continuous->RefreshTimer[j] -= frameTime;
				}
				//if PS is ready to emit and there is space available, create a particle
				if ((!(pPS_Inst->PS_Continuous->ShutDown)) && (pPS_Inst->PS_Continuous->EmitTimer < 0.0f) && (pPS_Inst->PS_Continuous->Particle[j] == NULL))
				{
					(pPS_Inst->PS_Continuous->vpParticle_Create)(j, pPS_Inst);
					printf("Particle Create at %.3f x and %.3f y\n", pPS_Inst->PS_Continuous->Particle[j]->sprite->x, pPS_Inst->PS_Continuous->Particle[j]->sprite->y);

					pPS_Inst->PS_Continuous->RefreshTimer[j] = pPS_Inst->PS_Continuous->LifeTime;

					pPS_Inst->PS_Continuous->EmitTimer += 1.0f / (pPS_Inst->PS_Continuous->EmitRate);

					//if there is an EmitCount limit, decrement EmitCounter
					if (pPS_Inst->PS_Continuous->EmitCounter > 0)
					{
						--(pPS_Inst->PS_Continuous->EmitCounter);
					}
					
				}
			}
			//if there's an emit count limit, and it was reached, shut down the PS
			if (pPS_Inst->PS_Continuous->EmitCounter == 0)
			{
				pPS_Inst->PS_Continuous->EmitCounter = -1;
				pPS_Inst->PS_Continuous->ShutDown = 1;
				pPS_Inst->PS_Continuous->EmitTimer = 1.0f / (pPS_Inst->PS_Continuous->EmitRate);
			}
		}
		//update burst PS
		else if ((pPS_Inst->PS_Burst != NULL) && (pPS_Inst->PS_Burst->ShutDown != 1))
		{
			ParticleCount = pPS_Inst->PS_Burst->EmitCount;	//set loop max vairable to be total number of particles in the PS

			//if PS was just started, create all of teh necessary particles
			if (pPS_Inst->PS_Burst->DeathTimer == pPS_Inst->PS_Burst->LifeTime)
			{
				for (j = 0; j < ParticleCount; j++)
				{
					if (pPS_Inst->PS_Burst->vpParticle_Create != NULL)
					{
						(pPS_Inst->PS_Burst->vpParticle_Create)(j, pPS_Inst);
						printf("Particle Create at %.3f and %.3f\n", pPS_Inst->PS_Burst->Particle[j]->sprite->x, pPS_Inst->PS_Burst->Particle[j]->sprite->y);

					}
				}
			}
			//countdown death timer
			pPS_Inst->PS_Burst->DeathTimer -= frameTime;

			//update burst PS
			if (pPS_Inst->PS_Burst->vpParticle_System_Update != NULL)
			{
				(pPS_Inst->PS_Burst->vpParticle_System_Update)(pPS_Inst, frameTime);
			}

			//if DeathTimer reaches 0, stop the burst PS
			if (pPS_Inst->PS_Burst->DeathTimer < 0.0f)
			{
				Stop_PS(pPS_Inst);
			}
		}
		
	}

}

/*!
\brief
	unload all of the PS data
*/
void UnloadAll_PS(void)
{
	int i;	//loop counter
	PS_Instance *pPS_Inst;

	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;	//walk through PS instance list

		//if it doesn't exist, ignore it
		if (pPS_Inst->ExistFlag == 0)
			continue;

		Destroy_PS(pPS_Inst);
	}
	AEGfxMeshFree(pTempParticleMesh);
}