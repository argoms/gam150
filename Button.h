#include "GameObject.h"
#include "AEEngine.h"
#include "LevelManager.h"
#include "Collision.h"

enum ButtonTypes 
{
  MAIN_MENU_BUTTON = 0,    /* Button that will take you to the main menu    */
  LEVEL_ONE_BUTTON,        /* Button that will take you to Level one        */
  LEVEL_TWO_BUTTON,        /* Button that will take you to Level two        */
  LEVEL_THREE_BUTTON,      /* Button that will take you to Level three      */
  DEATH_SCREEN_BUTTON,     /* Button that will take you to the death screen */
  TOWN_SCREEN_BUTTON,      /* Button that will take you to the town         */
  DP_SPLASH_SCREEN_BUTTON, /* digipen splashscreen level                    */
  NUMBER_OF_BUTTON_TYPES   /* number of buttons in this enum. MUST B LAST   */  
};

typedef struct 
{
  int type;               /* type of button                     */
  void (*onClick)(int);   /* function pointer for a click       */
  void (*onOver)(int);    /* function pointer for mouse over    */
  void (*onRelease)(int); /* function pointer for mouse release */
  int next;               /* next level to load                 */
  float Xscale;           /* x scale of the sprite              */
  float Yscale;           /* y scale of the sprite              */
  float size;             /* size of the button                 */
}Button;

GameObject *CreateButton(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _button_type, float _size, float _scalex, float _scaley);
//void ChangeNextLevel(int level);
void PlayPressedAnimation(GameObject *button);
void PlayButtonHoverAnimation(GameObject *button);
void PlayButtonReleasedAnimation(GameObject *button);
void ChangeButtonNextLevel(GameObject *button, int level);
void ChangeButtonType(GameObject *button, int type);
void ButtonSimulate(GameObject *button);