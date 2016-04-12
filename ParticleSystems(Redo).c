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
#include "GameLevel.h"

//based on float of pi * 2 = 6.2831853
#define RANDOM_ANGLE (float) ((rand() / (float)RAND_MAX) * 6.2831853)

//array of PS instances
PS_Instance		goiaPS_ObjectInstanceList[PS_OBJ_INST_MAX];

float           VectorX, VectorY, ImpactAngle, HealthGatherAngle;

AEGfxVertexList *pTempParticleMesh;

//IMPORTANT PS STRUCTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct ParticleData
{
	int PS_ID;
	int ParticleID;
}ParticleData;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//FUNCTIONS FOR SPAWNING SPECIFIC PSs
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
\brief
Spawns a Dodge Smoke PS
\param StartPosX
float for the PS's Starting X Position.
\param StartPosY
float for the PS's Starting Y Position.
*/
void SpawnDodgeSmokePS(float StartPosX, float StartPosY)
{
	int i;

	for (i = 0; i < sizeof(pDodgeSmoke) / sizeof(*pDodgeSmoke); i++)
	{
		if (pDodgeSmoke[i]->PS_Burst->ShutDown)
		{
			pDodgeSmoke[i]->PS_Burst->StartPosX = StartPosX;
			pDodgeSmoke[i]->PS_Burst->StartPosY = StartPosY;
			Start_PS(pDodgeSmoke[i]);
			break;
		}
	}
}

/*!
\brief
Spawns a Dodge Smoke PS
\param StartPosX
float for the PS's Starting X Position.
\param StartPosY
float for the PS's Starting Y Position.
*/
void SpawnDodgeSmoke2PS(float StartPosX, float StartPosY)
{
	int i;

	for (i = 0; i < sizeof(pDodgeSmoke2) / sizeof(*pDodgeSmoke2); i++)
	{
		if (pDodgeSmoke2[i]->PS_Burst->ShutDown)
		{
			pDodgeSmoke2[i]->PS_Burst->StartPosX = StartPosX;
			pDodgeSmoke2[i]->PS_Burst->StartPosY = StartPosY;
			Start_PS(pDodgeSmoke2[i]);
			break;
		}
	}
}

/*!
\brief
Spawns a Fire Hazard PS
\param StartPosX
float for the PS's Starting X Position.
\param StartPosY
float for the PS's Starting Y Position.
*/
void SpawnFireHazardPS(float StartPosX, float StartPosY)
{
	int i;

	for (i = 0; i < sizeof(pFireHazard) / sizeof(*pFireHazard); i++)
	{
		if (pFireHazard[i]->PS_Burst->ShutDown)
		{
			pFireHazard[i]->PS_Continuous->StartPosX = StartPosX;
			pFireHazard[i]->PS_Continuous->StartPosY = StartPosY;
			Start_PS(pFireHazard[i]);
			break;
		}
	}
}

/*!
\brief
Spawns a Hit Splash PS 2
\param PlayerPosX
float for the Player's X Position.
\param PlayerPosY
float for the Player's Y Position.
\param EnemyPosX
float for the Enemy's X Position and the PS's starting X Position.
\param EnemyPosY
float for the Enemy's Y Position and the PS's starting Y Position.
*/
void SpawnHitSplashPS(float PlayerPosX, float PlayerPosY, float EnemyPosX, float EnemyPosY)
{
	int i;

	VectorX = EnemyPosX - PlayerPosX;
	VectorY = EnemyPosY - PlayerPosY;
	for (i = 0; i < sizeof(pHitSplash) / sizeof(*pHitSplash); i++)
	{
		if (pHitSplash[i]->PS_Burst->ShutDown)
		{
			pHitSplash[i]->PS_Burst->StartPosX = EnemyPosX;
			pHitSplash[i]->PS_Burst->StartPosY = EnemyPosY;
			Start_PS(pHitSplash[i]);
			break;
		}
	}
}

/*!
\brief
Spawns a Hit Splash PS 2
\param PlayerPosX
float for the Player's X Position.
\param PlayerPosY
float for the Player's Y Position.
\param EnemyPosX
float for the Enemy's X Position and the PS's starting X Position.
\param EnemyPosY
float for the Enemy's Y Position and the PS's starting Y Position.
*/
void SpawnHitSplash2PS(float PlayerPosX, float PlayerPosY, float EnemyPosX, float EnemyPosY)
{
	int i;

	VectorX = EnemyPosX - PlayerPosX;
	VectorY = EnemyPosY - PlayerPosY;
	for (i = 0; i < sizeof(pHitSplash2) / sizeof(*pHitSplash2); i++)
	{
		if (pHitSplash2[i]->PS_Burst->ShutDown)
		{
			pHitSplash2[i]->PS_Burst->StartPosX = EnemyPosX;
			pHitSplash2[i]->PS_Burst->StartPosY = EnemyPosY;
			Start_PS(pHitSplash2[i]);
			break;
		}
	}
}

