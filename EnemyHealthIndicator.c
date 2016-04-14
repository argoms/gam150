/*!
Project (working title): Epoch
\file   EnemyHealthIndicator.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Functionality for displaying hearts representing enemy health above enemies.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EnemyHealthIndicator.h"


typedef struct HealthIndicatorComponent HealthIndicatorComponent;
struct HealthIndicatorComponent
{
  GameObject* enemyOwner; /**< owner to follow*/
  TextString* healthText; /**< pointer to stuff being displayed*/
  Vector2D offset;
};

static HealthIndicatorComponent* GetHIComponent(GameObject* input);

/*!
\brief Call to initialize the health bar attachment
*/
void AddEnemyHealthBar(GameObject* inst, Vector2D offset)
{
  GameObject* health = GameObjectCreate(0, 0, 0, entity_damageText);

  health->simulate = &HealthObjectSimulate;
  HealthIndicatorComponent* hpComponent = malloc(sizeof(HealthIndicatorComponent));
  health->syncSpritePhysics = 0;
  hpComponent->enemyOwner = inst;
  hpComponent->healthText = NULL;
  hpComponent->offset = offset;

  health->miscData = hpComponent;
}

/*!
\brief Updates the health display above the given inst
*/
void HealthObjectSimulate(GameObject* inst)
{
  if (!GetHIComponent(inst)->enemyOwner || GetHIComponent(inst)->enemyOwner->destroyFlag)
  {
    GameObjectDestroy(&inst);
    inst->simulate = NULL;
    TextRemoveString(GetHIComponent(inst)->healthText);
    return;
  }
  char hpstring[11] = "          ";
  int tempHP = GetHIComponent(inst)->enemyOwner->entity->health;
  int count = 0;
  
  //fill characters in buffer
  while (tempHP > 0 && count < 10) 
  {
    count++;
    hpstring[count] = 4;
    tempHP -= 1;
  }
  if (GetHIComponent(inst)->healthText)
  {
    TextRemoveString(GetHIComponent(inst)->healthText);
  }

  Vector2D writePosition = GetHIComponent(inst)->enemyOwner->physics->position;
  writePosition = IsoWorldToScreen(&writePosition);
  GetHIComponent(inst)->healthText = TextCreateString(hpstring, writePosition.x + GetHIComponent(inst)->offset.x, 
                                                      writePosition.y + GetHIComponent(inst)->offset.y);
}

/*!
\brief Getter for HealthIndicatorComponent misc data
*/
static HealthIndicatorComponent* GetHIComponent(GameObject* input)
{
  return ((HealthIndicatorComponent*)(input->miscData));
}