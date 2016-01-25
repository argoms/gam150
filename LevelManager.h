#pragma once

enum LevelStates {
  level_mainMenu,
  level_level1,
  level_exit
};
void LevelLoad();
void LevelUnload();
void LevelRun();


void MainMenuInit();
void Level1Init();