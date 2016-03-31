/*!
Project (working title): Epoch
\file   MapGen.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Declarations for interface for procedurally generating game levels.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
typedef struct GameObject GameObject;

void GenerateMap(IsoMap* inputMap);
void OpenRoom(GameObject* room);
int GetRoomSize(GameObject* room);
void CloseRoom(GameObject* room);
void EnemyKilled(GameObject* room);
Animation* GetGateAnimation();