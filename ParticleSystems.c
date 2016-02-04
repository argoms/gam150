#include "AEEngine.h"
#include "ParticleSystems.h"
#include <math.h>
#include <stdio.h>

//based on float of pi * 2 = 6.2831853
#define RANDOM_ANGLE (float) ((rand() / (float)RAND_MAX) * 6.2831853)


//Dynamically allocated arrays as storage for PSs as assets

extern PS_Continuous_S	*pPS_Continuous_S_List;

extern PS_Burst_S		*pPS_Burst_S_List;

//extern PS_Continuous_NS *pPS_Continuous_NS_List;

extern PS_Burst_NS		*pPS_Burst_NS_List;


//array of PS instances
extern PS_Instance		goiaPS_ObjectInstanceList[PS_OBJ_INST_MAX];

extern AEGfxVertexList *pTempParticleMesh;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DeafaultInstantiation_Continuous_S(int i, PS_Instance *pPS_Inst)
{
	float RandomAngle = RANDOM_ANGLE;
	pPS_Inst->PS_Continuous_S->PosX[i] = pPS_Inst->PS_Continuous_S->StartPosX;
	pPS_Inst->PS_Continuous_S->PosY[i] = pPS_Inst->PS_Continuous_S->StartPosY;
	pPS_Inst->PS_Continuous_S->PosZ[i] = pPS_Inst->PS_Continuous_S->StartPosZ;
	pPS_Inst->PS_Continuous_S->VelX[i] = (float)(100 * cos(RandomAngle));
	pPS_Inst->PS_Continuous_S->VelY[i] = (float)(100 * cos(RandomAngle));
	pPS_Inst->PS_Continuous_S->VelZ[i] = 0.0f;
	pPS_Inst->PS_Continuous_S->Ang[i] = RANDOM_ANGLE;
	pPS_Inst->PS_Continuous_S->AngVel[i] = (float) (RANDOM_ANGLE - 3.14159265359);
	pPS_Inst->PS_Continuous_S->ScaleX[i] = 1.0f;
	pPS_Inst->PS_Continuous_S->ScaleY[i] = 1.0f;
	pPS_Inst->PS_Continuous_S->ScaleVelX[i] = 0.0f;
	pPS_Inst->PS_Continuous_S->ScaleVelY[i] = 0.0f;
	pPS_Inst->PS_Continuous_S->RefreshTimer[i] = pPS_Inst->PS_Continuous_S->LifeTime;
}

void DeafaultInstantiation_Burst_S(int i, PS_Instance *pPS_Inst)
{
	float RandomAngle = RANDOM_ANGLE;
	pPS_Inst->PS_Burst_S->PosX[i] = pPS_Inst->PS_Burst_S->StartPosX;
	pPS_Inst->PS_Burst_S->PosY[i] = pPS_Inst->PS_Burst_S->StartPosY;
	pPS_Inst->PS_Burst_S->PosZ[i] = pPS_Inst->PS_Burst_S->StartPosZ;
	pPS_Inst->PS_Burst_S->VelX[i] = (float)(100 * cos(RandomAngle));
	pPS_Inst->PS_Burst_S->VelY[i] = (float)(100 * cos(RandomAngle));
	pPS_Inst->PS_Burst_S->VelZ[i] = 0.0f;
	pPS_Inst->PS_Burst_S->Ang[i] = RANDOM_ANGLE;
	pPS_Inst->PS_Burst_S->AngVel[i] = (float)(RANDOM_ANGLE - 3.14159265359);
	pPS_Inst->PS_Burst_S->ScaleX[i] = 1.0f;
	pPS_Inst->PS_Burst_S->ScaleY[i] = 1.0f;
	pPS_Inst->PS_Burst_S->ScaleVelX[i] = 0.0f;
	pPS_Inst->PS_Burst_S->ScaleVelY[i] = 0.0f;
}

