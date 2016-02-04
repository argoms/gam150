#ifndef PS_H
#define PS_H

#define PS_OBJ_INST_MAX 128

typedef struct PS_Instance PS_Instance;


//state-perserved continuous PS
typedef struct PS_Continuous_S
{
	//Dynamically allocated arrays based on number of particles
	//Particle data is recycled.
	float *PosX;		//position of particles
	float *PosY;
	float *PosZ;

	float *VelX;		//velocity of particles
	float *VelY;
	float *VelZ;

	float *Ang;			//angle of particles
	float *AngVel;		//angular velocity of particles

	float *ScaleX;		//scale of particles
	float *ScaleY;		//only use ScaleX if scales are uniform
	float *ScaleVelX;	//scale velocity of particles
	float *ScaleVelY;	//only use ScaleVelX if scales are uniform

	float *RefreshTimer;	//life time of each particle before being refreshed
							//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().

	//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float LifeTime;		//standard life time of each particle in seconds.  Particle count will be based on this and EmitRate.
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;
	float StartPosZ;

	//Function pointers for behavior of the particle system
	void(*vpParticle_Instantiation)(int, PS_Instance *);//used for setting up particle's properties on refresh
	void(*vpParticle_Update)(int, PS_Instance *, float);//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpDrawSettings)(void);						//draw settings for all of the particles to add some possible boosts to efficiency.
	void(*vpParticle_Draw)(int, PS_Instance *);			//used for drawing a particle.  all particles are layered the same.

	//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	float EmitRate;		//rate of emittion of particles per second.  Particle count will be based on this and LifeTime.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Set to -1 for no limit nor self destruction.
	float EmitTimer;	//timer for determining next particle instatiation.  Starting value = 1 / EmitRate.  Reduced by frameTime per frame.  Increased by 1/EmitRate everytime a particle is instantiated.

}PS_Continuous_S;


//state-preserved burst PS
typedef struct PS_Burst_S
{
	//Dynamically allocated arrays based on number of particles
	//Particles are instantiated all at once.
	float *PosX;		//position of particles
	float *PosY;
	float *PosZ;

	float *VelX;		//velocity of particles
	float *VelY;
	float *VelZ;

	float *Ang;			//angle of particles
	float *AngVel;		//angular velocity of particles

	float *ScaleX;		//scale of particles
	float *ScaleY;		//only use ScaleX if scales are uniform
	float *ScaleVelX;	//scale velocity of particles
	float *ScaleVelY;	//only use ScaleVelX if scales are uniform

	//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float LifeTime;		//standard life time of each particle in seconds.
	float DeathTimer;	//life time of each particle before being destroyed.
						//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;
	float StartPosZ;

	//Function pointers for behavior of the particle system
	void(*vpParticle_Instantiation)(int, PS_Instance *);//used for setting up particle's properties on refresh
	void(*vpParticle_Update)(int, PS_Instance *, float);//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpDrawSettings)(void);						//draw settings for all of the particles to add some possible boosts to efficiency.
	void(*vpParticle_Draw)(int, PS_Instance *);			//used for drawing a particle.  all particles are layered the same.

	//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Setting to 0 or less will not create any particles, and PS will immediately destroy itself.
						//Particle count based on this value.
}PS_Burst_S;


//stateless continuous PS
typedef struct PS_Continuous_NS
{
	//Particle data is calculated algorithmatically, and not stored

	//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float CycleTimer;	//timer for particle behavior to cycle
						//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().
	float LifeTime;		//life time of each particle before being destroyed.
	int EmitCount;		//total number of particles in the PS.
						//can be used to proportionate properties between particles.
						//set a hard cap for how many total particles to instantiate before self-destruction.  Setting to 0 or less will not create any particles, and PS will immediately destroy itself.
						//set to -1 to have unlimited particles

	//Function pointers for behavior of the particle system
	//instantiation is not necessary, since particles' properties are all written out on a timeline
	void(*vpParticle_Update)(int, PS_Instance *, float);//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpDrawSettings)(void);						//draw settings for all of the particles to add some possible boosts to efficiency.
	void(*vpParticle_Draw)(int, PS_Instance *);			//used for drawing a particle.  all particles are layered the same.

											//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
}PS_Continuous_NS;


