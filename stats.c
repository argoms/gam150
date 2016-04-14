/*
typedef struct
{
int kills;                  // enemies killed
int damageTaken;                 // times died
int time;                   // in seconds
int rooms_cleared;          // #rooms cleared
int highest_level_reached;  // highest level reached
}statistics;
*/
#include "stats.h"
#define BUFFER_SIZE 64

statistics* InitializePlayerStats()
{
  statistics* stats = (statistics*)malloc(sizeof(statistics));  // malloc the staticsitc struct
                                                                // set everything to zero
  stats->kills = 0;
  stats->damageTaken = 0;
  stats->time = 0;
  stats->rooms_cleared = 0;
  stats->highest_level_reached = 0;

  return stats;
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

void IncrementDamageTaken(statistics* stats)
{
  if (stats)
  {
    stats->damageTaken++;
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

void ResetDamageTaken(statistics* stats)
{
  if (stats)
  {
    stats->damageTaken = 0;
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

int GetSeconds(statistics* stats)
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

int GetMinutes(statistics* stats)
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

int GetRoomsCleared(statistics* stats)
{
  if (stats)
  {
    return (stats->rooms_cleared);
  }
  else
  {
    return 0;
  }
}

int GetHighestLevelReached(statistics* stats)
{
  if (stats)
  {
    return (stats->highest_level_reached);
  }
  else
  {
    return 0;
  }
}

int GetRawTime(statistics* stats)
{
  if (stats)
  {
    return (stats->time);
  }
  else
  {
    return 0;
  }
}

int ResetStats(statistics* stats)
{
  if (stats)
  {
    stats->kills = 0;
    stats->damageTaken = 0;
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

  //TextInit();
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
  float score = 0.0f;
  score = ((stats->rooms_cleared) * (stats->rooms_cleared)) / ((stats->time) * (0.15f)) + (stats->kills)  - (stats->damageTaken);

  // init buffers for the 
  char buffer_kills[BUFFER_SIZE];
  char buffer_damage_taken[BUFFER_SIZE];
  char buffer_time[BUFFER_SIZE];
  char buffer_rooms_cleared[BUFFER_SIZE];
  char buffer_highest_level_reached[BUFFER_SIZE];
  char buffer_overall_score[BUFFER_SIZE];
  
  // put the stats into buffers
  sprintf(&buffer_kills,"%d enemies killed", stats->kills);
  sprintf(&buffer_damage_taken, "%d damage taken", stats->damageTaken);
  sprintf(&buffer_time,"Time: %d\' %d\"", GetMinutes(stats), GetSeconds(stats));
  sprintf(&buffer_rooms_cleared,"%d rooms cleared", GetRoomsCleared(stats));
  sprintf(&buffer_highest_level_reached, "Highest Level: %d", GetHighestLevelReached(stats));
  sprintf(&buffer_overall_score, "Your score is %.3f", score);
  
  // print kills
  stat_print_data = TextCreateHUDString(buffer_kills, 0, 100);
  TextStringSetTint(stat_print_data, GTint(1, 1, 1, 1));

  //print damage taken
  stat_print_data = TextCreateHUDString(buffer_damage_taken, 0, 25);
  TextStringSetTint(stat_print_data, GTint(1, 1, 1, 1));

  // print data of rooms cleared
  stat_print_data = TextCreateHUDString(buffer_rooms_cleared, 0, -50);
  TextStringSetTint(stat_print_data, GTint(1, 1, 1, 1));

  // print time
  stat_print_data = TextCreateHUDString(buffer_time, 0, -125);
  TextStringSetTint(stat_print_data, GTint(1, 1, 1, 1));

  //print highest level
  //stat_print_data = TextCreateHUDString(buffer_highest_level_reached, 0, -125);
  //TextStringSetTint(stat_print_data, GTint(1, 1, 1, 1));

  //print score
  stat_print_data = TextCreateHUDString(buffer_overall_score, 0, -200);
  TextStringSetTint(stat_print_data, GTint(1, 1, 1, 1));
}

void FreeStats(statistics* stats)
{
  free(stats);
}

/*
typedef struct
{
int kills;                  // enemies killed
int damageTaken;                 // times died
int time;                   // in seconds
int rooms_cleared;          // #rooms cleared
int highest_level_reached;  // highest level reached
}statistics;
*/