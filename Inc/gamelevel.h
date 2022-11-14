#pragma once
extern int level_selector;
void Game_Level_Init(void);

void Game_Level_Update(void);

void Game_Level_Exit(void);

void initializePlatform(int x);

void drawPlatform(void);

void HUD(void);

void scoreMultiplier(void);

void playerMovement(void);

void platformMovement(void);
int playerPlatformCollision(void);

void Clear_Fail_Pause(void);

//void purpleOrb(void);
//void yellowOrb(void);

void initializeOrbs(void);
void makeOrbsFall(void);

void drawOrbs(void);
void orbOnFloor(void);
void orbsCollected(void);
//void yellowOrb(void);

//void setOrbInitialPosition();
//void makeOrbsFall();
void drawOrbs();
void orbOnFloor();
int pointsCollected(int x);
void drawTreasureChest(void);

int ChestCollision(void);

void immunity(void);

void add_health(void);

void jump_high(void);

void move_fast(void);

void time_extension(void);

void power_up(void);
void textAbovePlayer(float x, float y, char* text);
void setText(char* text);
void drawBackground(void);
//Balancing Variables to change
void resetBomb(int i);
void resetyOrb(int i);
void resetpOrb(int i);
void start_timer(void);
void addStarsRating(void);
