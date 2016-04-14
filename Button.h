/*****************************************************************************/
/*!
\file   Button.h
\author Tarrant Mangasarian
\par    email: tarrant.mangasarian\@digipen.edu
\par    DigiPen login: tarrant.mangasarian
\par    Course: GAM150
\date   2/19/2016
\brief
This file contains the function declarations for buttons
*/
/*****************************************************************************/
#include "GameObject.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Collision.h"

#define BUTTON_ALPHA_MODIFIER              1.65f
#define BUTTON_RED_MODIFIER                1.65f
#define BUTTON_BLUE_MODIFIER               1.65f
#define BUTTON_GREEN_MODIFIER              1.65f

#define DONT_CHANGE_LEVEL                  - 1

enum ButtonTypes 
{
  MAIN_MENU_BUTTON = 0,    /* Button that will take you to the main menu    */
  LEVEL_ONE_BUTTON,        /* Button that will take you to Level one        */
  LEVEL_TWO_BUTTON,        /* Button that will take you to Level two        */
  LEVEL_THREE_BUTTON,      /* Button that will take you to Level three      */
  LEVEL_FOUR_BUTTON,       /* Button that will take u to level four         */
  DEATH_SCREEN_BUTTON,     /* Button that will take you to the death screen */
  WIN_SCREEN,              /* Button for the win screen                     */
  TOWN_SCREEN_BUTTON,      /* Button that will take you to the town         */
  DP_SPLASH_SCREEN_BUTTON, /* digipen splashscreen level                    */
  CONTROL_SCREEN,          /* button for the control screen                 */
  CREDIT_SCREEN,           /* button for the credit screen                  */
  LEVEL_SELECT,            /* level select                                  */
  NUMBER_OF_BUTTON_TYPES,  /* number of buttons in this enum. MUST B LAST   */ 
  QUIT                     /* buttton for quitting                          */
};

typedef struct 
{
  int type;               /* type of button                     */
  void (*onClick)(void);   /* function pointer for a click       */
  void (*onOver)(void);    /* function pointer for mouse over    */
  void (*onRelease)(void); /* function pointer for mouse release */
  int next;               /* next level to load                 */
  float Xscale;           /* x scale of the sprite              */
  float Yscale;           /* y scale of the sprite              */
  float size;             /* size of the button                 */
  int isSelected;         /* is the button pressed              */
  int isActive;           /* is the button active               */
}Button;

GameObject *CreateButton(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _button_type, float _size, float _scalex, float _scaley);
//void ChangeNextLevel(int level);
void PlayPressedAnimation(GameObject *button);
void PlayButtonHoverAnimation(GameObject *button);
void PlayButtonReleasedAnimation(GameObject *button);
void ChangeButtonNextLevel(GameObject *button, int level);
void ChangeButtonType(GameObject *button, int type);
void ButtonSimulate(GameObject *button);
void ScaleButtonSpriteColor(GameObject *button);
void UnscaleButtonSpriteColor(GameObject *button);
void ReactivateAndDisplayButton(GameObject *button);
void FadeAndDisableButton(GameObject *button);
void DisableAndHideButton(GameObject *button);