/*!
Project (working title): Epoch
\file   Cloud.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Contains implementation of clouds (background graphical effect)
*/

#include "Cloud.h"
#include "MyRandom.h"
#include "AEEngine.h"
#include "Graphics.h"
#include "Vector2D.h"

typedef struct Cloud Cloud;

static float CLOUD_MAX_SPEED = 0.25; 
static float CLOUD_DEPTH_VARIANCE = 200;
static float DEPTH = 0.5; //used for parallax calculations

/*!
\struct Cloud
\brief Component attached to cloud gameobjects for cloud-specific behavior.
*/
struct Cloud
{
  Vector2D direction; /**< movement direction*/
  float lastCamX; /**< used for calculating delta camera pos*/
  float lastCamY; /**< used for calculating delta camera pos*/
};

/*!
\brief Attaches cloud component to misc data pointer and sets other gameobject flags to act more cloud-y (picking up random pugstar not included)

\param instance GameObject to attach cloud functionality to.
*/
void CloudInit(GameObject* instance)
{
  //create the cloud-specific component:
  Cloud* cloud = malloc(sizeof(Cloud));
  instance->miscData = cloud;

  //randomize cloud dir:
  cloud->direction = Vec2(CLOUD_MAX_SPEED - (RandFloat() * CLOUD_MAX_SPEED * 2), CLOUD_MAX_SPEED - (RandFloat() * CLOUD_MAX_SPEED * 2));

  //set cloud's last camera location to current camera location
  AEGfxGetCamPosition(&cloud->lastCamX, &cloud->lastCamY);

  //set this to 0 since clouds shouldn't have a physics component 
  instance->syncSpritePhysics = 0;
  
  //set the function that the cloud runs every frame
  instance->simulate = &CloudSimulate;

  //set offsets so clouds will render below everything else:
   instance->sprite->y += 5000 + RandFloat() * CLOUD_DEPTH_VARIANCE;
  instance->sprite->offset.y = -5000;

  
}

/*!
\brief Run on every cloud GameObject every frame.
\param GameObject* Pointer to instance being simulated.
*/
void CloudSimulate(GameObject* instance)
{
  GSortSprite(instance->sprite, 0);
  Cloud* cloud = (Cloud*)instance->miscData; //grab pointer to cloud component
  
  //printf("CLOUD: %f, %f\n", cloud->direction.x, cloud->direction.y);

  //parallax faking:

  //calculate the change in camera since last frame
  float deltaCamX;
  float deltaCamY;
  AEGfxGetCamPosition(&deltaCamX, &deltaCamY);

  deltaCamX -= cloud->lastCamX;
  deltaCamY -= cloud->lastCamY;

  //update last cam position
  AEGfxGetCamPosition(&cloud->lastCamX, &cloud->lastCamY);

  //update sprite position including slight adjustment for "parallax"
  instance->sprite->x += cloud->direction.x + (deltaCamX * DEPTH);
  instance->sprite->y += cloud->direction.y + (deltaCamY * DEPTH);


}