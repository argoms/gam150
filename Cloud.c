/*!
Project (working title): Epoch
\file   Cloud.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Contains implementation of clouds (background graphical effect)

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Cloud.h"
#include "MyRandom.h"
#include "AEEngine.h"
#include "Graphics.h"
#include "Vector2D.h"

typedef struct Cloud Cloud;
static float CLOUD_MAX_SPEED = 0.3;
static float DEPTH = 0.5; //used for parallax calculations
struct Cloud
{
  Vector2D direction;
  float lastCamX;
  float lastCamY;
};
/*!
\brief Attaches cloud component to misc data pointer and sets other gameobject flags to act more cloud-y (picking up random pugstar not included)
*/
void CloudInit(GameObject* instance)
{
  Cloud* cloud = malloc(sizeof(Cloud));

  
  instance->miscData = cloud;

  cloud->direction = Vec2(CLOUD_MAX_SPEED - (RandFloat() * CLOUD_MAX_SPEED * 2), CLOUD_MAX_SPEED - (RandFloat() * CLOUD_MAX_SPEED * 2));

  //set this to 0 since clouds shouldn't have a physics component 
  instance->syncSpritePhysics = 0;

  instance->simulate = &CloudSimulate;

  //instance->sprite->
   instance->sprite->y += 5000;
  instance->sprite->offset.y = -5000;
}
void CloudSimulate(GameObject* instance)
{
  GSortSprite(instance->sprite, 0);
  Cloud* cloud = (Cloud*)instance->miscData;
  
  //printf("CLOUD: %f, %f\n", cloud->direction.x, cloud->direction.y);
  //parallax faking

  //calculate the change in camera since last frame
  float deltaCamX;
  float deltaCamY;
  AEGfxGetCamPosition(&deltaCamX, &deltaCamY);

  deltaCamX -= cloud->lastCamX;
  deltaCamY -= cloud->lastCamY;

  //update last cam position
  AEGfxGetCamPosition(&cloud->lastCamX, &cloud->lastCamY);

  //update sprite position
  instance->sprite->x += cloud->direction.x + (deltaCamX * DEPTH);
  instance->sprite->y += cloud->direction.y + (deltaCamY * DEPTH);


}