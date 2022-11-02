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

void TimerBar(void);

void purpleOrb(void);

void yellowOrb(void);

void setOrbInitialPosition();
void makeOrbsFall();
void drawOrbs();
void orbOnFloor();
void orbsCollected(void);