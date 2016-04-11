#pragma once
typedef struct
{
  int kills;                  // enemies killed
  int deaths;                 // times died
  float time;                 // in seconds
  int rooms_cleared;          // #rooms cleared
  int highest_level_reached;  // highest level reached
}statistics;

statistics* InitializePlayerStats();