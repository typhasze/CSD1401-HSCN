#include "cprocessing.h"
#include "gamelevel.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include <string.h>
#include <stdlib.h>

// Volatile Variables for Game 
int timer, health, points, multiplier;
int gIsPaused;
CP_Image heart;

void Game_Level_Init() {
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(1280, 720);
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(25.0);
	heart = CP_Image_Load("Assets/heart.png");
	timer = 60 * 100, health = 3, points = 0, multiplier = 1;
	gIsPaused = FALSE;
}

void Game_Level_Update() {

	gIsPaused = CP_Input_KeyTriggered(KEY_P) ? !gIsPaused : gIsPaused; //Press P to Pause
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Input_KeyTriggered(KEY_Q) ? CP_Engine_Terminate() : 0; //Press Q to Terminate
	

	switch (gIsPaused) {
	case TRUE: //Game is paused

		break;

	case FALSE: //Game not paused
		timer -= CP_System_GetDt(); //Game Timer Reduction
		points += 10; //Test Increase Points
		break;
	}
	HUD(); //Displays Timer, Score, Score Multiplier, Health Remaining
}

void Game_Level_Exit() {
	CP_Image_Free(&heart);
}

//Code to Generate Random Number; upper = maximum value, lower = min value;
int randNum() {
	int lower = 0, upper = 1920;
	int num = (rand() % (upper - lower + 1)) + lower;
}

//Displays Timer, Score, Score Multiplier, Health Remaining
void HUD() {
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	char Timer[50] = { 0 }, Points[50] = { 0 }, Multiplier[15] = { 0 };
	//Combine string and integer 
	sprintf_s(Timer, _countof(Timer), "Timer: %i", timer/100);
	sprintf_s(Points, _countof(Points), "Points: %i", points);
	sprintf_s(Multiplier, _countof(Multiplier), "Multiplier: %ix", multiplier);
	//Timer and Points
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, 0);
	CP_Font_DrawText(Timer, 0, 25);
	CP_Font_DrawText(Points, 0, 50);
	//Multiplier
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, 0);
	CP_Font_DrawText(Multiplier, 1270, 25);
	//Health (1 to 3 Heart Icons)
	for (int x = 1240, i = 0; i < health; i++, x -= 45) {
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(heart, x, 35, 30, 30, 255);
	}

	//TESTCODE
	{
		//TO REMOVE: Test Health Increment and Cap at 3
		CP_Input_KeyTriggered(KEY_1) ? --health : NULL;
		CP_Input_KeyTriggered(KEY_2) && (health < 3) ? ++health : NULL;
		//TO REMOVE: Goes back to main menu when hp/timer reaches 0
		if (health == 0 || timer == 0)
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

}

