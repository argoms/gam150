/*!
\file   LevelManager.h
\author James Do
\par    email: j.do\@digipen.edu
\brief
Basic level/gamestate manager implementation.
*/
#pragma once

enum LevelStates {
  level_mainMenu, /**< main menu level*/
  level_level1, /**< level 1 (in-game level*/
  level_exit /**< closes the game*/
};
void LevelLoad();
void LevelUnload();
void LevelRun();


void MainMenuInit();
void Level1Init();