void DefaultUpdate_Continuous_S(int i, PS_Instance *pPS_Inst, float frameTime)
{
	pPS_Inst->PS_Continuous_S->PosX[i] += pPS_Inst->PS_Continuous_S->VelX[i] * frameTime;
	pPS_Inst->PS_Continuous_S->PosY[i] += pPS_Inst->PS_Continuous_S->VelY[i] * frameTime;
	pPS_Inst->PS_Continuous_S->PosZ[i] += pPS_Inst->PS_Continuous_S->VelZ[i] * frameTime;
	pPS_Inst->PS_Continuous_S->Ang[i] += pPS_Inst->PS_Continuous_S->AngVel[i] * frameTime;
	pPS_Inst->PS_Continuous_S->RefreshTimer[i] -= frameTime;
}

void DefaultUpdate_Burst_S(int i, PS_Instance *pPS_Inst, float frameTime)
{
	pPS_Inst->PS_Burst_S->PosX[i] += pPS_Inst->PS_Burst_S->VelX[i] * frameTime;
	pPS_Inst->PS_Burst_S->PosY[i] += pPS_Inst->PS_Burst_S->VelY[i] * frameTime;
	pPS_Inst->PS_Burst_S->PosZ[i] += pPS_Inst->PS_Burst_S->VelZ[i] * frameTime;
	pPS_Inst->PS_Burst_S->Ang[i] += pPS_Inst->PS_Burst_S->AngVel[i] * frameTime;
	pPS_Inst->PS_Burst_S->DeathTimer -= frameTime;
}

void DefaultDrawSettings(void)
{
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(NULL, 0, 0);
}

void DefaultDraw_Continuous_S(int i, PS_Instance *pPS_Inst)
{
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, pPS_Inst->PS_Continuous_S->RefreshTimer[i] / pPS_Inst->PS_Continuous_S->LifeTime);
	AEGfxSetFullTransformWithZOrder(pPS_Inst->PS_Continuous_S->PosX[i], pPS_Inst->PS_Continuous_S->PosY[i], pPS_Inst->PS_Continuous_S->PosZ[i],
		pPS_Inst->PS_Continuous_S->Ang[i], pPS_Inst->PS_Continuous_S->ScaleX[i], pPS_Inst->PS_Continuous_S->ScaleY[i]);

	AEGfxMeshDraw(pTempParticleMesh, AE_GFX_MDM_TRIANGLES);
}

void DefaultDraw_Burst_S(int i, PS_Instance *pPS_Inst)
{
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, pPS_Inst->PS_Burst_S->DeathTimer / pPS_Inst->PS_Burst_S->LifeTime);
	AEGfxSetFullTransformWithZOrder(pPS_Inst->PS_Burst_S->PosX[i], pPS_Inst->PS_Burst_S->PosY[i], pPS_Inst->PS_Burst_S->PosZ[i],
		pPS_Inst->PS_Burst_S->Ang[i], pPS_Inst->PS_Burst_S->ScaleX[i], pPS_Inst->PS_Burst_S->ScaleY[i]);

	AEGfxMeshDraw(pTempParticleMesh, AE_GFX_MDM_TRIANGLES);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void Load_PS_Pool(int iPS_Continuous_S, int iPS_Burst_S, int iPS_Continuous_NS, int iPS_Burst_NS)
{
	int i;
	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		goiaPS_ObjectInstanceList[i].PS_Continuous_S = NULL;
		goiaPS_ObjectInstanceList[i].PS_Burst_S = NULL;
		//goiaPS_ObjectInstanceList[i].PS_Continuous_NS = NULL;
		goiaPS_ObjectInstanceList[i].PS_Burst_NS = NULL;
		goiaPS_ObjectInstanceList[i].ExistFlag = 0;
	}

	pPS_Continuous_S_List = (PS_Continuous_S *)calloc(0, sizeof(PS_Continuous_S) * iPS_Continuous_S);
	pPS_Burst_S_List = (PS_Continuous_S *)calloc(0, sizeof(PS_Continuous_S) * iPS_Continuous_S);
	//pPS_Continuous_NS_List = (PS_Continuous_S *)calloc(0, sizeof(PS_Continuous_S) * iPS_Continuous_S);
	pPS_Burst_NS_List = (PS_Continuous_S *)calloc(0, sizeof(PS_Continuous_S) * iPS_Continuous_S);

	//form a normalized square mesh
	AEGfxMeshStart();
	//Upper Right 45-45-90 triangle
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	//Lower Left 45-45-90 triangle
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
		0.5, -0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pTempParticleMesh = AEGfxMeshEnd();
}


