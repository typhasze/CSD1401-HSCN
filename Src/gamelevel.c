#include "cprocessing.h"
#include "gamelevel.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

CP_Image Bob, BobL, heart;
double static Bobx = 1280 / 2, Boby = 720 - 90.0f;
// Volatile Variables for Game 
int health, points, multiplier; float timer;
// Variables for Movement
int currentElapsedTime, jump, gravity;
float position, jumpposition, gravityposition;
// Pause and BobStagnant 
bool gIsPaused, BobDirection;

void Game_Level_Init() {
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(1280, 720);
	CP_Settings_TextSize(25.0);
	Bob = CP_Image_Load("Assets/Bob.png");
	BobL = CP_Image_Load("Assets/BobL.png");
	heart = CP_Image_Load("Assets/heart.png");
	timer = 60.0, health = 3, points = 0, multiplier = 1, gIsPaused = FALSE, BobDirection = FALSE; // Game Starts With Max HP, 60 Sec Timer, 0 Points, 1x Multiplier
}

void Game_Level_Update() {

	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	// Draw Bob @ Idle, Platform 
	{
		CP_Settings_ImageMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(0, CP_System_GetWindowHeight() - 50.0f, CP_System_GetWindowWidth() - 280, 50.0f);
		// Draw player when Idle
		(BobDirection == FALSE) ? CP_Image_Draw(Bob, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255)
			: CP_Image_Draw(BobL, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255);
	}
	
	gIsPaused = CP_Input_KeyTriggered(KEY_P) ? !gIsPaused : gIsPaused;	//Press P to Pause
	CP_Input_KeyTriggered(KEY_Q) ? CP_Engine_Terminate() : 0;			//Press Q to Terminate
	
	//TESTCODE
	{
		//TO REMOVE: Test Health Increment and Cap at 3
		CP_Input_KeyTriggered(KEY_1) && (health > 0) ? --health : NULL;	//CP_Input_KeyTriggered(KEY_1) can be replaced for collision w/ bomb
		CP_Input_KeyTriggered(KEY_2) && (health < 3) ? ++health : NULL;
		//TO REMOVE: GamePause when HP/TIMER reaches 0
		(health == 0 || timer <= 0.10) ? gIsPaused = TRUE : 0;
		//TO REMOVE: Testing scoreMultiplier works as points increase past a range.
	}

	switch (gIsPaused) {
	case TRUE: //Game is paused

		break;

	case FALSE: //Game not paused
		playerMovement();	//checks for Movement
		scoreMultiplier(points);	//Multiplier Logic
		timer -= CP_System_GetDt();	//Game Timer Reduction
		points += 5 * multiplier;	//Test Increase Points
		break;
	}
	HUD(); //Displays Timer, Score, Score Multiplier, Health Remaining
}

void Game_Level_Exit() {
	CP_Image_Free(&heart);
}


//Displays Timer, Score, Score Multiplier, Health Remaining
void HUD() {

	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	char Timer[10] = { 0 }, Points[50] = { 0 }, Multiplier[15] = { 0 };
	//Combine String and integer 
	sprintf_s(Timer, _countof(Timer), "Timer: %.0f", timer);
	sprintf_s(Points, _countof(Points), "Points: %i", points);
	sprintf_s(Multiplier, _countof(Multiplier), "Multiplier: %ix", multiplier);
	
	//Timer and Points Display
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, 0);
	CP_Font_DrawText(Timer, 5, 25);
	CP_Font_DrawText(Points, 5, 50);

	//Multiplier Display
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, 0);
	CP_Font_DrawText(Multiplier, 1270, 25);

	//Health (1 to 3 Heart Icons)
	for (int x = 1240, i = 0; i < health; i++, x -= 45) {
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(heart, x, 35, 30, 30, 255);
	}
}

//scoreMultiplier to insert consecutive positive pickups as combo value.
void scoreMultiplier(int combo) {
	multiplier = (combo < 1000) ? 1 : multiplier;
	multiplier = (combo >= 1000 && combo < 3000) ? 2 : multiplier;
	multiplier = (combo >= 3000 && combo < 6000) ? 3 : multiplier;
	multiplier = (combo >= 6000 && combo < 9000) ? 4 : multiplier;
	multiplier = (combo >= 9000) ? 5 : multiplier;
}

// Logic For Player Movement
void playerMovement() {
	currentElapsedTime = CP_System_GetDt() * 300;
	jump = CP_System_GetDt() * 10000;
	gravity = CP_System_GetDt() * 400;
	position = currentElapsedTime;
	jumpposition = jump;
	gravityposition = gravity;
	if (CP_Input_KeyDown(KEY_A))
	{
		if (Bobx > 40) {
			Bobx -= position;
			CP_Image_Draw(BobL, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255);
		}
		BobDirection = TRUE;
	}

	if (CP_Input_KeyDown(KEY_D))
	{
		if (Bobx < CP_System_GetWindowWidth() - 40) {
			Bobx += position;
		}
		BobDirection = FALSE;
	}
	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		Boby -= jump;

	}
	// Collision Section (Base)
	if (Bobx > 1040) {
		if (Boby < CP_System_GetWindowHeight() - 40) {
			Boby += position;
		}
	}

	if (Boby < CP_System_GetWindowHeight() - 90) {
		Boby += gravityposition;
	}
}

//Code to Generate Random Number; upper = maximum value, lower = min value;
int randNum() {
	int lower = 0, upper = 1920;
	int num = (rand() % (upper - lower + 1)) + lower;
}

