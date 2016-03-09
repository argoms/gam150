/*!
\file   DamageNumbers.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Implementation for text popups when entities take damage.
*/
#include "GameObject.h"
#include "Text.h"
#include "DamageNumbers.h"
#include <stdio.h>
#include "Isometric.h"

extern double frameTime;
typedef struct DamageText DamageText;

/*!
\struct DamageText

Component specific to damage text popups.
*/
struct DamageText
{
  TextString* text; /**< Pointer to the damage text's text object*/
  int damage; /**< Amount of damage that was done*/
  double life; /**< lifetime before text disappears*/
};

/*!
\brief Creates a new damage text popup.

\param position World position to spawn object at (will be translated into screen position for object).
\param damage Amount of damage dealt (will break past 999999999)
\return Returns a pointer to the newly created object
*/
GameObject* DamageTextCreate(Vector2D position, int damage)
{
  char buffer[10];
  sprintf(buffer, "%d", damage);

  

  GameObject* damageText = GameObjectCreate(NULL, NULL, NULL, entity_damageText);
  damageText->syncSpritePhysics = 0;
  damageText->physics = NULL;
  damageText->sprite = NULL;
  damageText->entity = NULL;

  damageText->miscData = (DamageText*)malloc(sizeof(DamageText));

  DamageText* newObjectData = (DamageText*)(damageText->miscData);
  position = IsoWorldToScreen(&position);
  newObjectData->text = TextCreateString(buffer, position.x, position.y);
  newObjectData->life = 1;
  newObjectData->damage = damage;

  damageText->simulate = &DamageTextSimulate;
}

/*!
\brief Simulate function for damage text game objects.
\param instance Pointer to instance of object being simulated.
*/
void DamageTextSimulate(GameObject* instance)
{
  printf("3TEXTY5YOU");
  DamageText* instanceData = (DamageText*)(instance->miscData);
  TextStringAddOffset(instanceData->text, Vec2(0, 1));
  
  instanceData->life -= frameTime;
  if (instanceData->life < 0)
  {
    TextRemoveString(instanceData->text);
    GameObjectDestroy(&instance);
  }
}