PS_Instance * Create_PS_Continuous_S(float LifeTime, float EmitRate, int EmitCount)
{
	int i;
	PS_Instance *pPS_Inst;
	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;

		if (pPS_Inst->ExistFlag == 0)
		{
			if (pPS_Inst->PS_Continuous_S == NULL)
			{
				pPS_Inst->PS_Continuous_S = (PS_Continuous_S *)calloc(0, sizeof(PS_Continuous_S));
				//initialize everything
				if ((EmitCount > 0) && (EmitCount < (int)(EmitRate * LifeTime * 1.1f)))
				{
					pPS_Inst->PS_Continuous_S->PosX = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->PosY = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->PosZ = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->VelX = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->VelY = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->VelZ = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->Ang = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->AngVel = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->ScaleX = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->ScaleY = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->ScaleVelX = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->ScaleVelY = (float *)calloc(0.0f, sizeof(float) * EmitCount);
					pPS_Inst->PS_Continuous_S->RefreshTimer = (float *)calloc(0.0f, sizeof(float) * EmitCount);
				}
				else
				{
					pPS_Inst->PS_Continuous_S->PosX = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->PosY = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->PosZ = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->VelX = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->VelY = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->VelZ = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->Ang = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->AngVel = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->ScaleX = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->ScaleY = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->ScaleVelX = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->ScaleVelY = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
					pPS_Inst->PS_Continuous_S->RefreshTimer = (float *)calloc(0.0f, sizeof(float) * (int)(EmitRate * LifeTime * 1.1f));
				}
				pPS_Inst->PS_Continuous_S->LifeTime = LifeTime;
				pPS_Inst->PS_Continuous_S->EmitRate = EmitRate;
				pPS_Inst->PS_Continuous_S->EmitCount = EmitCount;
				pPS_Inst->PS_Continuous_S->ShutDown = 1;
				pPS_Inst->PS_Continuous_S->EmitTimer = 1.0f / EmitRate;
				pPS_Inst->ExistFlag = 1;

				pPS_Inst->PS_Continuous_S->vpParticle_Instantiation = DeafaultInstantiation_Continuous_S;
				pPS_Inst->PS_Continuous_S->vpParticle_Update = DefaultUpdate_Continuous_S;
				pPS_Inst->PS_Continuous_S->vpDrawSettings = DefaultDrawSettings;
				pPS_Inst->PS_Continuous_S->vpParticle_Draw = DefaultDraw_Continuous_S;
				break;
			}
		}
	}
	return pPS_Inst;
}


PS_Instance * Create_PS_Burst_S(float LifeTime, int EmitCount)
{
	int i;
	PS_Instance *pPS_Inst;
	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;

		if (pPS_Inst->ExistFlag == 0)
		{
			if (pPS_Inst->PS_Burst_S == NULL)
			{
				pPS_Inst->PS_Burst_S = (PS_Burst_S *)calloc(0, sizeof(PS_Burst_S));
				//initialize everything
				pPS_Inst->PS_Burst_S->PosX = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->PosY = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->PosZ = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->VelX = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->VelY = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->VelZ = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->Ang = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->AngVel = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->ScaleX = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->ScaleY = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->ScaleVelX = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->ScaleVelY = (float *)calloc(0, sizeof(float) * EmitCount);
				pPS_Inst->PS_Burst_S->LifeTime = LifeTime;
				pPS_Inst->PS_Burst_S->DeathTimer = LifeTime;
				pPS_Inst->PS_Burst_S->EmitCount = EmitCount;
				pPS_Inst->PS_Burst_S->ShutDown = 1;
				pPS_Inst->ExistFlag = 1;

				pPS_Inst->PS_Burst_S->vpParticle_Instantiation = DeafaultInstantiation_Burst_S;
				pPS_Inst->PS_Burst_S->vpParticle_Update = DefaultUpdate_Burst_S;
				pPS_Inst->PS_Burst_S->vpDrawSettings = DefaultDrawSettings;
				pPS_Inst->PS_Burst_S->vpParticle_Draw = DefaultDraw_Burst_S;
				break;
			}
		}
	}
	return pPS_Inst;
}


