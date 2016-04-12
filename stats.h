#pragma once
//#include "Text.h"
#include "Button.h"
#include "AEEngine.h"
#include <stdlib.h>

typedef struct
{
  int kills;                  // enemies killed
  int deaths;                 // times died
  float time;                 // in seconds
  int rooms_cleared;          // #rooms cleared
  int highest_level_reached;  // highest level reached
}statistics;

statistics* InitializePlayerStats();

void IncrementKills(statistics* stats);
void IncrementDeaths(statistics* stats);
void SetTime(statistics* stats, float elapsed_time);
void IncrementRoomsCleared(statistics* stats);
void Incrementhighest_level_reached(statistics* stats);

void ResetKills(statistics* stats);
void ResetDeaths(statistics* stats);
void ResetRoomsCleared(statistics* stats);
void Resethighest_level_reached(statistics* stats);
void ResetTime(statistics* stats);


float GetSeconds(statistics* stats);
float GetMinutes(statistics* stats);
float ResetStats(statistics* stats);

void FreeStats(statistics* stats);