#include "AEEngine.h"
#include "GUI.h"
#include <math.h>
#include <stdio.h>


static int iGlobalDragCancel = 0; //to prevent mouse from catching multiple objects while dragging a single object

extern GUI_ObjectInstance *goiaGUI_ObjectInstanceList;

extern AEGfxVertexList *pTempMesh;

void LoadGUI(int GUI_ObjectTotal)
{
	goiaGUI_ObjectInstanceList = (GUI_ObjectInstance *) calloc(0, sizeof(GUI_ObjectInstance) * GUI_ObjectTotal);

	//form a normalized square mesh
	AEGfxMeshStart();
	//Upper Right 45-45-90 triangle
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF999999, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFF999999, 0.0f, 0.0f,
		0.5, 0.5f, 0xFF999999, 0.0f, 0.0f);
	//Lower Left 45-45-90 triangle
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFF999999, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFF999999, 0.0f, 0.0f,
		0.5, -0.5f, 0xFF999999, 0.0f, 0.0f);
	pTempMesh = AEGfxMeshEnd();
}

//Update function for GUI Object Instances
void UpdateGUI(float *CamPosX, float *CamPosY, float *MousePosX, float *MousePosY)
{
	int i;	//loop counter
	GUI_ObjectInstance *pGUI_Inst;	//GUI Instance Pointer

	//cycle through all GUI Instances
	for (i = 0; i < GUI_OBJ_INST_MAX; i++)
	{
		pGUI_Inst = goiaGUI_ObjectInstanceList + i;
		if (pGUI_Inst->iActiveFlag == 0)
			continue;

		// if mouse cursor is away from button/GUI object
		if ((*MousePosX < pGUI_Inst->fPositionX - 0.5 * pGUI_Inst->fScaleX)
			|| (*MousePosX > pGUI_Inst->fPositionX + 0.5 * pGUI_Inst->fScaleX)
			|| (*MousePosY < pGUI_Inst->fPositionY - 0.5 * pGUI_Inst->fScaleY)
			|| (*MousePosY > pGUI_Inst->fPositionY + 0.5 * pGUI_Inst->fScaleY))
		{
			if (pGUI_Inst->vpIdle != NULL)
			{
				(pGUI_Inst->vpIdle)(pGUI_Inst);
			}
		}
		//all events for when mouse is over the GUI object
		else
		{
			if (AEInputCheckCurr(VK_LBUTTON))
			{
				if ((pGUI_Inst->iDragMode)||((!iGlobalDragCancel)&&(pGUI_Inst->iMobile)))
				{
					//this if statement is present to avoid reseting the drag offset repeatitively while dragging a GUI Object
					if (!(pGUI_Inst->iDragMode))
					{
						pGUI_Inst->iDragMode = 1;
						pGUI_Inst->fDrag_OffsetX = pGUI_Inst->fPositionX - *MousePosX - *CamPosX;
						pGUI_Inst->fDrag_OffsetY = pGUI_Inst->fPositionY - *MousePosY - *CamPosY;
					}
					if (pGUI_Inst->iDragMode)
					{
						pGUI_Inst->fPositionX = pGUI_Inst->fDrag_OffsetX + *MousePosX + *CamPosX;
						pGUI_Inst->fPositionY = pGUI_Inst->fDrag_OffsetY + *MousePosY + *CamPosY;
						//to prevent other GUI objects from being "caught" by the mouse while dragging a GUI object
						iGlobalDragCancel = 1;
					}
				}
				else
				{
					pGUI_Inst->fPositionX = *CamPosX;
					pGUI_Inst->fPositionY = *CamPosY;
				}
				if (pGUI_Inst->vpDown != NULL)
				{
					(pGUI_Inst->vpDown)(pGUI_Inst);
				}
				pGUI_Inst->iClicked = 1;
			}
			else
			{
				//must confirm that iGlobalDragCancel is caused by this specific GUI object in order to turn iGlobalDragCancel off
				if ((iGlobalDragCancel)&&(pGUI_Inst->iDragMode))
				{
					iGlobalDragCancel = 0;
				}
				pGUI_Inst->iDragMode = 0;

				//if GUI button was already clicked last frame, and is now not, then it must've been released
				if (pGUI_Inst->iClicked)
				{
					if (pGUI_Inst->vpRelease != NULL)
					{
						(pGUI_Inst->vpRelease)(pGUI_Inst);
					}
					pGUI_Inst->iClicked = 0;
				}
				else
				{
					if (pGUI_Inst->vpHover != NULL)
					{
						(pGUI_Inst->vpHover)(pGUI_Inst);
					}
				}
			}
		}
	}
}

