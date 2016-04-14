#pragma once

//#include "AEEngine.h"
//#include <stdlib.h>
#include "GameLevel.h"
#include "Graphics.h"
#include "GameObject.h"
#include "Text.h"
#include  <stdio.h>
//#include "PlayerEntity.h"

typedef struct statistics statistics;
struct statistics
{
  int kills;                  // enemies killed
  int damageTaken;                 // times died
  float time;                 // in seconds
  int rooms_cleared;          // #rooms cleared
  int highest_level_reached;  // highest level reached
};

statistics* InitializePlayerStats();

void IncrementKills(statistics* stats);
void IncrementDamageTaken(statistics* stats);
void SetTime(statistics* stats, float elapsed_time);
void IncrementRoomsCleared(statistics* stats);
void Incrementhighest_level_reached(statistics* stats);

void ResetKills(statistics* stats);
void ResetDamageTaken(statistics* stats);
void ResetRoomsCleared(statistics* stats);
void Resethighest_level_reached(statistics* stats);
void ResetTime(statistics* stats);


int GetSeconds(statistics* stats);
int GetMinutes(statistics* stats);
int GetHighestLevelReached(statistics* stats);
int GetRoomsCleared(statistics* stats);
int GetRawTime(statistics* stats);

int ResetStats(statistics* stats);
void PrintStats(statistics* stats);
void FreeStats(statistics* stats);
