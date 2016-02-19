#include "Button.h"


GameObject *CreateButton(PhysicsObject* _physics, Sprite* _sprite, Entity* _entity)
{
  GameObject *button;
  button = GameObjectCreate(_physics, _sprite, _entity, entity_button);


}