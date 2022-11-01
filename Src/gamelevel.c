#include "cprocessing.h"
#include "gamelevel.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

CP_Image Bob, BobL, heart, fail_screen, clear_screen, pause_menu;
double Bobx, Boby;
int BobWidth, BobHeight;
// Volatile Variables for Game 
int health, points, multiplier, multiplierCombo; 
double gameTimer, multiplierTimer;
// Variables for Movement
int velocity, jump, gravity;
// Variables for Platform Creation
float platformX[100], platformY[100], platformWidth[100], platformHeight = 50.0f;
int no_of_platforms = 6;
static double maxJump = 0, jumpCD = 0;
// Pause and BobStagnant 
bool gIsPaused, BobDirection;

// Creating Platforms XY Positions, call drawPlatform() to render
void createPlatformXY() {
	//For Creating Random Platforms to Test
	for (int i = 1; i < no_of_platforms; i++) {
		platformX[0] = 400, platformY[0] = CP_System_GetWindowHeight() - 50.0f, platformWidth[0] = CP_System_GetWindowWidth() - 780;
		platformX[1] = 0, platformY[1] = CP_System_GetWindowHeight() - 200.0f, platformWidth[1] = CP_System_GetWindowWidth() - 780;
		platformX[2] = 950, platformY[2] = CP_System_GetWindowHeight() - 200.0f, platformWidth[2] = CP_System_GetWindowWidth() - 1030;
		platformX[3] = 750, platformY[3] = CP_System_GetWindowHeight() - 100.0f, platformWidth[3] = CP_System_GetWindowWidth() - 1030;
		platformX[4] = 850, platformY[4] = CP_System_GetWindowHeight() - 150.0f, platformWidth[4] = CP_System_GetWindowWidth() - 1030;
		platformX[5] = 400, platformY[5] = CP_System_GetWindowHeight() - 350.0f, platformWidth[5] = CP_System_GetWindowWidth() - 780;
	}
	// Format to Create a Platform, Set X, Y, Width
	//platformX[1] = 0; platformY[1] = 0; platformWidth[1] = 0;
}

void Game_Level_Init() {
	CP_System_SetFrameRate(60); CP_System_SetWindowSize(1280, 720); CP_Settings_TextSize(25.0);
	srand(3);
	Bob = CP_Image_Load("Assets/Bob.png"); BobL = CP_Image_Load("Assets/BobL.png");
	heart = CP_Image_Load("Assets/heart.png");
	fail_screen = CP_Image_Load("Assets/fail.png"); clear_screen = CP_Image_Load("Assets/clear.png"); pause_menu = CP_Image_Load("Assets/pause.png");
	BobWidth = CP_Image_GetWidth(Bob), BobHeight = CP_Image_GetHeight(Bob);
	//Resets Timer/Health/Points/Multiplier/Bob Position/Unpause Game
	gameTimer = 60.0, health = 3, points = 0, multiplier = 1, multiplierTimer = 5, multiplierCombo = 0;
	gIsPaused = FALSE, BobDirection = FALSE; Bobx = 1280 / 2, Boby = 720 / 2;
	//Base Platform
	createPlatformXY();
}

void Game_Level_Update() {
	//Main Code 
	{
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		//Draw Bob
		(BobDirection == FALSE) ? CP_Image_Draw(Bob, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255)
			: CP_Image_Draw(BobL, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255);
		//Press P to Pause
		//gIsPaused = CP_Input_KeyTriggered(KEY_P) ? !gIsPaused : gIsPaused;	
		//Press Q to Terminate
		CP_Input_KeyTriggered(KEY_Q) ? CP_Engine_Terminate() : 0;			

		//Conditions for Pausing the Game (Dying, Time finish, Player Pause game)
		gIsPaused = ((CP_Input_KeyTriggered(KEY_P)|| health == 0 || gameTimer <= 0.10 || gIsPaused == TRUE || Boby > 720)) ? TRUE : FALSE;
		//Rendering
		HUD(), drawPlatform();
		switch (gIsPaused) {
		case TRUE: //Game is paused
			Clear_Fail_Pause();
			break;

		case FALSE: //Game not paused
			playerMovement();			//checks input for Movement
			scoreMultiplier(points);	//Multiplier Logic
			gameTimer -= CP_System_GetDt();	//Game Timer Reduction
			break;
		}
	}
	//TESTCODE
	{
		//TO REMOVE: Test Health Increment and Cap at 3
		CP_Input_KeyTriggered(KEY_1) && (health > 0) ? --health : NULL;	//CP_Input_KeyTriggered(KEY_1) can be replaced for collision w/ bomb
		CP_Input_KeyTriggered(KEY_2) && (health < 3) ? ++health : NULL;
		CP_Input_KeyTriggered(KEY_3) ? multiplierTimer = 5.00, points += 1 * multiplier, multiplierCombo++ : multiplierTimer;
	}
}

void Game_Level_Exit() {
	CP_Image_Free(&heart);
}
//rendering createPlatformXY();
void drawPlatform() {
	CP_Settings_RectMode(CP_POSITION_CORNER);
	for (int i = 0; i < no_of_platforms; i++) {
		CP_Graphics_DrawRect(platformX[i], platformY[i], platformWidth[i], platformHeight);
	}
}