PS_Instance * Create_PS_Burst_NS(float LifeTime, int EmitCount)
{
	int i;
	PS_Instance *pPS_Inst;
	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;

		if (pPS_Inst->ExistFlag == 0)
		{
			if (pPS_Inst->PS_Burst_S == NULL)
			{
				pPS_Inst->PS_Burst_S = (PS_Burst_NS *)calloc(0, sizeof(PS_Burst_NS));
				//initialize everything
				pPS_Inst->PS_Burst_NS->LifeTime = LifeTime;
				pPS_Inst->PS_Burst_NS->DeathTimer = LifeTime;
				pPS_Inst->PS_Burst_NS->EmitCount = EmitCount;
				pPS_Inst->PS_Burst_NS->ShutDown = 1;
				pPS_Inst->ExistFlag = 1;
				break;
			}
		}
	}
	return pPS_Inst;
}


void Start_PS(PS_Instance *pPS_Inst)
{
	if (pPS_Inst->ExistFlag)
	{
		if (pPS_Inst->PS_Continuous_S != NULL)
		{
			pPS_Inst->PS_Continuous_S->ShutDown = 0;
		}
		else if (pPS_Inst->PS_Burst_S != NULL)
		{
			pPS_Inst->PS_Burst_S->ShutDown = 0;
		}
		else if (pPS_Inst->PS_Burst_NS != NULL)
		{
			pPS_Inst->PS_Burst_NS->ShutDown = 0;
		}
	}
}


void Stop_PS(PS_Instance *pPS_Inst)
{
	if (pPS_Inst->ExistFlag)
	{
		if (pPS_Inst->PS_Continuous_S != NULL)
		{
			pPS_Inst->PS_Continuous_S->ShutDown = 1;
		}
		else if (pPS_Inst->PS_Burst_S != NULL)
		{
			pPS_Inst->PS_Burst_S->ShutDown = 1;
		}
		else if (pPS_Inst->PS_Burst_NS != NULL)
		{
			pPS_Inst->PS_Burst_NS->ShutDown = 1;
		}
	}
}



void Destroy_PS(PS_Instance *pPS_Inst)
{
	if (pPS_Inst->ExistFlag)
	{
		if (pPS_Inst->PS_Continuous_S != NULL)
		{
			//free static arrays, texture, and mesh
			free(pPS_Inst->PS_Continuous_S->PosX);
			free(pPS_Inst->PS_Continuous_S->PosY);
			free(pPS_Inst->PS_Continuous_S->PosZ);
			free(pPS_Inst->PS_Continuous_S->VelX);
			free(pPS_Inst->PS_Continuous_S->VelY);
			free(pPS_Inst->PS_Continuous_S->VelZ);
			free(pPS_Inst->PS_Continuous_S->Ang);
			free(pPS_Inst->PS_Continuous_S->AngVel);
			free(pPS_Inst->PS_Continuous_S->ScaleX);
			free(pPS_Inst->PS_Continuous_S->ScaleY);
			free(pPS_Inst->PS_Continuous_S->ScaleVelX);
			free(pPS_Inst->PS_Continuous_S->ScaleVelY);
			free(pPS_Inst->PS_Continuous_S->RefreshTimer);
			AEGfxMeshFree(pPS_Inst->PS_Continuous_S->pMesh);
			AEGfxTextureUnload(pPS_Inst->PS_Continuous_S->pTexture);
			//free everything else
			free(pPS_Inst->PS_Continuous_S);
			pPS_Inst->PS_Continuous_S = NULL;
			pPS_Inst->ExistFlag = 0;
		}
		else if (pPS_Inst->PS_Burst_S != NULL)
		{
			//free static arrays, texture, and mesh
			free(pPS_Inst->PS_Burst_S->PosX);
			free(pPS_Inst->PS_Burst_S->PosY);
			free(pPS_Inst->PS_Burst_S->PosZ);
			free(pPS_Inst->PS_Burst_S->VelX);
			free(pPS_Inst->PS_Burst_S->VelY);
			free(pPS_Inst->PS_Burst_S->VelZ);
			free(pPS_Inst->PS_Burst_S->Ang);
			free(pPS_Inst->PS_Burst_S->AngVel);
			free(pPS_Inst->PS_Burst_S->ScaleX);
			free(pPS_Inst->PS_Burst_S->ScaleY);
			free(pPS_Inst->PS_Burst_S->ScaleVelX);
			free(pPS_Inst->PS_Burst_S->ScaleVelY);
			AEGfxMeshFree(pPS_Inst->PS_Burst_S->pMesh);
			AEGfxTextureUnload(pPS_Inst->PS_Burst_S->pTexture);
			//free everything else
			free(pPS_Inst->PS_Burst_S);
			pPS_Inst->PS_Burst_S = NULL;
			pPS_Inst->ExistFlag = 0;
		}
		else if (pPS_Inst->PS_Burst_NS != NULL)
		{
			//free static arrays, texture, and mesh
			AEGfxMeshFree(pPS_Inst->PS_Burst_NS->pMesh);
			AEGfxTextureUnload(pPS_Inst->PS_Burst_NS->pTexture);
			//free everything else
			free(pPS_Inst->PS_Burst_NS);
			pPS_Inst->PS_Burst_NS = NULL;
			pPS_Inst->ExistFlag = 0;
		}
	}
}


