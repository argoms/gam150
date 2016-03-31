#pragma once
typedef struct GameObject GameObject;

void GenerateMap(IsoMap* inputMap);
void OpenRoom(GameObject* room);
int GetRoomSize(GameObject* room);
void CloseRoom(GameObject* room);
void EnemyKilled(GameObject* room);
Animation* GetGateAnimation();