//stateless burst PS
typedef struct PS_Burst_NS
{
	//Particle data is calculated algorithmatically, and not stored

	//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float DeathTimer;	//timer for particle destruction.
						//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().
	float LifeTime;		//life time of each particle before being destroyed.
	int EmitCount;		//total number of particles in the PS.
						//can be used to proportionate properties between particles.
						//set a hard cap for how many total particles to instantiate before self-destruction.  Setting to 0 or less will not create any particles, and PS will immediately destroy itself.
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;
	float StartPosZ;

	//Function pointers for behavior of the particle system
	//instantiation is not necessary, since particles' properties are all written out on a timeline
	void(*vpParticle_Update)(int, PS_Instance *, float);//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpDrawSettings)(void);						//draw settings for all of the particles to add some possible boosts to efficiency.
	void(*vpParticle_Draw)(int, PS_Instance *);			//used for drawing a particle.  all particles are layered the same.
											//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
}PS_Burst_NS;

typedef struct PS_Instance
{
	int					ExistFlag;
	PS_Continuous_S		*PS_Continuous_S;
	PS_Burst_S			*PS_Burst_S;
	//PS_Continuous_NS	*PS_Continuous_NS;
	PS_Burst_NS			*PS_Burst_NS;
};

//default PS behavior functions
void DeafaultInstantiation_Continuous_S(int i, PS_Instance *pPS_Inst);
void DeafaultInstantiation_Burst_S(int i, PS_Instance *pPS_Inst);
void DefaultUpdate_Continuous_S(int i, PS_Instance *pPS_Inst, float frameTime);
void DefaultUpdate_Burst_S(int i, PS_Instance *pPS_Inst, float frameTime);
void DefaultDrawSettings(void);
void DefaultDraw_Continuous_S(int i, PS_Instance *pPS_Inst);
void DefaultDraw_Burst_S(int i, PS_Instance *pPS_Inst);
//default PS behavior functions

void Load_PS_Pool(int iPS_Continuous_S, int iPS_Burst_S, int iPS_Continuous_NS, int iPS_Burst_NS);	//call in level load function

//PS creation functions.  Please create S (state-preserved) PSs in level load function for efficiency.  NS (stateless) PSscan be created anywhere
PS_Instance * Create_PS_Continuous_S(float LifeTime, float EmitRate, int EmitCount);
PS_Instance * Create_PS_Burst_S(float LifeTime, int EmitCount);
PS_Instance * Create_PS_Burst_NS(float LifeTime, int EmitCount);

void Start_PS(PS_Instance *pPS_Inst);	//start up pre-existing PS
void Stop_PS(PS_Instance *pPS_Inst);	//stop functionality, but don't destroy a PS
void Destroy_PS(PS_Instance *pPS_Inst);	//destroys a PS.  PLEASE DON'T USE IT AS A SUBSTITUTE FOR UnloadAllPS_Inst FUNCTION
void UpdateAllPS_Inst(float frameTime);	//updates all PS instance particles
void DrawAllPS_Inst(void);				//draws all PS instance particles if allowed
void UnloadAllPS_Inst(void);			//frees all memory occupied by PSs.

/*
enum PS_TYPE
{
	CONTINUOUS_S = 1,
	BURST_S = 2,
	CONTINUOUS_NS = 3,
	BURST_S = 4
};
*/


