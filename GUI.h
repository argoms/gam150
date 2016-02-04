#ifndef GUI_H
#define GUI_H

#define EPSILON 0.0001
#define GUI_OBJ_INST_MAX 128


//TRANSFORM VALUES ARE MEANT FOR THE BUTTON REGISTRATION
//SEPERATE TRANSFORMATION MATRICES AND RENDER SETTINGS ARE REQUIRED FOR DRAWING MANUALLY
typedef struct GUI_ObjectInstance
{
	AEGfxVertexList	*pMesh;
	AEGfxTexture *pTexture;

	//unsigned char	sName[64];	//name of button

	float	fPositionX;		//x position of button relative to the screen
	float	fPositionY;		//y position of button relative to the screen
	float	fScaleX;		//x scale of button on screen
	float	fScaleY;		//y scale of button on screen
	float	fDrag_OffsetX;	//x offset while being dragged
	float	fDrag_OffsetY;	//y offset while being dragged

	int		iActiveFlag;			//is this GUI object instance active for updating and drawing? (1 = yes, 0 = no)
	int		iLayer;                 //how it is layered when drawed and rendered (larger = top, 0 = bottom)
	int		iMobile;				//can it be dragged? (1 = yes, 0 = no)
	int		iClicked;				//is the button clicked? (1  yes, 0 = no)
	int		iDragMode;				//is the button being dragged (1 = yes, 0 = no)

									//these are used in GUI_Update
	void(*vpIdle)(GUI_ObjectInstance *pGUI_Inst);		//for button events can
	void(*vpHover)(GUI_ObjectInstance *pGUI_Inst);		//for button events
	void(*vpDown)(GUI_ObjectInstance *pGUI_Inst);		//for button events
	void(*vpRelease)(GUI_ObjectInstance *pGUI_Inst);	//for button events

	void(*vpDrawMethod)(GUI_ObjectInstance *pGUI_Inst);	//for how you want the button/menu/menu item to be drawn (filters, shaders, blend mode, etc.)
								//
}
GUI_ObjectInstance;


//call this in the level's load function
void LoadGUI(int GUI_ObjectTotal);  //loads GUI object instance array with instance total = GUI_ObjectTotal


//call this function in the level's update function
void UpdateGUI(float *CamPosX, float *CamPosY, float *MousePosX, float *MousePosY);  //updates all GUI objects, must pass in address of variables listed


void QuickGUI_Draw(GUI_ObjectInstance *pGUI_Inst);  //default draw method for GUI object if not provided a custom one


//call this function at THE END OF LEVEL'S DRAW FUNCTION
void DrawGUI(void);	//draws all of the GUI object instances.  YOU MAY HAVE TO SET A HIGH Z-ORDER VALUE FOR ALL OF THEIR DrawMethod FUNCTIONS IF Z-ORDER IS USED ON YOUR LEVEL GME OBJECT INSTANCES


//it is recommended to call this function in the level's initialization function.
GUI_ObjectInstance *CreateGUI_ObjectInstance(void);  //call this function to create a GUI object instance.  function will return a pointer, which you can use to set its variables


void DestroyGUI_ObjectInstance(GUI_ObjectInstance *pGUI_Inst); //call this if you want to destroy a GUI object instance for whatever reason. DO NOT USE THIS FUNCTION IN REPLACEMENT OF FreeGUI or UnloadGUI


void FreeGUI(void);  //Free dynamically allocated memory occupied by all GUI object instances


void UnloadGUI(void);  //Free the entire GUI object instance array




//DEPRICATED CODE PLEASE IGNORE

/*
//for type of GUI item
enum GUI_TYPE
{
GUI_TYPE_MENU = 0,		//normal GUI item with no special properties
GUI_TYPE_BUTTON = 1,	//GUI item is a button that can be clicked on
GUI_TYPE_DRAG = 2,		//GUI item can be dragged around with mouse clicking and movements

GUI_TYPE_NUM
};
*/
/*
//for type of GUI item
enum GUI_TYPE
{
	GUI_TYPE_MENU = 0,		//normal GUI item with no special properties
	GUI_TYPE_BUTTON = 1,	//GUI item is a button that can be clicked on
	GUI_TYPE_DRAG = 2,		//GUI item can be dragged around with mouse clicking and movements

	GUI_TYPE_NUM
};

typedef struct GameObjectInstance GameObjectInstance;

/* WARNING: BUTTON EVENTS ARE ONLY CHECKED WITH TRANSFORM VALUES SET IN THE Component_Button COMPONENT
typedef struct Component_GUI
{
	int iGUI_Type;              //type of GUI item.  Uses the GUI_TYPE enum.
	unsigned char sName[64];	//name of button
	Vector2D v2Button_Position;	//position of button relative to the screen
	float fButton_ScaleX;		//x scale of button on screen
	float fButton_ScaleY;		//y scale of button on screen
	int iLayer;                 //how it is layered when drawed and rendered (larger = top, 0 = bottom)

	GameObjectInstance *goipOwner;

}Component_GUI;

int CheckButtonIdle(GameObjectInstance *); //check if button is not interected with

int CheckButtonHover(GameObjectInstance *);  //check if mouse cursor is hovering over button

int CheckButtonDown(GameObjectInstance *);  //check if player clicked down on the menu button

int CheckButtonReleased(GameObjectInstance *);  //check if player released clicking the menu button

void UpdateGUI(float *, float *, float *, float *, GameObjectInstance *);

void DrawGUI(void);

void AddGUI_Component(GameObjectInstance *, Component_GUI *);

void DeleteGUI_Component(GameObjectInstance *);
*/
/*

typedef struct Component_Button
{
	unsigned char sName[64];	//name of button
	Vector2D v2Button_Position;	//position of button on screen
	float fButton_ScaleX;		//x scale of button on screen
	float fButton_ScaleY;		//y scale of button on screen
	int iLayer;                 //how it is layered when drawed and rendered (larger = top, smaller = bottom

	GameObjectInstance *goipOwner;

}Component_Button;

typedef struct Component_Menu
{
	unsigned char sName[64];	//name of button
	Vector2D v2Button_Position;	//position of button on screen
	float fButton_ScaleX;		//x scale of button on screen
	float fButton_ScaleY;		//y scale of button on screen
	int iLayer;                 //how it is layered when drawed and rendered (larger = top, smaller = bottom

	GameObjectInstance *goipOwner;

}Component_Menu;

typedef struct Component_Menu_Item
{
	unsigned char sName[64];	//name of button
	Vector2D v2Button_Position;	//position of button on screen
	float fButton_ScaleX;		//x scale of button on screen
	float fButton_ScaleY;		//y scale of button on screen
	int iLayer;                 //how it is layered when drawed and rendered (larger = top, smaller = bottom

	GameObjectInstance *goipOwner;

}Component_Menu_Item;

*/


#endif