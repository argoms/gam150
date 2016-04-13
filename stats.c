#include "stats.h"
/*
typedef struct
{
int kills;                  // enemies killed
int deaths;                 // times died
int time;                   // in seconds
int rooms_cleared;          // #rooms cleared
int highest_level_reached;  // highest level reached
}statistics;
*/


statistics* InitializePlayerStats()
{
  statistics* stats = (statistics*)malloc(sizeof(statistics));  // malloc the staticsitc struct
                                                                // set everything to zero
  stats->kills = 0;
  stats->deaths = 0;
  stats->time = 0;
  stats->rooms_cleared = 0;
  stats->highest_level_reached = 0;
}

void IncrementKills(statistics* stats)
{
  if (stats)
  {
    stats->kills++;
  }
  else
  {
    return;
  }
}

void IncrementDeaths(statistics* stats)
{
  if (stats)
  {
    stats->deaths++;
  }
  else
  {
    return;
  }
}

void SetTime(statistics* stats, float elapsed_time)
{
  if (stats)
  {
    stats->time = elapsed_time;
  }
  else
  {
    return;
  }
}

void IncrementRoomsCleared(statistics* stats)
{
  if (stats)
  {
    stats->rooms_cleared++;
  }
  else
  {
    return;
  }
}

void Incrementhighest_level_reached(statistics* stats)
{
  if (stats)
  {
    stats->highest_level_reached++;
  }
  else
  {
    return;
  }
}

void ResetKills(statistics* stats)
{
  if (stats)
  {
    stats->kills = 0;
  }
  else
  {
    return;
  }
}

void ResetDeaths(statistics* stats)
{
  if (stats)
  {
    stats->deaths = 0;
  }
  else
  {
    return;
  }
}
void ResetRoomsCleared(statistics* stats)
{
  if (stats)
  {
    stats->highest_level_reached = 0;
  }
  else
  {
    return;
  }
}

void Resethighest_level_reached(statistics* stats)
{
  if (stats)
  {
    stats->highest_level_reached = 0;
  }
  else
  {
    return;
  }
}

void ResetTime(statistics* stats)
{
  if (stats)
  {
    stats->time = 0;
  }
  else
  {
    return;
  }
}

float GetSeconds(statistics* stats)
{
  if (stats)
  {
    return (int)(stats->time) % 60;
  }
  else
  {
    return 0;
  }
}

float GetMinutes(statistics* stats)
{
  if (stats)
  {
    return (stats->time) / 60;
  }
  else
  {
    return 0;
  }
}

float ResetStats(statistics* stats)
{
  if (stats)
  {
    stats->kills = 0;
    stats->deaths = 0;
    stats->time = 0;
    stats->rooms_cleared = 0;
    stats->highest_level_reached = 0;
  }
  else
  {
    return;
  }
}

void PrintStats(statistics* stats)
{
  if (stats == NULL)
  {
    return;
  }

  TextInit();
  // window size vars
  float winMaxX;
  float winMaxY;
  float winMinX;
  float winMinY;

  // get the window values
  winMaxX = AEGfxGetWinMaxX();
  winMaxY = AEGfxGetWinMaxY();
  winMinX = AEGfxGetWinMinX();
  winMinY = AEGfxGetWinMinY();

  // for my sanity
  float screenWidth = winMaxX - winMinX;
  float screenHeight = winMaxY - winMinY;

  TextString* stat_print_data;

  stat_print_data = TextCreateHUDString("Level 1", 0, 0);
  TextStringSetTint(stat_print_data, GTint(1, 1, 1, 1));


}

void FreeStats(statistics* stats)
{
  free(stats);
}