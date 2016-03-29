#include "Gate.h"
#include "GameObject.h"
#include "Isometric.h"
#include "GameLevel.h"
#include "Physics.h"
#include "MapGen.h"
#include "Text.h"

struct WorldGate
{
  int status;

  int positionX;
  int positionY;
};

/*!
\brief Creates a gate object in the world
*/
GameObject* CreateWorldGate(Vector2D position)
{
  Entity* gateEntity = malloc(sizeof(Entity));
  gateEntity->health = 5;

  Animation* anim2 = GCreateAnimation(1,
    GCreateTexture("isoTileGreen.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);
  GameObject* newGate = GameObjectCreate(PhysicsCreateObject(Vec2(position.x, position.y), 1), GCreateSprite(position.x, position.y, anim2, 1), gateEntity, entity_gate);
  
  newGate->simulate = NULL;
  newGate->entity->onEntityKilled = &GateOpened;

  newGate->miscData = (WorldGate*)malloc(sizeof(WorldGate));
  ((WorldGate*)(newGate->miscData))->positionX = position.x;
  ((WorldGate*)(newGate->miscData))->positionY = position.y;

  newGate->sprite->tint.alpha = 0.1f;

  printf("created a gate. %f, %f\n", position.x, position.y);
  return newGate;
  //IsoTileSet(position.x, position.y, 1);
}



/*!
\brief Killed the gate! D:
*/
void GateOpened(GameObject* DeadGate)
{
  IsoTileSet(((WorldGate*)(DeadGate->miscData))->positionX, ((WorldGate*)(DeadGate->miscData))->positionY, 0);
  GameObjectDestroy(&DeadGate);
}

/*!
\brief Called by dying enemies
*/
void GateRemoveEnemy(GameObject* Target)
{
  Target->entity->health--;
}

/*!
\brief Called on creating enemies
*/
void GateAddEnemy(GameObject* Target)
{
  Target->entity->health++;
}


/*!
\brief Simulate function for rooms that contain gates.
*/
void GateRoomSimulate(GameObject* instance)
{
  if (instance->type != entity_room)
  {
    printf("You've assigned gateroomsimulate badly. Oops.");
    abort();
  }

  int roomRadius = (GetRoomSize(instance) - 2) / 2;
  if (PhysicsDistSQ(GetPlayerObject()->physics->position, instance->physics->position) < roomRadius * roomRadius)
  {
    //clear the sim function on gameobject after one execution:
    instance->simulate = NULL;
    CloseRoom(instance);
    printf("\n \n *** \n room closed \n \n *** \n");
  }


}