/*!
Project (working title): Epoch
\file   MapRoomInfo.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Implementation for functionality on the map-room scope of map generation.
Includes MapRoomInfo linked list objects containing information used during mapgen in MapGen.c
All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/

#include "MapRoomInfo.h"
#include "MapGen.h"
#include <stdlib.h>
#include "GameLevel.h"
#include "Enemy.h"
#include <stdio.h>
#include "MyRandom.h"
#include "EnemySpawnEffect.h"

int ChooseEnemyType();

static int EnemiesPerRoom = 5;
/*
struct MapRoomInfo
{
Vector2D position;
int type;
MapRoomInfo* next;

};
*/

/*!
\brief Used to add another MapRoomInfo object to a list
\param first Pointer to first object in the linked list
*/
void MapRoomInfoAdd(MapRoomInfo* first, Vector2D position, int type)
{
  MapRoomInfo* newNode = (MapRoomInfo*)malloc(sizeof(MapRoomInfo));
  newNode->next = NULL;
  newNode->type = type;
  newNode->position = position;

  MapRoomInfo* index = first;

  
  if (!index) //if no first, set new node to first
  {
    first = newNode;
  } //otherwise, loop through list until end is found
  else
  {
    while (index->next)
    {
      index = index->next;
    }
    index->next = newNode;
    printf("nodeadded");
  }
}

/*!
\brief Frees list of maproominfos
*/
void MapRoomInfoClear(MapRoomInfo* first)
{
  MapRoomInfo* index = first;
  MapRoomInfo* indexTemp = index;

  //index through linked list and free links
  while (index)
  {
    indexTemp = index;
    
    index = index->next;
    free(indexTemp);
  }
}

/*!
\brief Called by a basic enemies room once it is opened
*/
void MapRoomBehavior_BasicEnemies(MapRoom* roomData)
{
  Vector2D cursor = Vec2(roomData->position.x, roomData->position.y);
  roomData->numEnemies = 0;

  //create a number of enemies according to the per room variable
  for (int i = 0; i < EnemiesPerRoom; i++)
  {
    //grab random positions within the room
    cursor.x += (((roomData->size - 4) / -2) + (RandFloat() * (roomData->size - 4)));
    cursor.y += (((roomData->size - 4) / -2) + (RandFloat() * (roomData->size - 4)));

    //if far enough from the player, position is valid and spawn an enemy
    if (PhysicsDistSQ(cursor, GetPlayerObject()->physics->position) > 4)
    {
      //initialize enemy:
      GameObject* newEnemy;
      roomData->numEnemies++;
      newEnemy = EnemySpawn(cursor.x, cursor.y, ChooseEnemyType(), GetPlayerObject());
      newEnemy->parent = roomData->parent;

      //create particle effect
      CreateEnemySpawnEffect(cursor, 16);
    } 
    else
    {
      //otherwise, keep looping
      i--;
    }
    cursor = Vec2(roomData->position.x, roomData->position.y);

  }
}

/*!
\brief Called by a small enemies room once it is opened
*/
void MapRoomBehavior_SmallRoom(MapRoom* roomData)
{
  Vector2D cursor = Vec2(roomData->position.x, roomData->position.y);
  roomData->numEnemies = 0;

  //create a number of enemies according to the per room variable
  for (int i = 0; i < EnemiesPerRoom / 2; i++)
  {
    //grab random positions within the room
    cursor.x += (((roomData->size - 4) / -4) + (RandFloat() * 0.5 * (roomData->size - 4)));
    cursor.y += (((roomData->size - 4) / -4) + (RandFloat() * 0.5 * (roomData->size - 4)));

    //if far enough from the player, position is valid and spawn an enemy
    if (PhysicsDistSQ(cursor, GetPlayerObject()->physics->position) > 4)
    {
      //initialize enemy:
      GameObject* newEnemy;
      roomData->numEnemies++;
      newEnemy = EnemySpawn(cursor.x, cursor.y, ChooseEnemyType(), GetPlayerObject());
      newEnemy->parent = roomData->parent;
      CreateEnemySpawnEffect(cursor, 16);
    }
    else
    {
      //otherwise, keep looping
      i--;
    }
    cursor = Vec2(roomData->position.x, roomData->position.y);

  }
}

/*!
\brief Chooses what type of enemy to spawn, with distributions of probabilities based on the level.
*/
int ChooseEnemyType()
{
  float randomNumber = RandFloat();

  if (randomNumber < 0.33f)
  {
    return ENEMY_TYPE_MELEE;
  }
  else if (randomNumber >= .33f && randomNumber <= .66f)
  {
    return ENEMY_TYPE_MELEE_BIG;
  }
  else
  {
    return ENEMY_TYPE_MELEE_CHARGE;
  }
}