/*!
\brief
Spawns a Health Gather PS
\param StartPosX
float for the PS's Starting X Position.  Recommended to be the position of the health pick-up.
\param StartPosY
float for the PS's Starting Y Position.  Recommended to be the position of the health pick-up.
*/
void SpawnHealthGatherPS(float StartPosX, float StartPosY)
{
	int i;
	HealthGatherAngle = RANDOM_ANGLE;

	for (i = 0; i < sizeof(pHealthGather) / sizeof(*pHealthGather); i++)
	{
		if (pHealthGather[i]->PS_Burst->ShutDown)
		{
			pHealthGather[i]->PS_Burst->StartPosX = StartPosX;
			pHealthGather[i]->PS_Burst->StartPosY = StartPosY;
			Start_PS(pHealthGather[i]);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//DEFAULT PS FUNCTIONS ARE ONLY HERE FOR SAFETY DEFAULTS AND SAMPLES FOR HOW TO WRITE YOUR OWN PS FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
\brief
creates a particle for a Health Gather
\param i
int for the ID of the particle.
\param pPS_Inst
PS_Instance * for the owner continuous PS.
*/
void Particle_Create_HealthGather(int i, PS_Instance *pPS_Inst)
{
	int j;
	//you need to create and initialize these to make the particles work properly
	float RandomScale = (float)((rand() / (float)RAND_MAX));
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
	InitialVelocity.x = (float)(0.1 * cos(HealthGatherAngle + i * PI / 4));
	InitialVelocity.y = (float)(0.1 * sin(HealthGatherAngle + i * PI / 4));

	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("WhiteBox.png"), GCreateMesh(10, 10, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	ParticleSprite->specialFX = Particle_Special_FX_HealthGather;
	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, entity_particle);
	NewParticle->simulate = Particle_Simulate_HealthGather;

	NewParticle->miscData = malloc(sizeof(ParticleData));
	((ParticleData *)(NewParticle->miscData))->ParticleID = i;
	for (j = 0; j < sizeof(pHealthGather) / sizeof(*pHealthGather); j++)
	{
		if (pPS_Inst == pHealthGather[j])
		{
			((ParticleData *)(NewParticle->miscData))->PS_ID = j;
			break;
		}
	}
	
	ParticleSprite->owner = NewParticle;
	pPS_Inst->PS_Burst->Particle[i] = NewParticle;
}

/*!
\brief
particle update behavior for Health Gather
*/
void Particle_Simulate_HealthGather(void)
{

}

/*!
\brief
particle special visual effect for Health Gather
*/
void Particle_Special_FX_HealthGather(Sprite *Owner)
{
	float VisualMod = pHealthGather[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pHealthGather[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime * 0.5f;
	float x = GetPlayerObject()->sprite->x - Owner->x;
	float y = GetPlayerObject()->sprite->y - Owner->y;

	/*
	float x = GetPlayerObject()->sprite->x - Owner->x;
	float y = GetPlayerObject()->sprite->y - Owner->y;
	/*
	float x = GetPlayerObject()->sprite->x - pHealthGather[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->StartPosX;
	float y = GetPlayerObject()->sprite->y - pHealthGather[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->StartPosY;
	*/
	float length = (float)sqrt(x * x + y * y);
	x /= length;
	y /= length;

	Owner->owner->physics->velocity.x += x / 10.0f;
	Owner->owner->physics->velocity.y += y / 10.0f;

	AEGfxSetTintColor(1.0f - VisualMod, 1.0f, 1.0f - VisualMod, 0.8f);

	/*
	if (((ParticleData *)(Owner->owner->miscData))->ParticleID < 5)
	{
		if (pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 0.1f)
		{
			AEGfxSetTintColor(0.2f, 1.0f, 0.3f, 1.0f);
		}
		else
		{
			AEGfxSetTintColor(0.2f, 1.0f, 0.3f, 5.0f * pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
		}
	}
	else
	{
		if (pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 0.1f)
		{
			AEGfxSetTintColor(1.0f, 0.5f, 0.0f, 1.0f);
		}
		else
		{
			AEGfxSetTintColor(1.0f, 0.5f, 0.0f, 5.0f * pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
		}
	}
	*/
}

/*!
\brief
creates a particle for a Player Damage
\param i
int for the ID of the particle.
\param pPS_Inst
PS_Instance * for the owner continuous PS.
*/
void Particle_Create_PlayerDamage(int i, PS_Instance *pPS_Inst)
{
	int j;
	//you need to create and initialize these to make the particles work properly
	float RandomAngle = RANDOM_ANGLE;
	float RandomScale = (float)((rand() / (float)RAND_MAX));
	float RandomScale2 = (float)((rand() / (float)RAND_MAX));
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
	InitialVelocity.x = (float)((0.1 + RandomScale * 0.05) * cos(RandomAngle));
	InitialVelocity.y = (float)((0.1 + RandomScale * 0.05) * sin(RandomAngle));

	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("WhiteBox.png"), GCreateMesh(10 + 5 * RandomScale2, 10 + 5 * RandomScale2, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	ParticleSprite->specialFX = Particle_Special_FX_PlayerDamage;
	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, entity_particle);
	NewParticle->simulate = Particle_Simulate_PlayerDamage;

	NewParticle->miscData = malloc(sizeof(ParticleData));
	((ParticleData *)(NewParticle->miscData))->ParticleID = i;
	for (j = 0; j < sizeof(pPlayerDamage) / sizeof(*pPlayerDamage); j++)
	{
		if (pPS_Inst == pPlayerDamage[j])
		{
			((ParticleData *)(NewParticle->miscData))->PS_ID = j;
			break;
		}
	}

	ParticleSprite->owner = NewParticle;
	pPS_Inst->PS_Burst->Particle[i] = NewParticle;
}

/*!
\brief
particle update behavior for Hit Splash 2
*/
void Particle_Simulate_PlayerDamage(void)
{

}

/*!
\brief
particle special visual effect for Hit Splash 2
*/
void Particle_Special_FX_PlayerDamage(Sprite *Owner)
{
	Owner->owner->physics->velocity.x  -= 0.2f * pPlayerDamage[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pPlayerDamage[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime;
	Owner->owner->physics->velocity.y  -= 0.2f * pPlayerDamage[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pPlayerDamage[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime;

	if (((ParticleData *)(Owner->owner->miscData))->ParticleID < 5)
	{
		AEGfxSetTintColor(0.0f, 0.0f, 0.0f, 0.2f + 0.8f * pPlayerDamage[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pPlayerDamage[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 0.0f, 0.0f, 0.2f + 0.8f * pPlayerDamage[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pPlayerDamage[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
	}
}

/*!
\brief
creates a particle for a Hit Splash 2
\param i
int for the ID of the particle.
\param pPS_Inst
PS_Instance * for the owner continuous PS.
*/
void Particle_Create_HitSplash2(int i, PS_Instance *pPS_Inst)
{
	int j;
	//you need to create and initialize these to make the particles work properly
	float RandomAngle = RANDOM_ANGLE;
	float RandomAngle2 = RANDOM_ANGLE;
	float RandomScale = (float)((rand() / (float)RAND_MAX));
	float RandomScale2 = (float)((rand() / (float)RAND_MAX));
	Vector2D ParticlePosition;
	Vector2D InitialVelocity;
	PhysicsObject *ParticlePhysics;
	Animation *ParticleAnimation;
	Sprite *ParticleSprite;
	GameObject *NewParticle;

	ImpactAngle = (float)acos(VectorX / sqrt(VectorX * VectorX + VectorY * VectorY));
	if (asin(VectorX / sqrt(VectorX * VectorX + VectorY * VectorY)) < 0)
	{
		ImpactAngle = -ImpactAngle;
	}

	//spawn particles at the position of the PS.
	ParticlePosition.x = pPS_Inst->PS_Burst->StartPosX;
	ParticlePosition.y = pPS_Inst->PS_Burst->StartPosY;
	//launch particles in a random direction at a constant speed
	InitialVelocity.x = (float)((0.2 + RandomScale * 0.4) * cos(ImpactAngle + (RandomAngle - PI) / 8));
	InitialVelocity.y = (float)((0.2 + RandomScale * 0.4) * sin(ImpactAngle + (RandomAngle - PI) / 8));

	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticlePhysics->angle = RandomAngle2;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("WhiteBox.png"), GCreateMesh(5 + 10 * RandomScale2, 5 + 10 * RandomScale2, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	ParticleSprite->specialFX = Particle_Special_FX_HitSplash2;
	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, entity_particle);
	NewParticle->simulate = Particle_Simulate_HitSplash2;

	NewParticle->miscData = malloc(sizeof(ParticleData));
	((ParticleData *)(NewParticle->miscData))->ParticleID = i;
	for (j = 0; j < sizeof(pHitSplash2) / sizeof(*pHitSplash2); j++)
	{
		if (pPS_Inst == pHitSplash2[j])
		{
			((ParticleData *)(NewParticle->miscData))->PS_ID = j;
			break;
		}
	}

	ParticleSprite->owner = NewParticle;
	pPS_Inst->PS_Burst->Particle[i] = NewParticle;
}

/*!
\brief
particle update behavior for Hit Splash 2
*/
void Particle_Simulate_HitSplash2(void)
{

}

/*!
\brief
particle special visual effect for Hit Splash 2
*/
void Particle_Special_FX_HitSplash2(Sprite *Owner)
{
	if (((ParticleData *)(Owner->owner->miscData))->ParticleID < 3)
	{
		if (pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 0.15f)
		{
			AEGfxSetTintColor(0.1f, 0.2f, 0.7f, 1.0f);
		}
		else
		{
			AEGfxSetTintColor(0.1f, 0.2f, 0.7f, 0.2f + 0.8f * pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
		}
	}
	else if (((ParticleData *)(Owner->owner->miscData))->ParticleID < 6)
	{
		if (pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 0.15f)
		{
			AEGfxSetTintColor(0.0f, 0.7f, 0.1f, 1.0f);
		}
		else
		{
			AEGfxSetTintColor(0.0f, 0.7f, 0.1f, 0.2f + 0.8f * pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
		}
	}
	else
	{
		if (pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 0.15f)
		{
			AEGfxSetTintColor(0.9f, 0.05f, 0.0f, 1.0f);
		}
		else
		{
			AEGfxSetTintColor(0.9f, 0.05f, 0.0f, 0.2f + 0.8f * pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pHitSplash2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
		}
	}
}

/*!
\brief
creates a particle for a Hit Splash
\param i
int for the ID of the particle.
\param pPS_Inst
PS_Instance * for the owner continuous PS.
*/
void Particle_Create_HitSplash(int i, PS_Instance *pPS_Inst)
{
	int j;
	//you need to create and initialize these to make the particles work properly
	float RandomAngle = RANDOM_ANGLE;
	float RandomScale = (float)((rand() / (float)RAND_MAX));
	Vector2D ParticlePosition;
	Vector2D InitialVelocity;
	PhysicsObject *ParticlePhysics;
	Animation *ParticleAnimation;
	Sprite *ParticleSprite;
	GameObject *NewParticle;

	ImpactAngle = (float)acos(VectorX / sqrt(VectorX * VectorX + VectorY * VectorY));
	if (asin(VectorX / sqrt(VectorX * VectorX + VectorY * VectorY)) < 0)
	{
		ImpactAngle = -ImpactAngle;
	}

	//spawn particles at the position of the PS.
	ParticlePosition.x = pPS_Inst->PS_Burst->StartPosX;
	ParticlePosition.y = pPS_Inst->PS_Burst->StartPosY;
	//launch particles in a random direction at a constant speed
	InitialVelocity.x = (float)((0.1 + RandomScale * 0.2) * cos(ImpactAngle + (RandomAngle - PI)/8));
	InitialVelocity.y = (float)((0.1 + RandomScale * 0.2) * sin(ImpactAngle + (RandomAngle - PI)/8));

	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("WhiteBox.png"), GCreateMesh(10, 10, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	ParticleSprite->specialFX = Particle_Special_FX_HitSplash;
	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, entity_particle);
	NewParticle->simulate = Particle_Simulate_HitSplash;

	NewParticle->miscData = malloc(sizeof(ParticleData));
	((ParticleData *)(NewParticle->miscData))->ParticleID = i;
	for (j = 0; j < sizeof(pHitSplash) / sizeof(*pHitSplash); j++)
	{
		if (pPS_Inst == pHitSplash[j])
		{
			((ParticleData *)(NewParticle->miscData))->PS_ID = j;
			break;
		}
	}

	ParticleSprite->owner = NewParticle;
	pPS_Inst->PS_Burst->Particle[i] = NewParticle;
}

/*!
\brief
particle update behavior for Hit Splash
*/
void Particle_Simulate_HitSplash(void)
{

}

/*!
\brief
particle special visual effect for Hit Splash
*/
void Particle_Special_FX_HitSplash(Sprite *Owner)
{
	if (((ParticleData *)(Owner->owner->miscData))->ParticleID < 5)
	{
		if (pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 0.25f)
		{
			AEGfxSetTintColor(0.0f, 0.7f, 0.1f, 1.0f);
		}
		else
		{
			AEGfxSetTintColor(0.0f, 0.7f, 0.1f, 0.2f + 0.8f * pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
		}
	}
	else
	{
		if (pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 0.25f)
		{
			AEGfxSetTintColor(0.8f, 0.2f, 0.0f, 1.0f);
		}
		else
		{
			AEGfxSetTintColor(0.8f, 0.2f, 0.0f, 0.2f + 0.8f * pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pHitSplash[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
		}
	}
}

/*!
\brief
creates a particle for a Dodge Smoke 2
\param i
int for the ID of the particle.
\param pPS_Inst
PS_Instance * for the owner continuous PS.
*/
void Particle_Create_DodgeSmoke2(int i, PS_Instance *pPS_Inst)
{
	int j;

	//you need to create and initialize these to make the particles work properly
	float RandomAngle = RANDOM_ANGLE;
	float RandomAngle2 = RANDOM_ANGLE;
	float RandomScale = (float)((rand() / (float)RAND_MAX));
	float RandomScale2 = (float)((rand() / (float)RAND_MAX));
	float RandomScale3 = (float)((rand() / (float)RAND_MAX));
	Vector2D ParticlePosition;
	Vector2D InitialVelocity;
	PhysicsObject *ParticlePhysics;
	Animation *ParticleAnimation;
	Sprite *ParticleSprite;
	GameObject *NewParticle;

	//spawn particles at the position of the PS.
	ParticlePosition.x = pPS_Inst->PS_Burst->StartPosX + (float)(RandomScale * 0.5 * cos(RandomAngle));
	ParticlePosition.y = pPS_Inst->PS_Burst->StartPosY + (float)(RandomScale * 0.5 * sin(RandomAngle));
	//launch particles in a random direction at a constant speed
	InitialVelocity.x = (float)(RandomScale2 * 0.01 * cos(RandomAngle2));
	InitialVelocity.y = (float)(RandomScale2 * 0.01 * sin(RandomAngle2));

	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("WhiteBox.png"), GCreateMesh(20, 20, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke2;
	ParticleSprite->offset.y = 100.0f * RandomScale3;

	/*
	int j;
	for (j = 0; j < 4; j++)
	{
	if (pDodgeSmoke[j] == pPS_Inst)
	{
	switch (j)
	{
	case 0:
	ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke0;
	break;
	case 1:
	ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke1;
	break;
	case 2:
	ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke2;
	break;
	case 3:
	ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke3;
	break;
	}
	}
	}
	*/

	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, entity_particle);
	NewParticle->simulate = Particle_Simulate_DodgeSmoke2;
	NewParticle->miscData = malloc(sizeof(ParticleData));
	((ParticleData *)(NewParticle->miscData))->ParticleID = i;
	for (j = 0; j < sizeof(pDodgeSmoke2) / sizeof(*pDodgeSmoke2); j++)
	{
		if (pPS_Inst == pDodgeSmoke2[j])
		{
			((ParticleData *)(NewParticle->miscData))->PS_ID = j;
			break;
		}
	}
	ParticleSprite->owner = NewParticle;
	pPS_Inst->PS_Burst->Particle[i] = NewParticle;
}

/*!
\brief
particle update behavior for Dodge Smoke
*/
void Particle_Simulate_DodgeSmoke2(void)
{

}

/*!
\brief
particle special visual effect for Dodge Smoke
/*/
void Particle_Special_FX_DodgeSmoke2(Sprite *Owner)
{
	if (pDodgeSmoke2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 1.0f)
	{
		AEGfxSetTintColor(0.1f, 0.0f, 0.2f, 0.6f);
	}
	else
	{
		AEGfxSetTintColor(0.1f, 0.0f, 0.2f, 0.1f + pDodgeSmoke2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pDodgeSmoke2[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
	}
}

/*!
\brief
creates a particle for a Dodge Smoke
\param i
int for the ID of the particle.
\param pPS_Inst
PS_Instance * for the owner continuous PS.
*/
void Particle_Create_DodgeSmoke(int i, PS_Instance *pPS_Inst)
{
	int j;

	//you need to create and initialize these to make the particles work properly
	float RandomAngle = RANDOM_ANGLE;
	float RandomAngle2 = RANDOM_ANGLE;
	float RandomScale = (float)((rand() / (float)RAND_MAX));
	float RandomScale2 = (float)((rand() / (float)RAND_MAX));
	Vector2D ParticlePosition;
	Vector2D InitialVelocity;
	PhysicsObject *ParticlePhysics;
	Animation *ParticleAnimation;
	Sprite *ParticleSprite;
	GameObject *NewParticle;

	//spawn particles at the position of the PS.
	ParticlePosition.x = pPS_Inst->PS_Burst->StartPosX + (float)(RandomScale * 0.5 * cos(RandomAngle));
	ParticlePosition.y = pPS_Inst->PS_Burst->StartPosY + (float)(RandomScale * 0.5 * sin(RandomAngle));
	//launch particles in a random direction at a constant speed
	InitialVelocity.x = (float)(RandomScale2 * 0.01 * cos(RandomAngle2));
	InitialVelocity.y = (float)(RandomScale2 * 0.01 * sin(RandomAngle2));
	
	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("isotile.png"), GCreateMesh(20, 20, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke;

	/*
	int j;
	for (j = 0; j < 4; j++)
	{
		if (pDodgeSmoke[j] == pPS_Inst)
		{
			switch (j)
			{
			case 0:
				ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke0;
				break;
			case 1:
				ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke1;
				break;
			case 2:
				ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke2;
				break;
			case 3:
				ParticleSprite->specialFX = Particle_Special_FX_DodgeSmoke3;
				break;
			}
		}
	}
	*/

	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, entity_particle);
	NewParticle->simulate = Particle_Simulate_DodgeSmoke;
	NewParticle->miscData = malloc(sizeof(ParticleData));
	((ParticleData *)(NewParticle->miscData))->ParticleID = i;
	for (j = 0; j < sizeof(pDodgeSmoke) / sizeof(*pDodgeSmoke); j++)
	{
		if (pPS_Inst == pDodgeSmoke[j])
		{
			((ParticleData *)(NewParticle->miscData))->PS_ID = j;
			break;
		}
	}
	ParticleSprite->owner = NewParticle;
	pPS_Inst->PS_Burst->Particle[i] = NewParticle;
}

/*!
\brief
particle update behavior for Dodge Smoke
*/
void Particle_Simulate_DodgeSmoke(void)
{

}

/*!
\brief
particle special visual effect for Dodge Smoke
/*/
void Particle_Special_FX_DodgeSmoke(Sprite *Owner)
{
	if (pDodgeSmoke[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer > 1.0f)
	{
		AEGfxSetTintColor(0.9f, 0.9f, 0.9f, 0.5f);
	}
	else
	{
		AEGfxSetTintColor(0.9f, 0.9f, 0.9f, 0.3f + 0.4f * pDodgeSmoke[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->DeathTimer / pDodgeSmoke[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Burst->LifeTime);
	}
}

///*!
//\brief
//particle special visual effect for Dodge Smoke
//*/
//void Particle_Special_FX_DodgeSmoke0(void)
//{
//	if (pDodgeSmoke[0]->PS_Burst->DeathTimer > 1.0f)
//	{
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.5f);
//	}
//	else
//	{
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f * pDodgeSmoke[0]->PS_Burst->DeathTimer / pDodgeSmoke[0]->PS_Burst->LifeTime);
//	}
//}
//
///*!
//\brief
//particle special visual effect for Dodge Smoke
//*/
//void Particle_Special_FX_DodgeSmoke1(void)
//{
//	if (pDodgeSmoke[1]->PS_Burst->DeathTimer > 1.0f)
//	{
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.5f);
//	}
//	else
//	{
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f * pDodgeSmoke[1]->PS_Burst->DeathTimer / pDodgeSmoke[1]->PS_Burst->LifeTime);
//	}
//}
//
///*!
//\brief
//particle special visual effect for Dodge Smoke
//*/
//void Particle_Special_FX_DodgeSmoke2(void)
//{
//	if (pDodgeSmoke[2]->PS_Burst->DeathTimer > 1.0f)
//	{
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.5f);
//	}
//	else
//	{
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f * pDodgeSmoke[2]->PS_Burst->DeathTimer / pDodgeSmoke[2]->PS_Burst->LifeTime);
//	}
//}
//
///*!
//\brief
//particle special visual effect for Dodge Smoke
//*/
//void Particle_Special_FX_DodgeSmoke3(void)
//{
//	if (pDodgeSmoke[3]->PS_Burst->DeathTimer > 1.0f)
//	{
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 0.5f);
//	}
//	else
//	{
//		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f * pDodgeSmoke[3]->PS_Burst->DeathTimer / pDodgeSmoke[3]->PS_Burst->LifeTime);
//	}
//}

/*!
\brief
creates a particle for a Fire Hazard
\param i
int for the ID of the particle.
\param pPS_Inst
PS_Instance * for the owner continuous PS.
*/
void Particle_Create_FireHazard(int i, PS_Instance *pPS_Inst)
{
	int j;

	//you need to create and initialize these to make the particles work properly
	float RandomAngleUh = RANDOM_ANGLE;
	float RandomScale = (float)((rand() / (float)RAND_MAX));
	float RandomScale2 = (float)((rand() / (float)RAND_MAX));
	Vector2D ParticlePosition;
	Vector2D InitialVelocity;
	PhysicsObject *ParticlePhysics;
	Animation *ParticleAnimation;
	Sprite *ParticleSprite;
	GameObject *NewParticle;

	//spawn particles at the position of the PS.
	ParticlePosition.x = pPS_Inst->PS_Continuous->StartPosX + (float)(RandomScale * 0.5f);
	ParticlePosition.y = pPS_Inst->PS_Continuous->StartPosY + (float)((1.0f - RandomScale) * 0.5f);
	//launch particles in a random direction at a constant speed
	InitialVelocity.x = (float)(RandomScale2 * 0.01 * cos(RandomAngleUh));
	InitialVelocity.y = (float)(RandomScale2 * 0.01 * sin(RandomAngleUh));

	//NECESSARY SET-UP STUFF
	ParticlePhysics = PhysicsCreateObject(ParticlePosition, 1.0f);
	ParticlePhysics->velocity = InitialVelocity;
	ParticlePhysics->active = 0;
	ParticleAnimation = GCreateAnimation(1, GCreateTexture("WhiteBox.png"), GCreateMesh(20, 20, 1, 1), 1);
	ParticleSprite = GCreateSprite(ParticlePosition.x, ParticlePosition.y, ParticleAnimation, 0.0f);
	ParticleSprite->specialFX = Particle_Special_FX_FireHazard;

	/*
	int j;

	for (j = 0; j < 2; j++)
	{
		if (pFireHazard[j] == pPS_Inst)
		{
			switch (j)
			{
			case 0:
				switch (i)
				{
				case 0:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_0;
					break;
				case 1:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_1;
					break;
				case 2:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_2;
					break;
				case 3:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_3;
					break;
				case 4:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_4;
					break;
				case 5:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_5;
					break;
				case 6:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_6;
					break;
				case 7:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_7;
					break;
				case 8:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_8;
					break;
				case 9:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard0_9;
					break;
				}
				break;
			case 1:
				switch (i)
				{
				case 0:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_0;
					break;
				case 1:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_1;
					break;
				case 2:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_2;
					break;
				case 3:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_3;
					break;
				case 4:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_4;
					break;
				case 5:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_5;
					break;
				case 6:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_6;
					break;
				case 7:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_7;
					break;
				case 8:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_8;
					break;
				case 9:
					ParticleSprite->specialFX = Particle_Special_FX_FireHazard1_9;
					break;
				}
				break;
			}
		}
	}
	*/

	ParticleSprite->blendMode = AE_GFX_BM_BLEND;
	NewParticle = GameObjectCreate(ParticlePhysics, ParticleSprite, NULL, entity_particle);
	NewParticle->simulate = Particle_Simulate_FireHazard;
	NewParticle->miscData = malloc(sizeof(ParticleData));
	((ParticleData *)(NewParticle->miscData))->ParticleID = i;
	for (j = 0; j < sizeof(pFireHazard) / sizeof(*pFireHazard); j++)
	{
		if (pPS_Inst == pFireHazard[j])
		{
			((ParticleData *)(NewParticle->miscData))->PS_ID = j;
			break;
		}
	}
	ParticleSprite->owner = NewParticle;

	pPS_Inst->PS_Continuous->Particle[i] = NewParticle;
}

/*!
\brief
particle update behavior for Fire Hazard
*/
void Particle_Simulate_FireHazard(void)
{

}

/*!
\brief
particle special visual effect for Fire Hazard
*/
void Particle_Special_FX_FireHazard(Sprite *Owner)
{
	if (pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->Particle[((ParticleData *)(Owner->owner->miscData))->ParticleID])
	{
		pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->Particle[((ParticleData *)(Owner->owner->miscData))->ParticleID]->sprite->offset.y += 
			1.0f + (1.0f - pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->RefreshTimer[((ParticleData *)(Owner->owner->miscData))->ParticleID] / 
			pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->RefreshTimer[((ParticleData *)(Owner->owner->miscData))->ParticleID] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->RefreshTimer[((ParticleData *)(Owner->owner->miscData))->ParticleID] / 
			pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->LifeTime, 0.5f * 
			pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->RefreshTimer[((ParticleData *)(Owner->owner->miscData))->ParticleID] / 
			pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->RefreshTimer[((ParticleData *)(Owner->owner->miscData))->ParticleID] / 
			pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->LifeTime, 0.5f * 
			pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->RefreshTimer[((ParticleData *)(Owner->owner->miscData))->ParticleID] / 
			pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->RefreshTimer[((ParticleData *)(Owner->owner->miscData))->ParticleID] / 
			pFireHazard[((ParticleData *)(Owner->owner->miscData))->PS_ID]->PS_Continuous->LifeTime);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
\brief
particle special visual effect for Fire Hazard
*/
void Particle_Special_FX_FireHazard0_0(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[0])
	{
		pFireHazard[0]->PS_Continuous->Particle[0]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[0] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[0] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[0] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[0] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[0] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[0] / pFireHazard[0]->PS_Continuous->LifeTime, 
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[0] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_0(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[0])
	{
		pFireHazard[1]->PS_Continuous->Particle[0]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[0] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[0] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[0] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[0] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[0] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[0] / pFireHazard[1]->PS_Continuous->LifeTime, 
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[0] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_1(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[1])
	{
		pFireHazard[0]->PS_Continuous->Particle[1]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[1] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[1] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[1] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[1] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[1] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[1] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[1] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_1(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[1])
	{
		pFireHazard[1]->PS_Continuous->Particle[1]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[1] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[0] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[1] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[1] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[1] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[1] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[1] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_2(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[2])
	{
		pFireHazard[0]->PS_Continuous->Particle[2]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[2] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[2] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[2] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[2] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[2] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[2] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[2] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_2(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[2])
	{
		pFireHazard[1]->PS_Continuous->Particle[2]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[2] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[2] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[2] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[2] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[2] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[2] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[2] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_3(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[3])
	{
		pFireHazard[0]->PS_Continuous->Particle[3]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[3] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[3] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[3] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[3] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[3] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[3] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[3] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_3(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[3])
	{
		pFireHazard[1]->PS_Continuous->Particle[3]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[3] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[3] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[3] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[3] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[3] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[3] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[3] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_4(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[4])
	{
		pFireHazard[0]->PS_Continuous->Particle[4]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[4] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[4] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[4] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[4] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[4] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[4] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[4] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_4(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[4])
	{
		pFireHazard[1]->PS_Continuous->Particle[4]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[4] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[4] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[4] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[4] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[4] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[4] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[4] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_5(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[5])
	{
		pFireHazard[0]->PS_Continuous->Particle[5]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[5] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[5] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[5] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[5] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[5] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[5] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[5] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_5(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[5])
	{
		pFireHazard[1]->PS_Continuous->Particle[5]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[5] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[5] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[5] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[5] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[5] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[5] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[5] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_6(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[6])
	{
		pFireHazard[0]->PS_Continuous->Particle[6]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[6] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[6] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[6] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[6] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[6] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[6] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[6] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_6(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[6])
	{
		pFireHazard[1]->PS_Continuous->Particle[6]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[6] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[6] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[6] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[6] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[6] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[6] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[6] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_7(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[7])
	{
		pFireHazard[0]->PS_Continuous->Particle[7]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[7] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[7] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[7] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[7] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[7] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[7] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[7] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_7(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[7])
	{
		pFireHazard[1]->PS_Continuous->Particle[7]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[7] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[7] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[7] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[7] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[7] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[7] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[7] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_8(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[8])
	{
		pFireHazard[0]->PS_Continuous->Particle[8]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[8] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[8] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[8] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[8] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[8] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[8] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[8] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_8(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[8])
	{
		pFireHazard[1]->PS_Continuous->Particle[8]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[8] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[8] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[8] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[8] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[8] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[8] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[8] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard0_9(void)
{
	if (pFireHazard[0]->PS_Continuous->Particle[9])
	{
		pFireHazard[0]->PS_Continuous->Particle[9]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[0]->PS_Continuous->RefreshTimer[9] / pFireHazard[0]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[0]->PS_Continuous->RefreshTimer[9] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[9] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[9] / pFireHazard[0]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[9] / pFireHazard[0]->PS_Continuous->LifeTime, 0.5f * pFireHazard[0]->PS_Continuous->RefreshTimer[9] / pFireHazard[0]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[0]->PS_Continuous->RefreshTimer[9] / pFireHazard[0]->PS_Continuous->LifeTime);
	}
}

/*!
\brief
particle special visual effect for Fire Hazard Copy
*/
void Particle_Special_FX_FireHazard1_9(void)
{
	if (pFireHazard[1]->PS_Continuous->Particle[9])
	{
		pFireHazard[1]->PS_Continuous->Particle[9]->sprite->offset.y += 1.0f + (1.0f - pFireHazard[1]->PS_Continuous->RefreshTimer[9] / pFireHazard[1]->PS_Continuous->LifeTime);
	}

	if (pFireHazard[1]->PS_Continuous->RefreshTimer[9] > 0.1f)
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[9] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[9] / pFireHazard[1]->PS_Continuous->LifeTime, 1.0f);
	}
	else
	{
		AEGfxSetTintColor(1.0f, 1.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[9] / pFireHazard[1]->PS_Continuous->LifeTime, 0.5f * pFireHazard[1]->PS_Continuous->RefreshTimer[9] / pFireHazard[1]->PS_Continuous->LifeTime,
			5.0f * pFireHazard[1]->PS_Continuous->RefreshTimer[9] / pFireHazard[1]->PS_Continuous->LifeTime);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




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
void Default_Particle_Special_FX_Burst(Sprite *Owner)
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
				if ((EmitCount > 0) && (EmitCount < (int)(EmitRate * LifeTime * 1.0001f)))
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
