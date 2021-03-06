/*!
Project (working title): Epoch
\file   MapLevelSpawning.c
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright � 2016 DigiPen (USA) Corporation.
\brief
Implementation for functions that actually create the gameobjects corresponding to the map tiledata.
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
#include <math.h>
#include "EnvironmentalEffects.h"
#include "LevelManager.h"
#include "Text.h"


extern int level;

//pointers to animation objects for various tile types:
Animation* tileAnim_floor;
Animation* tileAnim_wall;
Animation* tileAnim_path;

enum EnvironmentalFeatures //a list of environmental features that appear in empty space
{
  environment_pillar, //never was actually implemented :(
  environment_block
};

void MakeClouds();
static void MakeTutorialText();

void GenerateMapObjects()
{
  AEGfxSetBackgroundColor(0.5f, 0.1f, 0.5f);
  IsoMap* gameMap = GetGameMap();
  int i = 0; //index
  int j = 0; //index
  int mapWidth = gameMap->mapWidth;
  int mapHeight = gameMap->mapHeight;

  Animation* tileAnim = GCreateAnimation(1,
    GCreateTexture("animations/isotile.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);
  tileAnim_path = GCreateAnimation(1,
    GCreateTexture("animations/world/pixelPath.png"),
    GCreateMesh(128.f, 64.f, 1, 1),
    1);

  tileAnim_floor = GCreateAnimation(1,
    GCreateTexture("animations/world/pixelFloor.png"),
    GCreateMesh(128.f, 128.f, 1, 1),
    1);

  tileAnim_wall = GCreateAnimation(1,
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
        Vector2D tilePos = Vec2((float)i, (float)j);
        float tileX = IsoWorldToScreen(&tilePos).x;
        float tileY = IsoWorldToScreen(&tilePos).y;
        //printf("(%i, %i)", i, j);
        //GCreateSprite(tileX, tileY, tileAnim, 0);
        // Sprite* newObj = GCreateSprite(tileX, tileY + 105, wall, 0);
        //   newObj->offset.y = -9;
      }

      if (IsoTileGet(i, j) == tile_floor)
      {
        Vector2D tilePos = Vec2((float)i, (float)j);
        float tileX = IsoWorldToScreen(&tilePos).x;
        float tileY = IsoWorldToScreen(&tilePos).y;
        //printf("(%i, %i)", i, j);
        Sprite* newObj = GCreateSprite(tileX, tileY + 80, tileAnim_floor, 0);
        newObj->offset.y = -112;
        newObj->tint.alpha = 0.9f;// (0.3 * RandFloat()) + 0.5;


        //create a random number of blocks under the main floor block to create the impression of a thicker platform
        int i = 0;
        while (i < 4 && (i < 2 || RandFloat() < 0.6))
        {
          Vector2D extraOffset = Vec2(0, -16.f * i);
          //extraOffset = IsoScreenToWorld(&extraOffset);

          //printf("(%i, %i)", i, j);
          Sprite* newObj = GCreateSprite(tileX + extraOffset.x, tileY + 96 + extraOffset.y, tileAnim_floor, 0);
          newObj->offset = extraOffset;
          newObj->offset.y -= 154;
          newObj->tint.alpha = (0.3f * RandFloat()) + 0.1f;
          i++;
        }

      }

      //path spawning (flat tile)
      if (IsoTileGet(i, j) == tile_path)
      {
        Vector2D tilePos = Vec2((float)i, (float)j);
        float tileX = IsoWorldToScreen(&tilePos).x;
        float tileY = IsoWorldToScreen(&tilePos).y;
        //printf("(%i, %i)", i, j);
        Sprite* newObj = GCreateSprite(tileX, tileY + 96, tileAnim_path, 0);
        newObj->tint.alpha = (0.3f * RandFloat()) + 0.7f;
        newObj->offset.y = -96;
      }

      //wall "spawning" actually just spawns random environmental blocks since we're floating
      if (IsoTileGet(i, j) == tile_wall)
      {
        if (RandFloat() > 0.5f)
        {
          Vector2D tilePos = Vec2((float)i, (float)j);
          int object = RandIntRange(0, 2);
          switch (object)
          {
          case environment_block:

           
            tilePos = Vec2((float)i, (float)j);
            float tileX = IsoWorldToScreen(&tilePos).x;
            float tileY = IsoWorldToScreen(&tilePos).y;

            float heightOffset =  -2 + (RandFloat() * -16);

            //printf("(%i, %i)", i, j);
            Sprite* newObj = GCreateSprite(tileX, tileY + 80, tileAnim_floor, 0);
            newObj->offset.y = -112 + heightOffset * 16;
            newObj->tint.alpha = + 0.2f + (RandFloat() * 0.5f);


            break;
          }
        }
        
        //CreateWorldGate(tilePos);
      }
      j++;
    }
    i++;
  }

  MakeClouds();

  if (level == 1)
  {
    MakeTutorialText();
  }
}

/*!
\brief Makes the tutorial stuff on level 1
*/
static void MakeTutorialText()
{
  TextCreateString("WASD to move, click to attack", -300, 500);
  TextCreateString("Press space to dodge", -250, 450);
}