// this is if you don't want to get fancy with drawing stuff and just want a button/menu/menu item
void QuickGUI_Draw(GUI_ObjectInstance *pGUI_Inst)
{
	//            y  x
	float TranMat[3][3];
	TranMat[0][0] = pGUI_Inst->fScaleX;
	TranMat[0][1] = 0.0f;
	TranMat[0][2] = pGUI_Inst->fPositionX;
	TranMat[1][0] = 0.0f;
	TranMat[1][1] = pGUI_Inst->fScaleY;
	TranMat[1][2] = pGUI_Inst->fPositionY;
	TranMat[2][0] = 0.0f;
	TranMat[2][1] = 0.0f;
	TranMat[2][2] = 1.0f;

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxTextureSet(pGUI_Inst->pTexture, 0, 0);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransform(TranMat);
	AEGfxMeshDraw(pGUI_Inst->pMesh, AE_GFX_MDM_TRIANGLES);
}

//draw function for GUI object instances
//MAKE SURE TO PUT AT END OF MAIN LEVEL DRAW FUNCTION
void DrawGUI(void)
{
	int i, iCurrentLayer = 0, iMaxLayer = 0;
	GUI_ObjectInstance *pGUI_Inst;

	//if max layer hasn't been exceeded yet
	while (iCurrentLayer >= iMaxLayer)
	{
		for (i = 0; i < GUI_OBJ_INST_MAX; i++)
		{
			pGUI_Inst = goiaGUI_ObjectInstanceList + i;
			if (pGUI_Inst->iActiveFlag == 0)
				continue;

			//draw GUI objects of the current layer
			if (pGUI_Inst->iLayer == iCurrentLayer)
			{
				if (pGUI_Inst->vpDrawMethod != NULL)
				{
					(pGUI_Inst->vpDrawMethod)(pGUI_Inst);
				}
			}
			else
			{
				//if GUI object isn't on current layer, but on a higher layer greater than iMaxLayer, set iMaxLayer to that new, higher layer.
				if (pGUI_Inst->iLayer > iMaxLayer)
				{
					iMaxLayer = pGUI_Inst->iLayer;
				}
			}
		}
		++iCurrentLayer;
	}
}

// USE THIS DURING LEVEL INITIALIZATION
GUI_ObjectInstance *CreateGUI_ObjectInstance(void)
{
	int i;
	GUI_ObjectInstance *pGUI_Inst;

	for (i = 0; i < GUI_OBJ_INST_MAX; i++)
	{
		pGUI_Inst = goiaGUI_ObjectInstanceList + i;

		if (pGUI_Inst->iActiveFlag == 0)
		{
			pGUI_Inst->iActiveFlag = 1;
			pGUI_Inst->fScaleX = 1.0f;
			pGUI_Inst->fScaleY = 1.0f;
			pGUI_Inst->vpDrawMethod = QuickGUI_Draw;
			pGUI_Inst->pMesh = pTempMesh;
			pGUI_Inst->pTexture = NULL;
			break;
		}
	}
	return pGUI_Inst;
}

