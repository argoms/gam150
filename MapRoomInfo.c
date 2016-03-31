/*!
Project (working title): Epoch
\file   MapRoomInfo.c
\author James Do
\par    email: j.do\@digipen.edu
\brief
Implementation for MapRoomInfo linked list objects containing information used during mapgen in MapGen.c

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/

#include "MapRoomInfo.h"
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
  MapRoomInfo* newNode = malloc(sizeof(MapRoomInfo));
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
  MapRoomInfo* indexTemp;

  while (index)
  {
    indexTemp = index;
    free(indexTemp);
    index = index->next;
  }
}