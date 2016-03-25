/*!
\file   ParticleSystems(Redo).h
\author Jack Ile
\par    email: jack.i\@digipen.edu
\brief
Prototypes for Particle Systems
*/
#pragma once
#ifndef PS_H
#define PS_H

//number of PSes to use.  THIS NUMBER MUST BE EQUAL TO THE TOTAL AMOUNT OF PS ASSETS YOU ARE CREATING
#define PS_OBJ_INST_MAX 8

typedef struct PS_Instance PS_Instance;
typedef struct GameObject GameObject;

//state-perserved, continuous PS
typedef struct PS_Continuous
{
	//Dynamically allocated arrays based on number of particles
	//Particle data is recycled.
	GameObject **Particle;  //pointer storing the ID of the particles

	float *RefreshTimer;	//life time of each particle before being refreshed
							//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().

	//Single variables as properties for all particles
	float LifeTime;		//standard life time of each particle in seconds.  Particle count will be based on this and EmitRate.
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;

	//Function pointers for behavior of the particle system
	void(*vpParticle_Create)(int, PS_Instance *);				//used for setting up particle's properties on refresh
	void(*vpParticle_System_Update)(PS_Instance *, float);		//used for updating the particle system itself
	
	//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	float EmitRate;		//rate of emittion of particles per second.  Particle count will be based on this and LifeTime.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Set to -1 for no limit nor self destruction.
	int EmitCounter;	//counter for EmitCount to function
	float EmitTimer;	//timer for determining next particle instatiation.  Starting value = 1 / EmitRate.  Reduced by frameTime per frame.  Increased by 1/EmitRate everytime a particle is instantiated.

}PS_Continuous;


//state-preserved, burst PS
typedef struct PS_Burst
{
	//Dynamically allocated arrays based on number of particles
	//Particles are instantiated all at once.
	GameObject **Particle;  //pointer storing the ID of the particles

	//Single variables as properties for all particles
	float LifeTime;		//standard life time of each particle in seconds.
	float DeathTimer;	//life time of each particle before being destroyed.
						//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;

	//Function pointers for behavior of the particle system
	void(*vpParticle_Create)(int, PS_Instance *);			//used for setting up particle's properties on refresh
																
	void(*vpParticle_System_Update)(PS_Instance *, float);	//used for updating the particle system itself

	//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Setting to 0 or less will not create any particles, and PS will immediately destroy itself.
						//Particle count based on this value.
}PS_Burst;

//PS Instance struct
typedef struct PS_Instance
{
	int					ExistFlag;
	PS_Continuous		*PS_Continuous;
	PS_Burst			*PS_Burst;

}PS_Instance;

//PS object instance list
PS_Instance		goiaPS_ObjectInstanceList[PS_OBJ_INST_MAX];

//PS spawn functions
void SpawnDodgeSmokePS(float StartPosX, float StartPosY);

//default PS behavior functions
void Default_Particle_Create_Continuous(int i, PS_Instance *pPS_Inst);
void Default_Particle_Create_Burst(int i, PS_Instance *pPS_Inst);
void Default_Particle_System_Update_Continuous(PS_Instance *pPS_Inst, float frameTime);
void Default_Particle_System_Update_Burst(PS_Instance *pPS_Inst, float frameTime);
void Default_Particle_Special_FX_Burst(void);
void Default_Particle_Simulate(void);
//default PS behavior functions

//other PS behavior functions
void Particle_Create_HitSplash(int i, PS_Instance *pPS_Inst);
void Particle_Simulate_HitSplash(void);
void Particle_Special_FX_HitSplash(void);

void Particle_Create_DodgeSmoke(int i, PS_Instance *pPS_Inst);
void Particle_Simulate_DodgeSmoke(void);
void Particle_Special_FX_DodgeSmoke0(void);
void Particle_Special_FX_DodgeSmoke1(void);
void Particle_Special_FX_DodgeSmoke2(void);
void Particle_Special_FX_DodgeSmoke3(void);

void Particle_Create_FireHazard(int i, PS_Instance *pPS_Inst);
void Particle_Simulate_FireHazard(void);
void Particle_Special_FX_FireHazard0_0(void);
void Particle_Special_FX_FireHazard1_0(void);
void Particle_Special_FX_FireHazard0_1(void);
void Particle_Special_FX_FireHazard1_1(void);
void Particle_Special_FX_FireHazard0_2(void);
void Particle_Special_FX_FireHazard1_2(void);
void Particle_Special_FX_FireHazard0_3(void);
void Particle_Special_FX_FireHazard1_3(void);
void Particle_Special_FX_FireHazard0_4(void);
void Particle_Special_FX_FireHazard1_4(void);
void Particle_Special_FX_FireHazard0_5(void);
void Particle_Special_FX_FireHazard1_5(void);
void Particle_Special_FX_FireHazard0_6(void);
void Particle_Special_FX_FireHazard1_6(void);
void Particle_Special_FX_FireHazard0_7(void);
void Particle_Special_FX_FireHazard1_7(void);
void Particle_Special_FX_FireHazard0_8(void);
void Particle_Special_FX_FireHazard1_8(void);
void Particle_Special_FX_FireHazard0_9(void);
void Particle_Special_FX_FireHazard1_9(void);


void LoadAll_PS(void);	//call in level load function in level manager

//PS creation functions.  Please create S (state-preserved) PSs in level load function for efficiency.
PS_Instance * Create_PS_Continuous(float LifeTime, float EmitRate, int EmitCount);
PS_Instance * Create_PS_Burst(float LifeTime, int EmitCount);


void Start_PS(PS_Instance *pPS_Inst);	//start up pre-existing PS
void Stop_PS(PS_Instance *pPS_Inst);	//stop functionality, but don't destroy a PS
void Destroy_PS(PS_Instance *pPS_Inst);	//destroys a PS.  PLEASE DON'T USE IT AS A SUBSTITUTE FOR UnloadAllPS_Inst FUNCTION
void UpdateAllPS_Inst(float frameTime);	//updates all PS instance particles

//call this FIRST in level free function in the level manager before game object instance destruction
void UnloadAll_PS(void);			//frees all memory occupied by PSs.

//test particle effect istances
PS_Instance *pPS_C;
PS_Instance *pPS_B;

//regular particle effect instances
PS_Instance *pDodgeSmoke[4];
PS_Instance *pFireHazard[2];

#endif