/*
enum PS_TYPE
{
	CONTINUOUS_2D = 1,
	BURST_2D = 2,
	CUSTOM_2D = 3,
	CONTINUOUS_3D = 4,
	BURST_3D = 5,
	CUSTOM_3D = 6
};

//CONTINUOUS PSs and BURST PSs PRODUCE BASIC PARTICLE EFFECTS.  IF YOU WISH FOR A COMPLEX ONE, USE MULTIPLE DIFFERENT BASIC PSs, OR USE A CUSTOM PS(BUT SPAWN PARTICLE INSTANCES SPARINGLY, AS A CUSTOM PS IS LESS EFFICIENT).

//closed, state-preserved, 2D PS
typedef struct Continuous2D_PS
{
	//Dynamically allocated arrays based on number of particles
	//Particle data is recycled.
	float *PosX;		//position of particles
	float *PosY;
	float *VelX;		//velocity of particles
	float *VelY;
	float *RefreshTimer;	//life time of each particle before being refreshed
							//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().

							//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float LifeTime;		//standard life time of each particle in seconds.  Particle count will be based on this and EmitRate.
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;

	//Function pointers for behavior of the particle system
	void(*vpParticle_Instantiation)(int);	//used for setting up particle's properties on refresh
	void(*vpParticle_Update)(int);			//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpDrawSettings)(void);			//draw settings for all of the particles to add some possible boosts to efficiency.
	void(*vpParticle_Draw)(int);			//used for drawing a particle.  all particles are layered the same.

											//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	float EmitRate;		//rate of emittion of particles per second.  Particle count will be based on this and LifeTime.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Set to -1 for no limit nor self destruction.
	float EmitTimer;	//timer for determining next particle instatiation.  Starting value = 1 / EmitRate.  Reduced by frameTime per frame.  Increased by 1/EmitRate everytime a particle is instantiated.

}Continuous2D_PS;



//closed, stateless, 2D PS
typedef struct SL_Burst2D_PS
{
	//Particle data is calculated algorithmatically, and not stored

	//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float DeathTimer;	//timer for particle destruction.
						//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().
	float LifeTime;		//life time of each particle before being destroyed.
	int EmitCount;		//total number of particles in the PS.
						//can be used to proportionate properties between particles.
						//set a hard cap for how many total particles to instantiate before self-destruction.  Setting to 0 or less will not create any particles, and PS will immediately destroy itself.

						//Function pointers for behavior of the particle system
						//instantiation is not necessary, since particles' properties are all written out on a timeline
	void(*vpParticle_Update)(int);			//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpDrawSettings)(void);			//draw settings for all of the particles to add some possible boosts to efficiency.
	void(*vpParticle_Draw)(int);			//used for drawing a particle.  all particles are layered the same.

											//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
}SL_Burst2D_PS;


//closed, state-preserved, 2D PS
typedef struct Burst2D_PS
{
	//Dynamically allocated arrays based on number of particles
	//Particles are instantiated all at once.
	float *PosX;		//position of particles
	float *PosY;
	float *VelX;		//velocity of particles
	float *VelY;

	//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float DeathTimer;	//life time of each particle before being destroyed.
						//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;

	//Function pointers for behavior of the particle system
	void(*vpParticle_Instantiation)(int);	//used for setting up all of the particles' properties on PS creation.
	void(*vpParticle_Update)(int);			//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpDrawSettings)(void);			//draw settings for all of the particles to add some possible boosts to efficiency.
	void(*vpParticle_Draw)(int);			//used for drawing a particle.  all particles are layered the same.

											//Single variables as properties of the PS
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Setting to 0 or less will not create any particles, and PS will immediately destroy itself.
						//Particle count based on this value.
}Burst2D_PS;




/*WIP///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*
//open, custom-state-preserved, 2D PS
typedef struct Custom2D_PS
{
	float *PosX;	//Dynamically allocated arrays based on number of particles
	float *PosY;	//Dynamically allocated arrays based on number of particles
	float *VelX;	//Dynamically allocated arrays based on number of particles
	float *VelY;	//Dynamically allocated arrays based on number of particles
	float *AccelX;	//Dynamically allocated arrays based on number of particles
	float *AccelY;	//Dynamically allocated arrays based on number of particles
	float *ScaleX;	//Dynamically allocated arrays based on number of particles
	float *ScaleY;	//Dynamically allocated arrays based on number of particles
	float *Angle;	//Dynamically allocated arrays based on number of particles
	float *LocalAlpha;	//Dynamically allocated arrays based on number of particles
	float *LifeTime;	//Dynamically allocated arrays based on number of particles

	AEGfxVertexList *pMesh;
	AEGfxTexture *pTexture;
	int RenderMode; //AE_GFX_RM_TEXTURE = 2 = use pTexture, AE_GFX_RM_COLOR = 1 = no/ignore pTexture
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;
	float GlobalRedTint;
	float GlobalGreenTint;
	float GlobalBlueTint;

	unsigned char	sName[64];

	//Function pointers for behavior of the particle system
	void(*vpParticle_Instantiation)(int);	//used for setting up particle's properties on refresh
	void(*vpParticle_Update)(int);			//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpDrawSettings)(void);			//draw settings for all of the particles to add some possible boosts to efficiency.
	void(*vpParticle_Draw)(int);			//used for drawing a particle.  all particles are layered the same.

											//Single variables as properties of the PS
	int EmitReady;		//boolean flag for instantiating particles. 1 = can instantiate, 0 = can't instantiate.
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	float EmitRate;		//rate of emittion of particles per second.  Particle count will be based on this and LifeTime.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Set to 0 for no limit nor self destruction.
	float EmitTimer;	//timer for determining next particle instatiation.  Starting value = 1 / EmitRate.  Reduced by frameTime per frame.  Increased by 1/EmitRate everytime a particle is instantiated.

}Custom2D_PS;
/*WIP///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/



/**
//closed, state-preserved, psuedo-3D PS
typedef struct Continuous3D_PS
{
	//Dynamically allocated arrays based on number of particles
	//Particle data is recycled.
	float *PosX;		//position of particles
	float *PosY;		//used for draw/rendering ordering for top-down view. lowest y = first, highest y = last.
						//Also affects scaling and parallaxing.  Influence of such features exist for 0 < PerspectiveAngle <= pi/2.  Increasing influence as PerspectiveAngle increases.
	float *PosZ;		//used for draw/rendering ordering for side-scroll view. lowest z = first, highest z = last.
						//Also affects scaling and parallaxing.  Influence of such features exist for 0 <= PerspectiveAngle < pi/2.  Decreasing influence as PerspectiveAngle increases.
	float *VelX;		//velocity of particles
	float *VelY;
	float *VelZ;
	float *RefreshTimer;	//life time of each particle before being refreshed
							//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().

							//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float LifeTime;		//standard life time of each particle in seconds.  Particle count will be based on this and EmitRate.
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;

	//Function pointers for behavior of the particle system
	void(*vpParticle_Instantiation)(int);	//used for setting up particle's properties on refresh
	void(*vpParticle_Update)(int);			//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpParticle_Draw)(int);			//used for efficiently drawing all of the particles.  particles are layered based on PosY, PosZ, and PerspectiveAngle.

											//Single variables as properties of the PS
	int EmitReady;		//boolean flag for instantiating particles. 1 = can instantiate, 0 = can't instantiate.
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	float EmitRate;		//rate of emittion of particles per second.  Particle count will be based on this and LifeTime.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Set to -1 for no limit nor self destruction.
	float EmitTimer;	//timer for determining next particle instatiation.  Starting value = 1 / EmitRate.  Reduced by frameTime per frame.  Increased by 1/EmitRate everytime a particle is instantiated.
	float PerspectiveAngle;	//viewing angle of patricle system, with anlge measured in radians. Min of 0 = side-scroll view, Max of pi/2 = top-down view.  Factors inbetween angles as well.
}Continuous3D_PS;


//closed, semi-state-preserved, psuedo-3D PS
typedef struct Burst3D_PS
{
	//Dynamically allocated arrays based on number of particles
	//Particles are instantiated all at once.
	float *PosX;		//position of particles
	float *PosY;		//used for draw/rendering ordering for top-down view. lowest y = first, highest y = last.
						//Also affects scaling and parallaxing.  Influence of such features exist for 0 < PerspectiveAngle <= pi/2.  Increasing influence as PerspectiveAngle increases.
	float *PosZ;		//used for draw/rendering ordering for side-scroll view. lowest z = first, highest z = last.
						//Also affects scaling and parallaxing.  Influence of such features exist for 0 <= PerspectiveAngle < pi/2.  Decreasing influence as PerspectiveAngle increases.
	float *VelX;		//velocity of particles
	float *VelY;
	float *VelZ;
	//float *DeathTimer;		//life time of each particle before being destroyed.
	//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().

	//Single variables as properties for all particles
	AEGfxVertexList *pMesh;		//mesh of each particle.  Color, alpha, scale, and angle are assumed to be covered
	AEGfxTexture *pTexture;		//texture of each particle.  Color, alpha, scale, and angle are assumed to be covered
	float DeathTimer;	//life time of each particle before being destroyed.
						//this variable can also be used to the change the particle's appearance, and path with velocity over time in *vpParticle_Draw().
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;

	//Function pointers for behavior of the particle system
	void(*vpParticle_Instantiation)(int);	//used for setting up all of the particles' properties on PS creation.
	void(*vpParticle_Update)(int);			//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpParticle_Draw)(int);			//used for efficiently drawing all of the particles.  particles are layered based on PosY, PosZ, and PerspectiveAngle.

											//Single variables as properties of the PS
	int ShutDown;			//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
							//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	int EmitCount;			//set a hard cap for how many total particles to instantiate before self-destruction.  Setting to 0  or less will not create any particles, and PS will immediately destroy itself.
							//Particle count based on this value.
	float PerspectiveAngle;	//viewing angle of patricle system, with anlge measured in radians. Min of 0 = side-scroll view, Max of pi/2 = top-down view.  Factors inbetween angles as well.
}Burst3D_PS;



/*WIP///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*
//open, custom-state-preserved, psuedo-3D PS
typedef struct Custom3D_PS
{
	float *PosX;	//Dynamically allocated arrays based on number of particles
	float *PosY;	//Dynamically allocated arrays based on number of particles
	float *VelX;	//Dynamically allocated arrays based on number of particles
	float *VelY;	//Dynamically allocated arrays based on number of particles
	float *AccelX;	//Dynamically allocated arrays based on number of particles
	float *AccelY;	//Dynamically allocated arrays based on number of particles
	float *ScaleX;	//Dynamically allocated arrays based on number of particles
	float *ScaleY;	//Dynamically allocated arrays based on number of particles
	float *Angle;	//Dynamically allocated arrays based on number of particles
	float *LocalAlpha;	//Dynamically allocated arrays based on number of particles
	float *LifeTime;	//Dynamically allocated arrays based on number of particles

	AEGfxVertexList *pMesh;
	AEGfxTexture *pTexture;
	int RenderMode; //AE_GFX_RM_TEXTURE = 2 = use pTexture, AE_GFX_RM_COLOR = 1 = no/ignore pTexture
	float StartPosX;	//where the PS_Instance was created, and an option for the particles to first spawn
	float StartPosY;
	float GlobalRedTint;
	float GlobalGreenTint;
	float GlobalBlueTint;

	unsigned char	sName[64];

	//Function pointers for behavior of the particle system
	void(*vpParticle_Instantiation)(int);	//used for setting up particle's properties on refresh
	void(*vpParticle_Update)(int);			//used for updating the particle properties and for calling particle instantiation when necessary.
	void(*vpParticle_Draw)(int);			//used for efficiently drawing all of the particles.  particles are layered based on PosY, PosZ, and PerspectiveAngle.

											//Single variables as properties of the PS
	int EmitReady;		//boolean flag for instantiating particles. 1 = can instantiate, 0 = can't instantiate.
	int ShutDown;		//boolean flag for triggering shutdown mode.  Shutdown mode necessary for safe destruction. 0 = no, 1 = yes.  Set to 1 when preset EmitCount is reached or PS Object is requested to be destroyed.
						//Shutdown mode exists for compensation of no particles existing in the general GameObjectInstanceList array.
	float EmitRate;		//rate of emittion of particles per second.  Particle count will be based on this and LifeTime.
	int EmitCount;		//set a hard cap for how many total particles to instantiate before self-destruction.  Set to 0 for no limit nor self destruction.
	float EmitTimer;	//timer for determining next particle instatiation.  Starting value = 1 / EmitRate.  Reduced by frameTime per frame.  Increased by 1/EmitRate everytime a particle is instantiated.

}Custom3D_PS;
/*WIP///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

#endif