void UpdateAllPS_Inst(float frameTime)
{
	int i, j, ParticleCount;
	PS_Instance *pPS_Inst;

	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;

		if (pPS_Inst->ExistFlag == 0)
			continue;

		if (pPS_Inst->PS_Continuous_S != NULL)
		{
			if (!(pPS_Inst->PS_Continuous_S->ShutDown))
			{
				pPS_Inst->PS_Continuous_S->EmitTimer -= frameTime;
			}
			ParticleCount = sizeof(pPS_Inst->PS_Continuous_S->PosX) / sizeof(pPS_Inst->PS_Continuous_S->PosX[0]);

			for (j = 0; j < ParticleCount; j++)
			{
				if (pPS_Inst->PS_Continuous_S->RefreshTimer[j] > 0.0f)
				{
					if (pPS_Inst->PS_Continuous_S->vpParticle_Update != NULL)
					{
						(pPS_Inst->PS_Continuous_S->vpParticle_Update)(j, pPS_Inst, frameTime);
					}
				}
				else
				{
					if (!(pPS_Inst->PS_Continuous_S->ShutDown))
					{
						if (pPS_Inst->PS_Continuous_S->EmitTimer < 0.0f)
						{
							(pPS_Inst->PS_Continuous_S->vpParticle_Instantiation)(j, pPS_Inst);

							if (pPS_Inst->PS_Continuous_S->EmitCount > 0)
							{
								--(pPS_Inst->PS_Continuous_S->EmitCount);
							}
						}
					}
				}
			}
			if (pPS_Inst->PS_Continuous_S->EmitCount == 0)
			{
				pPS_Inst->PS_Continuous_S->EmitCount = -1;
				pPS_Inst->PS_Continuous_S->ShutDown = 1;
				pPS_Inst->PS_Continuous_S->EmitTimer = pPS_Inst->PS_Continuous_S->LifeTime;
			}
			//if ((pPS_Inst->PS_Continuous_S->EmitTimer < 0.0f) && (pPS_Inst->PS_Continuous_S->ShutDown))
			//{
			//	Destroy_Continuous2D_PS(pPS_Inst);
			//}
		}
		else if ((pPS_Inst->PS_Burst_S != NULL)&&(pPS_Inst->PS_Burst_S->ShutDown != 1))
		{
			ParticleCount = pPS_Inst->PS_Burst_S->EmitCount;

			if (pPS_Inst->PS_Burst_S->DeathTimer == pPS_Inst->PS_Burst_S->LifeTime)
			{
				for (j = 0; j < ParticleCount; j++)
				{
					if (pPS_Inst->PS_Burst_S->vpParticle_Instantiation != NULL)
					{
						(pPS_Inst->PS_Burst_S->vpParticle_Instantiation)(j, pPS_Inst);
					}
				}
			}
			pPS_Inst->PS_Burst_S->DeathTimer -= frameTime;

			for (j = 0; j < pPS_Inst->PS_Burst_S->EmitCount; j++)
			{
				if (pPS_Inst->PS_Burst_S->vpParticle_Update != NULL)
				{
					(pPS_Inst->PS_Burst_S->vpParticle_Update)(j, pPS_Inst, frameTime);
				}
			}
			
			if (pPS_Inst->PS_Burst_S->DeathTimer < 0.0f)
			{
				pPS_Inst->PS_Burst_S->DeathTimer = pPS_Inst->PS_Burst_S->LifeTime;
				Stop_PS(pPS_Inst);
			}
		}
		else if ((pPS_Inst->PS_Burst_NS != NULL) && (pPS_Inst->PS_Burst_NS->ShutDown != 1))
		{
			ParticleCount = pPS_Inst->PS_Burst_NS->EmitCount;
			pPS_Inst->PS_Burst_NS->DeathTimer -= frameTime;

			for (j = 0; j < pPS_Inst->PS_Burst_NS->EmitCount; j++)
			{
				if (pPS_Inst->PS_Burst_NS->vpParticle_Update != NULL)
				{
					(pPS_Inst->PS_Burst_NS->vpParticle_Update)(j, pPS_Inst, frameTime);
				}
			}

			if (pPS_Inst->PS_Burst_NS->DeathTimer < 0.0f)
			{
				pPS_Inst->PS_Burst_NS->DeathTimer = pPS_Inst->PS_Burst_NS->LifeTime;
				Stop_PS(pPS_Inst);
			}
		}
	}

}