/*!
\brief Spawns a bunch of clouds under the level.
*/
void MakeClouds()
{

  IsoMap* gameMap = GetGameMap();
  int mapWidth = gameMap->mapWidth;
  int mapHeight = gameMap->mapHeight;

  Animation* cloud = GCreateAnimation(1,
    GCreateTexture("animations/world/cloudTemplate.png"),
    GCreateMesh(384.f, 255.f, 1, 1),
    1);

  //making some clouds:
  int i = 0;
  while (i++ < 256)
  {
    Vector2D randomPos = Vec2((0.5f - RandFloat()) * mapWidth, (0.5f - RandFloat()) * mapHeight);
    //printf("pre-iso: %f, %f\n", randomPos.x, randomPos.y);
    randomPos = IsoWorldToScreen(&randomPos);
    //printf("new cloud: %f, %f\n", randomPos.x, randomPos.y);

    GameObject* cloudObject = GameObjectCreate(0, GCreateSprite(randomPos.x, randomPos.y, cloud, 0), 0, entity_cloud);
    cloudObject->sprite->tint.alpha = 0.1f;
    CloudInit(cloudObject);
  }
}

/*!
\brief Spawns a vertical pillar (under the world, aesthetic only)
*/
/*
static void SpawnPillar(Vector2D tilePos)
{
  float tileX = tilePos.x;// IsoWorldToScreen(&tilePos).x;
  float tileY = tilePos.y;// IsoWorldToScreen(&tilePos).y;
  //printf("(%i, %i)", i, j);
  //Sprite* newObj = GCreateSprite(tileX, tileY + 80, tileAnim_floor, 0)
  //newObj->offset.y = -112;
  //newObj->tint.alpha = 0.9f;// (0.3 * RandFloat()) + 0.5;

  
  int i = 2;
  while (i < 64)
  {
    for (int j = 0; j < 9; j++)
    {
      if (1) //lol
      {
        //extraoffset acts as a Y offset, along with offsetting along a 3x3 square
        Vector2D extraOffset = Vec2(-1 + (j % 3), roundf(j / 3));
        //Vector2DScale(&extraOffset, &extraOffset, 16);
        extraOffset = IsoWorldToScreen(&extraOffset);

        //extraOffset.y -= (16 * i);

        //printf("(%i, %i)", i, j);
        Sprite* newObj = GCreateSprite(tileX + extraOffset.x, tileY + 256, tileAnim_floor, 0);
        newObj->offset = extraOffset;
        newObj->offset.y -= 256;
        newObj->tint.alpha = 1.f;

        GSortSprite(newObj, 0);
        i++;
      }
    }
  }
}*/