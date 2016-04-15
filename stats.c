/*!
Project (working title): Epoch
\file   stats.c
\author Tarrant Mangasarian
\par    email: tarrant.mangasarian\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
This file contains function implementations for player stats/end of level scoring
*/
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

/*************************************************************************/
/*!
\par   Function:  InitializePlayerStats

\par Description: inits the stats to 0 so no garabage

\return  pointer to a statisict struct
*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  IncrementKills

\par Description: increments the kills

\param - pointer to a statistic struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  IncrementDamageTaken

\par Description: increments the damage taken

\param - stats pointer to a statsistic struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  SetTime

\par Description: sets the time

\param stats - poointer to a statistic struct

\param elapsed_time - float of the time

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  IncrementRoomsCleared

\par Description: increments the rooms cleared

\param stats - pointer to a statistic struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  Incrementhighest_level_reached

\par Description: increments the highest level reached

\param  stats - pointer to a statistic struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  ResetKills

\par Description: resets the number of kills

\param stats - pointer to a ststistc struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  ResetDamageTaken

\par Description: resests the damage taken back to 0

\param stats - pointer to a statsic struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  ResetRoomsCleared

\par Description: resets the number of rooms cleared

\param stats - pointer to a statsistic struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function: Resethighest_level_reached

\par Description: resets the highest level reached back to 0

\param stats - pointer to a statistic struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  ResetTime

\par Description: Resets the time

\param stats - pointer to a statsistic struct

*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  GetSeconds

\par Description: Returns the number of seconds in the time, not raw
seconds

\param  stats - pointer to a statistic struct

\return the number of seconds
*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  GetMinutes

\par Description: returns the number of muntes in the time

\param  stats - pointer to a statistic struct

\return the number of minutes
*/
/*************************************************************************/
int GetMinutes(statistics* stats)
{
  if (stats)
  {
    return (int)((stats->time) / 60);
  }
  else
  {
    return 0;
  }
}

/*************************************************************************/
/*!
\par   Function:  GetRoomsCleared

\par Description: Returns the number of rooms cleared

\param  stats - pointer to a statsistic struct

\return the number of rooms cleared
*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function: GetHighestLevelReached

\par Description: Returns the highest level reached

\param stats - pointer to a statistic struct

\return the highest level reached
*/
/*************************************************************************/
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

/*************************************************************************/
/*!
\par   Function:  GetRawTime

\par Description: Gets the number of seconds in the stats,
like 70 seconds NOT 1 min 10 secs

\param stats - pointer to the statsistic struct

\return the number of seconds
*/
/*************************************************************************/
int GetRawTime(statistics* stats)
{
  if (stats)
  {
    return (int)(stats->time);
  }
  else
  {
    return 0;
  }
}

/*************************************************************************/
/*!
\par   Function:  ResetStats

\par Description: Resets the stats to 0

\param  stats - pointer to a statistic struct

\return 0 to show that it is now 0
*/
/*************************************************************************/
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
    return 0;
  }
  return 0;
}

/*************************************************************************/
/*!
\par   Function:   PrintStats

\par Description:  prints the stats, use in death screen, win or town screen

\param stats - pointe to statsitcs

*/
/*************************************************************************/
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
  score = ((stats->rooms_cleared) * (stats->rooms_cleared)) / ((stats->time) * (0.15f)) + (stats->kills) - (stats->damageTaken);

  // init buffers for the 
  char buffer_kills[BUFFER_SIZE];
  char buffer_damage_taken[BUFFER_SIZE];
  char buffer_time[BUFFER_SIZE];
  char buffer_rooms_cleared[BUFFER_SIZE];
  char buffer_highest_level_reached[BUFFER_SIZE];
  char buffer_overall_score[BUFFER_SIZE];

  // put the stats into buffers
  sprintf_s(&buffer_kills, (size_t)BUFFER_SIZE, "%d enemies killed", stats->kills);
  sprintf_s(&buffer_damage_taken, (size_t)BUFFER_SIZE, "%d damage taken", stats->damageTaken);
  sprintf_s(&buffer_time, (size_t)BUFFER_SIZE, "Time: %d\' %d\"", GetMinutes(stats), GetSeconds(stats));
  sprintf_s(&buffer_rooms_cleared, (size_t)BUFFER_SIZE, "%d rooms cleared", GetRoomsCleared(stats));
  sprintf_s(&buffer_highest_level_reached, (size_t)BUFFER_SIZE, "Highest Level: %d", GetHighestLevelReached(stats));
  sprintf_s(&buffer_overall_score, (size_t)BUFFER_SIZE, "Your score is %.1f", score);

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

/*************************************************************************/
/*!
\par   Function:  FreeStats

\par Description: frees the stats

\param stats - pointer to a statsitics struct

*/
/*************************************************************************/
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