void DrawAllPS_Inst(void)
{
	int i, j, ParticleCount;
	PS_Instance *pPS_Inst;

	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;

		if (pPS_Inst->ExistFlag == 0)
			continue;

		if (pPS_Inst->PS_Continuous_S != NULL)
		{
			if (pPS_Inst->PS_Continuous_S->vpDrawSettings != NULL)
			{
				(pPS_Inst->PS_Continuous_S->vpDrawSettings)();
			}
			ParticleCount = sizeof(pPS_Inst->PS_Continuous_S->PosX) / sizeof(pPS_Inst->PS_Continuous_S->PosX[0]);
			for (j = 0; j < ParticleCount; j++)
			{
				if (pPS_Inst->PS_Continuous_S->RefreshTimer[j] > 0.0f)
				{
					if (pPS_Inst->PS_Continuous_S->vpParticle_Draw != NULL)
					{
						(pPS_Inst->PS_Continuous_S->vpParticle_Draw)(j, pPS_Inst);
					}
				}
			}
		}
		else if ((pPS_Inst->PS_Burst_S != NULL) && (pPS_Inst->PS_Burst_S->ShutDown != 1))
		{
			if (pPS_Inst->PS_Burst_S->vpDrawSettings != NULL)
			{
				(pPS_Inst->PS_Burst_S->vpDrawSettings)();
			}
			ParticleCount = sizeof(pPS_Inst->PS_Burst_S->PosX) / sizeof(pPS_Inst->PS_Burst_S->PosX[0]);
			for (j = 0; j < ParticleCount; j++)
			{
				if (pPS_Inst->PS_Burst_S->vpParticle_Draw != NULL)
				{
					(pPS_Inst->PS_Burst_S->vpParticle_Draw)(j, pPS_Inst);
				}
			}
		}
		else if ((pPS_Inst->PS_Burst_NS != NULL) && (pPS_Inst->PS_Burst_NS->ShutDown != 1))
		{
			if (pPS_Inst->PS_Burst_NS->vpDrawSettings != NULL)
			{
				(pPS_Inst->PS_Burst_NS->vpDrawSettings)();
			}
			ParticleCount = pPS_Inst->PS_Burst_NS->EmitCount;
			for (j = 0; j < ParticleCount; j++)
			{
				if (pPS_Inst->PS_Burst_NS->vpParticle_Draw != NULL)
				{
					(pPS_Inst->PS_Burst_NS->vpParticle_Draw)(j, pPS_Inst);
				}
			}
		}
	}
}

void UnloadAllPS_Inst(void)
{
	int i;
	PS_Instance *pPS_Inst;

	for (i = 0; i < PS_OBJ_INST_MAX; i++)
	{
		pPS_Inst = goiaPS_ObjectInstanceList + i;

		if (pPS_Inst->ExistFlag == 0)
			continue;

		Destroy_PS(pPS_Inst);
	}
}

/* may not be needed
void UnloadAllPS_Inst(void)
{

}
*/