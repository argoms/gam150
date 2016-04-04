/*!
Project (working title): Epoch
\file   MapLevelSpawning.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Implementation for functions that actually create the gameobjects corresponding to the map tiledata.

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Vector2D.h"
#include "Isometric.h"
#include "Graphics.h"
#include <stdlib.h>
#include "MapGen.h"
#include "MyRandom.h"
#include "MapLevelSpawning.h"
#include "AEEngine.h"
#include "GameObject.h"
#include "Cloud.h"


enum EnvironmentalFeatures //a list of environmental features that appear in empty space
{
  environment_pillar,
  environment_block
};

void GenerateMapObjects()
{
  AEGfxSetBackgroundColor(0.5f, 0.1f, 0.5f);
  IsoMap* gameMap = GetGameMap();
  int i = 0; //index
  int j = 0; //index
  int mapWidth = gameMap->mapWidth;
  int mapHeight = gameMap->mapHeight;

  Animation* tileAnim = GCreateAnimation(1,
    GCreateTexture("isotile.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);
  Animation* tileAnim2 = GCreateAnimation(1,
    GCreateTexture("animations/world/pixelPath.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);

  Animation* floor = GCreateAnimation(1,
    GCreateTexture("animations/world/pixelFloor.png"),
    GCreateMesh(128.f, 128.f, 1, 1),
    1);

  Animation* wall = GCreateAnimation(1,
    GCreateTexture("animations/world/basicWall2.png"),
    GCreateMesh(512.f, 512.f, 1, 1),
    1);

  //playerSprite = GCreateSprite(0, 40, anim, 1);

  while (i < mapWidth)
  {
    j = 0;
    while (j < mapHeight)
    {
      if (IsoTileGet(i, j) == tile_wall)
      {
        Vector2D tilePos = Vec2(i, j);
        float tileX = IsoWorldToScreen(&tilePos).x;
        float tileY = IsoWorldToScreen(&tilePos).y;
        //printf("(%i, %i)", i, j);
        //GCreateSprite(tileX, tileY, tileAnim, 0);
        // Sprite* newObj = GCreateSprite(tileX, tileY + 105, wall, 0);
        //   newObj->offset.y = -9;
      }

      if (IsoTileGet(i, j) == tile_floor)
      {
        Vector2D tilePos = Vec2(i, j);
        float tileX = IsoWorldToScreen(&tilePos).x;
        float tileY = IsoWorldToScreen(&tilePos).y;
        //printf("(%i, %i)", i, j);
        Sprite* newObj = GCreateSprite(tileX, tileY + 80, floor, 0);
        newObj->offset.y = -112;
        newObj->tint.alpha = 0.9f;// (0.3 * RandFloat()) + 0.5;

        int i = 0;
        while (i < 4 && (i < 2 || RandFloat() < 0.6))
        {
          Vector2D extraOffset = Vec2(0, -16 * i);
          //extraOffset = IsoScreenToWorld(&extraOffset);

          //printf("(%i, %i)", i, j);
          Sprite* newObj = GCreateSprite(tileX + extraOffset.x, tileY + 96 + extraOffset.y, floor, 0);
          newObj->offset = extraOffset;
          newObj->offset.y -= 154;
          newObj->tint.alpha = (0.3 * RandFloat()) + 0.1;
          i++;
        }

      }

      //FOR DEBUG PURPOSES:
      if (IsoTileGet(i, j) == tile_path)
      {
        Vector2D tilePos = Vec2(i, j);
        float tileX = IsoWorldToScreen(&tilePos).x;
        float tileY = IsoWorldToScreen(&tilePos).y;
        //printf("(%i, %i)", i, j);
        Sprite* newObj = GCreateSprite(tileX, tileY + 96, tileAnim2, 0);
        newObj->tint.alpha = (0.3 * RandFloat()) + 0.7;
        newObj->offset.y = -96;
      }

      if (IsoTileGet(i, j) == tile_wall)
      {
        if (RandFloat() > 0.5f)
        {
          Vector2D tilePos = Vec2(i, j);
          int object = RandIntRange(0, 2);
          switch (object)
          {
          case environment_block:

           
            tilePos = Vec2(i, j);
            float tileX = IsoWorldToScreen(&tilePos).x;
            float tileY = IsoWorldToScreen(&tilePos).y;

            float heightOffset =  -2 + (RandFloat() * -16);

            //printf("(%i, %i)", i, j);
            Sprite* newObj = GCreateSprite(tileX, tileY + 80, floor, 0);
            newObj->offset.y = -112 + heightOffset * 16;
            newObj->tint.alpha = 0.9f;


            break;
          }
        }
        
        //CreateWorldGate(tilePos);
      }
      j++;
    }
    i++;
  }

  Animation* cloud = GCreateAnimation(1,
    GCreateTexture("animations/world/cloudTemplate.png"),
    GCreateMesh(384.f, 255.f, 1, 1),
    1);

  //making some clouds:
  i = 0;
  while (i++ < 256)
  {
    Vector2D randomPos = Vec2((0.5 - RandFloat()) * mapWidth, (0.5 - RandFloat()) * mapHeight);
    printf("pre-iso: %f, %f\n", randomPos.x, randomPos.y);
    randomPos = IsoWorldToScreen(&randomPos);
    //printf("new cloud: %f, %f\n", randomPos.x, randomPos.y);

    GameObject* cloudObject = GameObjectCreate(0, GCreateSprite(randomPos.x, randomPos.y, cloud, 0), 0, entity_cloud);
    cloudObject->sprite->tint.alpha = 0.1;
    CloudInit(cloudObject);
  }
}