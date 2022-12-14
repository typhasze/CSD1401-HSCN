//---------------------------------------------------------
// gamelevel.h
// Code for all levels of the game
//
// authors:		 Hafiz
// contributors: Clement Chua, Scott Sia, Nicholas Theong
//
// Hafiz:
// -Score/Bonus Multiplier
// -User Interface
// -Visual Indications (Game timer start, powerups indicators)
// -Level Ratings
// -Clear/Fail/Pause Menu
// -Everything (helping with Integration)
// 
// Clement: 
// -Chest Placement
// -Power ups and Chest Collision
// -Sound Design
// -Level Design
// 
// Scott Sia:
// -Platform Placement 
// -Player Movement
// -Player Collision
// -Level Design
// -Platform Movement
//
// Nicholas:
// -Spawning and Despawning of Orbs and Bombs
// -Collision for Orbs and Bombs (collision with player and platform)
// and what happens after collision
// -Level Design
// -Bombs Exploding
// -Platform Placement
// 
//---------------------------------------------------------
// 
// Copyright(c) 2022 DigiPen Institute of Technology

#pragma once
extern int level_selector;
void Game_Level_Init(void);
void Game_Level_Update(void);
void Game_Level_Exit(void);

//Base Initiliaze
void initializePlatform(int x);
void initializeOrbs(void);

//Rendering
void drawBackground(void);
void drawPlatform(void);
void drawOrbs(void);
void HUD(void);

//Movement Logic
void playerMovement(void);
void platformMovement(void);
int playerPlatformCollision(void);
void scoreMultiplier(void);

void start_timer(void);
void Clear_Fail_Pause(void);
void addStarsRating(void);

//Orbs and Bombs Logic 
void makeOrbsFall(void);
void orbOnFloor(void);
void orbsCollected(void);
int pointsCollected(int x);

//Treasure Chest
void drawTreasureChest(void);
int ChestCollision(void);

//Power Ups Logic
void immunity(void);
void add_health(void);
void jump_high(void);
void move_fast(void);
void time_extension(void);
void power_up(void);

//Reset and Reinitialisize 
void resetBomb(int i);
void resetyOrb(int i);
void resetpOrb(int i);

//Utility
void textAbovePlayer(float x, float y, char* text);
void setText(char* text);


