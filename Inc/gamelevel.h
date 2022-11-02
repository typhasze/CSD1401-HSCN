#pragma once

void Game_Level_Init(void);

void Game_Level_Update(void);

void Game_Level_Exit(void);

void createPlatformXY(void);

void drawPlatform(void);

void HUD(void);

void scoreMultiplier(void);

void playerMovement(void);

void platformMovement(void);
int playerPlatformCollision(void);

void Clear_Fail_Pause(void);

//void purpleOrb(void);
//void yellowOrb(void);

void setOrbInitialPosition(void);
void makeOrbsFall(void);

void drawOrbs(void);
void orbOnFloor(void);
void orbsCollected(void);