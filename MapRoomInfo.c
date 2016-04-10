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
  //printf("======= %f, %f======= ROOM POSITION\n", cursor.x, cursor.y);
  roomData->numEnemies = 2;
  GameObject* newEnemy;
  newEnemy = EnemySpawn(cursor.x, cursor.y, ENEMY_TYPE_MELEE, GetPlayerObject());
  newEnemy->parent = roomData->parent;
  GameObject* yetAnotherEnemy;
  yetAnotherEnemy = EnemySpawn(cursor.x, cursor.y, ENEMY_TYPE_MELEE_BIG, GetPlayerObject());
  yetAnotherEnemy->parent = roomData->parent;
}

/*!
\brief Called by a small enemies room once it is opened
*/
void MapRoomBehavior_SmallRoom(MapRoom* roomData)
{
  Vector2D cursor = Vec2(roomData->position.x, roomData->position.y);
  //printf("======= %f, %f======= ROOM POSITION\n", cursor.x, cursor.y);
  roomData->numEnemies = 2;
  GameObject* newEnemy;
  newEnemy = EnemySpawn(cursor.x, cursor.y, ENEMY_TYPE_MELEE, GetPlayerObject());
  newEnemy->parent = roomData->parent;
  GameObject* yetAnotherEnemy;
  yetAnotherEnemy = EnemySpawn(cursor.x, cursor.y, ENEMY_TYPE_MELEE_BIG, GetPlayerObject());
  yetAnotherEnemy->parent = roomData->parent;
}