//frees memory it ocupies and initializes variables to 0
void DestroyGUI_ObjectInstance(GUI_ObjectInstance *pGUI_Inst)
{
	if (pGUI_Inst->pMesh != NULL)
	{
		AEGfxMeshFree(pGUI_Inst->pMesh);
		pGUI_Inst->pMesh = NULL;
	}
	if (pGUI_Inst->pTexture != NULL)
	{
		AEGfxTextureUnload(pGUI_Inst->pTexture);
		pGUI_Inst->pTexture = NULL;
	}
	pGUI_Inst->fDrag_OffsetX = 0.0f;
	pGUI_Inst->fDrag_OffsetY = 0.0f;
	pGUI_Inst->fPositionX = 0.0f;
	pGUI_Inst->iClicked = 0;
	pGUI_Inst->iDragMode = 0;
	pGUI_Inst->iLayer = 0;
	pGUI_Inst->iMobile = 0;
	pGUI_Inst->vpIdle = NULL;
	pGUI_Inst->vpHover = NULL;
	pGUI_Inst->vpDown = NULL;
	pGUI_Inst->vpRelease = NULL;
	pGUI_Inst->vpDrawMethod = NULL;
	pGUI_Inst->iActiveFlag = 0;
}

//free memory occupied by GUI object instances.
void FreeGUI(void)
{
	int i;
	GUI_ObjectInstance *pGUI_Inst;

	for (i = 0; i < GUI_OBJ_INST_MAX; i++)
	{
		pGUI_Inst = goiaGUI_ObjectInstanceList + i;

		if (pGUI_Inst->iActiveFlag == 0)
			continue;

		DestroyGUI_ObjectInstance(pGUI_Inst);
	}
}

//free GUI object instance array
void UnloadGUI(void)
{
	free(goiaGUI_ObjectInstanceList);
	if (pTempMesh != NULL)
	{
		AEGfxMeshFree(pTempMesh);
	}
}

//DEPRICATED CODE PLEASE IGNORE

/*
void AddGUI_Component(GameObjectInstance *pGOI, Component_GUI cpComponent_GUI)
{
if (pGOI != NULL)
{
if (pGOI->cpComponent_GUI != NULL)
{
*(pGOI->cpComponent_GUI) = cpComponent_GUI;
}
//THE FOLLOWING IS AUTO-INITIALIZATIONs
if ((pGOI->cpComponent_GUI->iGUI_Type < GUI_TYPE_MENU) || (pGOI->cpComponent_GUI->iGUI_Type > GUI_TYPE_DRAG))
{
pGOI->cpComponent_GUI->iGUI_Type = GUI_TYPE_MENU;
}
}
}
*/

// THIS IS A HACKY THING BEING USED FOR TESTING PURPOSES.  DO NOT USE IN FINAL PRODUCT /////////////////
//typedef struct GameObjectInstance
//{
//	Component_GUI *cpComponent_GUI;
//}
//GameObjectInstance;
// THIS IS A HACKY THING BEING USED FOR TESTING PURPOSES.  DO NOT USE IN FINAL PRODUCT /////////////////



/* WARNING: BUTTON EVENTS ARE ONLY CHECKED WITH TRANSFORM VALUES SET IN THE Component_Button COMPONENT */
/*
typedef struct Component_GUI
{
int iGUI_Type;              //type of GUI item.  Uses the GUI_TYPE enum.
unsigned char sName[64];	//name of button
Vector2D v2Button_Position;	//position of button relative to the screen
float fButton_ScaleX;		//x scale of button on screen
float fButton_ScaleY;		//y scale of button on screen
int iLayer;                 //how it is layered when drawed and rendered (larger = top, 0 = bottom)

void(*vpIdle)(void);
void(*vpHover)(void);
void(*vpDown)(void);
void(*vpRelease)(void);

GameObjectInstance *goipOwner;

}Component_GUI;
*/

/*
int CheckButtonIdle(GameObjectInstance *); //check if button is not interected with

int CheckButtonHover(GameObjectInstance *);  //check if mouse cursor is hovering over button

int CheckButtonDown(GameObjectInstance *);  //check if player clicked down on the menu button

int CheckButtonReleased(GameObjectInstance *);  //check if player released clicking the menu button
*/

//static GUI_ObjectInstance goiaGUI_ObjectInstanceList[GUI_OBJ_INST_MAX];
