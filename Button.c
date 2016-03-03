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

#define LEFT_CLICK 1
#define RIGHT_CLICK 2

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

\param _button_type type of button

\param _size size of button

\param _scalex size of the x of the mesh

\param _scaley size of the y of the mesh

\return Returns a pointer to the new gameobject.
*/
/*************************************************************************/
GameObject *CreateButton(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity, int _button_type, float _size, float _scalex, float _scaley)
{
  printf("created button");

  GameObject *buttonObject;                   /* button object pointer              */
  Button* buttonComponent;                    /* reference to the component created */

  /* call the create game object function to make the button */
  buttonObject = GameObjectCreate(NULL, _sprite, _entity, entity_button);  

  buttonObject->simulate = &ButtonSimulate;   /* set the similate function          */

  buttonComponent = (Button*)malloc(sizeof(Button));    /* malloc the button component */
  buttonComponent->type = _button_type;                 /* give the component the type */
  buttonComponent->Xscale = _scalex;                    /* set the x scale             */
  buttonComponent->Yscale = _scaley;                    /* set the y scale             */
  buttonComponent->size = _size;                        /* set the size                */

  /* the type will determine where the next level will be upon function call */

  //free(buttonObject->physics);
  buttonObject->physics = NULL;


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

  
  }
  buttonObject->miscData = buttonComponent;
  return buttonObject;  //return the button object
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


  //printf("played pressed animation");
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
  //printf("played hover animation");
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
  if (button == NULL)                               /* check if it is null */
  {
    printf("attempted to play release animation and failed");  /* print               */
    return;
  }

  if (button->type != entity_button)
  {
    return;
  }

  Button *button_data = button->miscData; /* get the micelaneous data */
  //printf("played released animation");    /* print                    */
  int level = button_data->next;          /* get the level            */
  LevelSetNext(level);                    /* set the next level       */
}

/*************************************************************************/
/*!
\par   Function: ButtonSimulate

\par Description: Simulate function for the button. Checks if the mouse is
                  over the button(colliding), pressed, released. 
                  Releaasing the button switches the level or whatever 
                  the function pointer is set to.

\param button pointer to the button game object, not its component

*/
/*************************************************************************/
void ButtonSimulate(GameObject *button)
{
  float winMaxX;            
  float winMaxY;
  float winMinX;
  float winMinY;

  winMaxX = AEGfxGetWinMaxX();
  winMaxY = AEGfxGetWinMaxY();
  winMinX = AEGfxGetWinMinX();
  winMinY = AEGfxGetWinMinY();

  if (button == NULL)
  {
    return;
  }

  if (button->miscData == NULL)
  {
    return;
  }


  Button *button_data = button->miscData;
 
  float x = button->sprite->x;                  /* button x position            */
  float y = button->sprite->y;                  /* button y position            */
  Vector2D position;                            /* button position vector       */
  position.x = x;                               /* set the x of the rect*/
  position.y = y;                               /* set thr y of the rect*/
  float scalex = button_data->Xscale;           /* button width                 */
  float scaley = button_data->Yscale;           /* button height                */
                                                    
  signed long mouse_x = 0.0f;                   /* mouse x position             */
  signed long mouse_y = 0.0f;                   /* mouse y position             */
  AEInputGetCursorPosition(&mouse_x, &mouse_y); /* modify the x and y positions */
  mouse_x += - (winMaxX - winMinX) / 2;
  mouse_y = -1 * mouse_y + (winMaxY - winMinY) / 2;
  //mouse_y += - (mouse_y - (winMaxY - winMinY) / 2);

  Vector2D point;                               /* vector for the mouse pos     */
  point.x = mouse_x;      /* set the x of the mouse       */
  point.y = mouse_y;      /* set the y of the mouse       */

  //AEGfxConvertScreenCoordinatesToWorld(mouse_x, mouse_y, &point.x,&point.y);

  if (PointToRect(&point, &position, scalex, scaley))
  {
    int derp = 1 + 1;
  }
  
    //if a left click and collision of mouse position and button position
  if (AEInputCheckCurr(LEFT_CLICK) && PointToRect(&point, &position, scalex, scaley))
  {
    //printf("left click \n");
    button_data->onClick(button);//call the on click function pointer
    //(button);
  }//if a left click release and still near the button
  else if (AEInputCheckReleased(LEFT_CLICK) && PointToRect(&point, &position, scalex, scaley))//if left click released
  {
    //printf("left click and released \n");
    button_data->onRelease(button);     //call the on release function pointer
    
  }//if collision
  else if (PointToRect(&point, &position, scalex, scaley))
  {
    //printf("mouse is over \n");
    //get the button data
    //Button *button_data = button->miscData; 
    button_data->onOver(button);   //call the on over function pointer
    //PlayButtonHoverAnimation(button); //
  }
  else
  {
    //printf("Mouse: %d  %d | sprite %f %f\ | diffY %f \n", mouse_x, mouse_y, x, y, winMaxY - winMinY);
  }
}