//Displays Timer, Score, Score Multiplier, Health Remaining
void HUD() {
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	char Timer[10] = { 0 }, Points[50] = { 0 }, Multiplier[15] = { 0 };
	//Combine String and integer 
	sprintf_s(Timer, _countof(Timer), "Timer: %.0f", gameTimer);
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

void playerPlatformCollision(void) {
	for (int i = 0; i < no_of_platforms; i++) {
		//unique bottom collision
		if (Boby + BobHeight + 5 >= platformY[i] && Boby + BobHeight - 5 <= platformY[i] &&
			Bobx + BobWidth > platformX[i] && Bobx < platformX[i] + platformWidth[i]) {
			//collision =  1;
			gravity = 0;
		}
		//unique top collision
		if (Boby - 10 <= platformY[i] + platformHeight && Boby + 10 >= platformY[i] + platformHeight &&
			Bobx + BobWidth > platformX[i] && Bobx < platformX[i] + platformWidth[i]) {
			//collision =  3;
			maxJump = 0;
		}
	}
}

int playerPlatformCollision2(int i) {
	int collision = 0;
	//Initial check on Bob position
	if (Boby + BobHeight > platformY[i] && Bobx + BobWidth > platformX[i] && Bobx < platformX[i] + platformWidth[i] && Boby < platformY[i] + platformHeight) {
		//unique left collision check
		if (Bobx < platformX[i] + platformWidth[i] && Bobx + BobWidth > platformX[i] + platformWidth[i]
			&& Bobx > platformX[i]) {
			return 1;
		}
		//unique right collision check
		if (Bobx + BobWidth > platformX[i] && Bobx + BobWidth < platformX[i] + platformWidth[i]
			&& Boby + BobHeight > platformY[i] && Bobx < platformX[i]) {
			return 2;
		}
	}
	return collision;
}

//scoreMultiplier Sets the Multiplier based on the game state.
void scoreMultiplier(void) {
	multiplierTimer -= CP_System_GetDt();
	if (multiplierTimer >= 0) {
		multiplier = (multiplierCombo < 5) ? 1 : multiplier;
		multiplier = (multiplierCombo >= 5 && multiplierCombo < 10) ? 2 : multiplier;
		multiplier = (multiplierCombo >= 10 && multiplierCombo < 20) ? 3 : multiplier;
		multiplier = (multiplierCombo >= 20 && multiplierCombo < 30) ? 4 : multiplier;
		multiplier = (multiplierCombo >= 30) ? 5 : multiplier;
	}
	else {
		multiplier = 1;
		multiplierCombo = 0;
	}
}

//Run to Check for Movement
void playerMovement() {
	velocity = CP_System_GetDt() * 300;
	jump = CP_System_GetDt() * 1500;
	gravity = CP_System_GetDt() * 500;
	static int jumpCounter = 2;
	playerPlatformCollision();
	if (CP_Input_KeyDown(KEY_A))
	{	
		if (Bobx > 0) {
			Bobx -= velocity;
		}
		BobDirection = TRUE;
	}

	if (CP_Input_KeyDown(KEY_D))
	{	
		if (Bobx < 1280 - BobWidth) {
			Bobx += velocity;
		}
		BobDirection = FALSE;
	}

	for (int i = 0; i < no_of_platforms; i++) {
		int collision = playerPlatformCollision2(i);
		//printf("collision is %d\n", collision);

		if (collision == 1) {
			Bobx = platformX[i] + platformWidth[i];
		}
		if (collision == 2) {
			Bobx = platformX[i] - BobWidth;
		}
	}

	//Jump Section
	{
		if (CP_Input_KeyTriggered(KEY_SPACE) && jumpCD <= 0 && jumpCounter != 0)
		{
			--jumpCounter;
			maxJump = 200;
			if (jumpCounter == 0) {
				jumpCD = 0.75;
				jumpCounter = 2;
			}

		}
		//Jump CD Decrement every deltaTime
		jumpCD -= (jumpCD >= 0) ? CP_System_GetDt() : jumpCD;
		if (maxJump > 0) {
			Boby -= jump;
			maxJump -= jump;
		}
		Boby += gravity;
	}
}

void Clear_Fail_Pause(void) {
	//Clear Condition
	if (gameTimer <= 0) {
		//TODO: SHOW MENU FOR CLEAR - CLEAR! => POINTS EARNED, HEALTH REMAINING, RETRY STAGE / GOTO NEXT STAGE
		CP_Image_Draw(clear_screen, 0, 0, CP_Image_GetWidth(clear_screen), CP_Image_GetHeight(clear_screen), 255);
		if (CP_Input_MouseClicked()) {
			if (isRectangleClicked(550, 360, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				Game_Level_Init();
				//TODO: LAUNCH NEW LEVEL
			}
			if (isRectangleClicked(550, 455, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				Game_Level_Init();
				//TODO: RESTART LEVEL
			}
		}
	}
	 
	//Fail Condition
	else if (health == 0 || Boby > 720) {
		//TODO: SHOW MENU FOR FAIL - YOU DIED => POINTS EARNED, 0 HEALTH, RETRY STAGE / GOTO NEXT STAGE 
		CP_Image_Draw(fail_screen, 0, 0, CP_Image_GetWidth(fail_screen), CP_Image_GetHeight(fail_screen), 255);
		if (CP_Input_MouseClicked()) {
			if (isRectangleClicked(550, 360, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				Game_Level_Init();
				//TODO: LAUNCH NEW LEVEL
			}
			if (isRectangleClicked(550, 455, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				Game_Level_Init();
				//TODO: RESTART LEVEL
			}
		}
	}
	//Game Pause
	else {
		CP_Image_Draw(pause_menu, 0, 0, CP_Image_GetWidth(pause_menu), CP_Image_GetHeight(pause_menu), 255);
		if (CP_Input_MouseClicked()) {
			//Retry
			if (IsCircleClicked(575, 330, 75, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				Game_Level_Init();
			}
			//Menu
			if (IsCircleClicked(684.5+37.5, 330, 75, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
			//Resume
			if (isRectangleClicked(540, 391, 200, 75, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
				gIsPaused = !gIsPaused;
		}
	}
}



