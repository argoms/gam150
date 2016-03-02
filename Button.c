/*****************************************************************************/
/*!
\file   Button.c
\author Tarrant Mangasarian
\par    email: tarrant.mangasarian\@digipen.edu
\par    DigiPen login: tarrant.mangasarian
\par    Course: GAM150
\date   2/19/2016
\brief
This file contains the function implementations for conversions
include this if you have warnings with type conversions
*/
/*****************************************************************************/
#include "Button.h"


/*************************************************************************/
/*!
\par   Function: CreateButton

\par Description: Creates a button, similar to create object instance but
                  adds a button component and the function takes an int 
                  for the type of button

\param _physics physics component to add

\param _sprite sprite component to add

\param _entity entity component to add

\param _type type of the new object

\param _button_typen type of button

\return Returns a pointer to the new gameobject.
*/
/*************************************************************************/
GameObject *CreateButton(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _button_type)
{
  GameObject *buttonObject; /* button object pointer              */
  Button* buttonComponent;  /* reference to the component created */

  /* call the create game object function to make the button */
  buttonObject = GameObjectCreate(_physics, _sprite, _entity, entity_button);  

  buttonComponent = (Button*)malloc(sizeof(Button));    /* malloc the button component */
  buttonComponent->type = _button_type;                 /* give the component the type */
  /* the type will determine where the next level will be upon function call */

  // set the function pointers to point to whatever function you wish to call
  switch (_button_type)
  {
  case MAIN_MENU_BUTTON:
  {
    buttonComponent->onClick = &PlayPressedAnimation;
    buttonComponent->onOver = &PlayButtonHoverAnimation;
    buttonComponent->onRelease = &PlayButtonReleasedAnimation;
    buttonComponent->next = level_mainMenu;
  }
  break;
  case LEVEL_ONE_BUTTON:
  {
    buttonComponent->onClick = &PlayPressedAnimation;
    buttonComponent->onOver = &PlayButtonHoverAnimation;
    buttonComponent->onRelease = &PlayButtonReleasedAnimation;
    buttonComponent->next = level_level1;
  }
  break;
  case LEVEL_TWO_BUTTON:
  {
    buttonComponent->onClick = &PlayPressedAnimation;
    buttonComponent->onOver = &PlayButtonHoverAnimation;
    buttonComponent->onRelease = &PlayButtonReleasedAnimation;
    buttonComponent->next = NULL;
  }
  break;
  case LEVEL_THREE_BUTTON:
  {
    buttonComponent->onClick = &PlayPressedAnimation;
    buttonComponent->onOver = &PlayButtonHoverAnimation;
    buttonComponent->onRelease = &PlayButtonReleasedAnimation;
    buttonComponent->next = NULL;
  }
  break;
  case DEATH_SCREEN_BUTTON:
  {
    buttonComponent->onClick = &PlayPressedAnimation;
    buttonComponent->onOver = &PlayButtonHoverAnimation;
    buttonComponent->onRelease = &PlayButtonReleasedAnimation;
    buttonComponent->next = NULL;
  }
  break;
  case TOWN_SCREEN_BUTTON:
  {
    buttonComponent->onClick = &PlayPressedAnimation;
    buttonComponent->onOver = &PlayButtonHoverAnimation;
    buttonComponent->onRelease = &PlayButtonReleasedAnimation;
    buttonComponent->next = level_town;
  }
  break;
  case DP_SPLASH_SCREEN_BUTTON:
  {
    buttonComponent->onClick = &PlayPressedAnimation;
    buttonComponent->onOver = &PlayButtonHoverAnimation;
    buttonComponent->onRelease = &PlayButtonReleasedAnimation;
    buttonComponent->next = level_town;
  }
  break;
  default:
  {
    /* set everything to null for the default */
    buttonComponent->onClick = NULL;
    buttonComponent->onOver = NULL;
    buttonComponent->onRelease = NULL;
    buttonComponent->next = level_mainMenu;
  }
  break;

  return buttonObject;  //return the button object
  }

  return 0;
}

/*************************************************************************/
/*!
\par   Function: ChangeButtonNextLevel

\par Description: Changes the next level to go to for the buttton, 
                  different from type, type is like the id, 
                  next is the level

\param button pointer to the button game object, not its component

\param level  integer representing the new level to switch to
*/
/*************************************************************************/
void ChangeButtonNextLevel(GameObject *button, int level)
{
  //check if null
  if (button == NULL)
  {
    return;
  }
  //check if it is a button
  if (button->type != entity_button)
  {
    return;
  }

  //get the button data
  Button *button_data = button->miscData;

  //check if it is null
  if (button_data == NULL)
  {
    return;
  }

  //change the level
  button_data->next = level;
}

/*************************************************************************/
/*!
\par   Function: ChangeButtonType

\par Description: Changes the button type, so it can be identified as a 
                  different button, different from next. Next is the next level.
                  Type is the id of the button.

\param button pointer to the button game object, not its component

\param type  integer representing the new type for the button
*/
/*************************************************************************/
void ChangeButtonType(GameObject *button, int type)
{
  if (button == NULL)
  {
    return;
  }

  if (button->type != entity_button)
  {
    return;
  }

  Button *button_data = button->miscData;

  if (button_data == NULL)
  {
    return;
  }

  button_data->type = type;
}

/*************************************************************************/
/*!
\par   Function: PlayPressedAnimation

\par Description: Plays the button pressed animation, 
                  NEEDS TO BE COMPLETED WITH AN ANIMATION

\param button pointer to the button game object, not its component

*/
/*************************************************************************/
void PlayPressedAnimation(GameObject *button)
{
  //check if null
  if (button == NULL)
  {
    return;
  }
  //check if it is a button
  if (button->type != entity_button)
  {
    return;
  }

  //get the button data
  Button *button_data = button->miscData;

  //check if it is null
  if (button_data == NULL)
  {
    return;
  }


  printf("played pressed animation");
}

/*************************************************************************/
/*!
\par   Function: PlayButtonHoverAnimation

\par Description: Plays the button hover animation,
                  NEEDS TO BE COMPLETED WITH AN ANIMATION

\param button pointer to the button game object, not its component

*/
/*************************************************************************/
void PlayButtonHoverAnimation(GameObject *button)
{
  //check if null
  if (button == NULL)
  {
    return;
  }
  //check if it is a button
  if (button->type != entity_button)
  {
    return;
  }

  //get the button data
  Button *button_data = button->miscData;

  //check if it is null
  if (button_data == NULL)
  {
    return;
  }
  printf("played hover animation");
}

/*************************************************************************/
/*!
\par   Function: PlayButtonReleasedAnimation

\par Description: Plays the button release animation,
                  NEEDS TO BE COMPLETED WITH AN ANIMATION

\param button pointer to the button game object, not its component

*/
/*************************************************************************/
void PlayButtonReleasedAnimation(GameObject *button)
{
  if (button == NULL)
  {
    printf("attempted to play release animation");
    return;
  }

  if (button->type != entity_button)
  {
    return;
  }

  Button *button_data = button->miscData;
  printf("played released animation");
  int level = button_data->type;
  LevelSetNext(level);
}


