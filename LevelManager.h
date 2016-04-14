/*!
Project (working title): Epoch
\file   LevelManager.h
\author James Do
\par    email: j.do\@digipen.edu
\par    Course: GAM150
\par    Copyright © 2016 DigiPen (USA) Corporation.
\brief
Basic level/gamestate manager definitions.

*/
#pragma once



enum LevelStates
{
  level_splashScreen, /**< DigiPen splash screen        */
  level_mainMenu,     /**< main menu level              */
  level_level1,       /**< level 1 (in-game level       */
  level_level2,       /**< level 2 (in-game level       */
  level_level3,       /**< level 3 (in-game level       */
  level_level4,       /**< level 4 (in game level       */
  level_exit,         /**< closes the game              */
  level_town,         /**< town screen for intermission.*/
  level_deathScreen,  /**< end game loss/death screen   */
  level_winScreen,    /**< end game victory screen      */
  level_creditScreen, /**< credits screen               */
  level_controlScreen,/**< control screen               */
  level_selectScreen  /**< level select screen          */
};
void LevelLoad();
void LevelUnload();
void LevelRun();


void MainMenuInit();
void Level1Init();

void MainMenuRun();
void LevelSetNext(int _input);