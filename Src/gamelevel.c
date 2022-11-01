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
//for balls
static int purpleBalls[3];
static int yellowBalls[3];

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
	CP_System_SetFrameRate(60); CP_System_SetWindowSize(1280, 720); CP_Settings_TextSize(25.0); srand(3);
	Bob = CP_Image_Load("Assets/Bob.png"); BobL = CP_Image_Load("Assets/BobL.png");
	heart = CP_Image_Load("Assets/heart.png");
	fail_screen = CP_Image_Load("Assets/fail.png"); clear_screen = CP_Image_Load("Assets/clear.png"); pause_menu = CP_Image_Load("Assets/pause.png");
	BobWidth = CP_Image_GetWidth(Bob), BobHeight = CP_Image_GetHeight(Bob);
	//Resets Timer/Health/Points/Multiplier/Bob Position/Unpause Game
	gameTimer = 60.0, health = 3, points = 0, multiplier = 1, multiplierTimer = 5, multiplierCombo = 0;
	gIsPaused = FALSE, BobDirection = FALSE; Bobx = 1280 / 2, Boby = 720 / 2;
	//Base Platform
	createPlatformXY();
	for (int i = 0; i < 3; i++)
	{
		purpleBalls[i] = (rand() % (1200 + 1 - 25) + 25);
	}

	for (int i = 0; i < 3; i++)
	{
		yellowBalls[i] = (rand() % (1200 + 1 - 25) + 25);
	}
}

void Game_Level_Update() {
	//Main Code 
	{
		//Press P to Pause
		gIsPaused = CP_Input_KeyTriggered(KEY_P) ? !gIsPaused : gIsPaused;
		//Conditions for Pausing the Game (Dying, Time finish, Player Pause game)
		gIsPaused = (health == 0 || gameTimer <= 0.10 || gIsPaused == TRUE || Boby > 720) ? TRUE : FALSE;
		//Press Q to Terminate
		CP_Input_KeyTriggered(KEY_Q) ? CP_Engine_Terminate() : 0;


		//Draw Bob
		(BobDirection == FALSE) ? CP_Image_Draw(Bob, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255)
			: CP_Image_Draw(BobL, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255);
		//Rendering
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255)), HUD(), drawPlatform();

		switch (gIsPaused) {
		case TRUE: //Game is paused
			Clear_Fail_Pause();
			break;

		case FALSE: //Game not paused
			playerMovement();			//checks input for Movement
			scoreMultiplier();	//Multiplier Logic
			gameTimer -= CP_System_GetDt();	//Game Timer Reduction
			yellowOrb();
			purpleOrb();
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
void pointsCollected(int x) {
	multiplierTimer = 5.00, points += x * multiplier, multiplierCombo++;
}
void Game_Level_Exit() {
	CP_Image_Free(&heart);
}
//rendering createPlatformXY();
void drawPlatform() {
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Settings_RectMode(CP_POSITION_CORNER);
	for (int i = 0; i < no_of_platforms; i++) {
		CP_Graphics_DrawRect(platformX[i], platformY[i], platformWidth[i], platformHeight);
	}
}

//Displays Timer, Score, Score Multiplier, Health Remaining
void HUD() {

	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	char Timer[10] = { 0 }, Points[50] = { 0 }, Multiplier[15] = { 0 };
	CP_Settings_TextSize(30);
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

	//Multiplier Timer Bar
	static float x;
	x = multiplierTimer * 100;
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(CP_Color_Create(145, 224, 255, 255));
	CP_Graphics_DrawRectAdvanced(1280 / 2, 35, x, 35, 0, 15);
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
	//Only Decrease if Timer > 0
	(multiplierTimer > 0) ? multiplierTimer -= CP_System_GetDt() : multiplier;
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
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	char Points[50] = { 0 };
	sprintf_s(Points, _countof(Points), "%i", points);
	CP_Settings_TextSize(75); CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//Clear Condition
	if (gameTimer <= 0.10) {
		//TODO: SHOW MENU FOR CLEAR - CLEAR! => POINTS EARNED, HEALTH REMAINING, RETRY STAGE / GOTO NEXT STAGE
		CP_Image_Draw(clear_screen, 0, 0, CP_Image_GetWidth(clear_screen), CP_Image_GetHeight(clear_screen), 255);
		CP_Font_DrawText(Points, CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() / 2 - 45);
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
		CP_Font_DrawText(Points, CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() / 2 - 45);
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
			if (IsCircleClicked(684.5 + 37.5, 330, 75, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
			//Resume
			if (isRectangleClicked(540, 391, 200, 75, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
				gIsPaused = !gIsPaused;
		}
	}
}

void purpleOrb()
{
	static int sec = 0;
	static int drop = 0; //
	static int drop2 = 0;//for purple orbs
	static int drop3 = 0;//

	sec = CP_System_GetSeconds();
	CP_Color purple = CP_Color_Create(255, 0, 255, 255);
	if ((sec % 2 == 0 && sec != 0) || drop == 1)
	{
		drop = 1;
		static int time = 0;
		CP_Settings_Fill(purple);
		int i = 0;
		int collision = 0;
		//left of ball, right of bob
		if (((((purpleBalls[0] - 25) <= (Bobx + BobWidth)) && (purpleBalls[0] + 25) >= (Bobx + BobWidth)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop = 0;
			purpleBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}
		//right of ball, left of bob
		if (((((purpleBalls[0] + 25) >= (Bobx)) && (purpleBalls[0] - 25) <= (Bobx)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop = 0;
			purpleBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}
		//bottom of ball, top of bob
		if (((((purpleBalls[0]) >= (Bobx)) && (purpleBalls[0]) <= (Bobx + BobWidth)) && ((time + 25 >= Boby) && (time + 25 < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop = 0;
			purpleBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}

		//top of ball, bottom of bob
		if (((((purpleBalls[0]) >= (Bobx)) && (purpleBalls[0]) <= (Bobx + BobWidth)) && ((time - 25 <= Boby + BobHeight) && (time - 25 >= Boby))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop = 0;
			purpleBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}

		for (i; i < no_of_platforms; i++)
		{
			if ((purpleBalls[0] > platformX[i] && purpleBalls[0] < (platformX[i] + platformWidth[i])) && ((time + 25) >= platformY[i]))
			{
				collision = 1;
			}
		}

		if (collision == 1)
		{
			CP_Graphics_DrawCircle(purpleBalls[0], time, 50.0f);
			//printf("Time: %d\n", time);
			float currentElapsedTime = CP_System_GetDt();
			static float totalElapsedTime = 0;
			totalElapsedTime += currentElapsedTime;
			printf("Time: %f\n", totalElapsedTime);
			//printf("In touch\n");
			if ((int)totalElapsedTime == 2)
			{
				drop = 0;
				purpleBalls[0] = (rand() % (1200 + 1 - 25) + 25);
				time = 0;
				totalElapsedTime = 0;
			}
		}

		else if (time == 720)
		{
			drop = 0;
			purpleBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;

		}

		else
		{
			CP_Graphics_DrawCircle(purpleBalls[0], time += 3, 50.0f);
		}

	}

	if ((sec % 4 == 0 && sec != 0) || drop2 == 1)
	{
		drop2 = 1;
		static int time = 0;
		CP_Settings_Fill(purple);
		int i = 0;
		int collision = 0;
		//left of ball, right of bob
		if (((((purpleBalls[1] - 25) <= (Bobx + BobWidth)) && (purpleBalls[1] + 25) >= (Bobx + BobWidth)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop2 = 0;
			purpleBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}
		//right of ball, left of bob
		if (((((purpleBalls[1] + 25) >= (Bobx)) && (purpleBalls[1] - 25) <= (Bobx)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop2 = 0;
			purpleBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}
		//bottom of ball, top of bob
		if (((((purpleBalls[1]) >= (Bobx)) && (purpleBalls[1]) <= (Bobx + BobWidth)) && ((time + 25 >= Boby) && (time + 25 < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop2 = 0;
			purpleBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}

		//top of ball, bottom of bob
		if (((((purpleBalls[1]) >= (Bobx)) && (purpleBalls[1]) <= (Bobx + BobWidth)) && ((time - 25 <= Boby + BobHeight) && (time - 25 >= Boby))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop2 = 0;
			purpleBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}
		for (i; i < no_of_platforms; i++)
		{
			if ((purpleBalls[1] > platformX[i] && purpleBalls[1] < (platformX[i] + platformWidth[i])) && ((time + 25) >= platformY[i]))
			{
				collision = 1;
			}
		}

		if (collision == 1)
		{
			CP_Graphics_DrawCircle(purpleBalls[1], time, 50.0f);
			//printf("Time: %d\n", time);
			float currentElapsedTime = CP_System_GetDt();
			static float totalElapsedTime = 0;
			totalElapsedTime += currentElapsedTime;
			printf("Time: %f\n", totalElapsedTime);
			//printf("In touch\n");
			if ((int)totalElapsedTime == 2)
			{
				drop2 = 0;
				purpleBalls[1] = (rand() % (1200 + 1 - 25) + 25);
				time = 0;
				totalElapsedTime = 0;
			}
		}

		else if (time == 720)
		{
			drop2 = 0;
			purpleBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;

		}

		else
		{
			CP_Graphics_DrawCircle(purpleBalls[1], time += 3, 50.0f);
		}

	}

	if ((sec % 8 == 0 && sec != 0) || drop3 == 1)
	{
		drop3 = 1;
		static int time = 0;
		CP_Settings_Fill(purple);
		int i = 0;
		int collision = 0;
		//left of ball, right of bob
		if (((((purpleBalls[2] - 25) <= (Bobx + BobWidth)) && (purpleBalls[2] + 25) >= (Bobx + BobWidth)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop3 = 0;
			purpleBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}
		//right of ball, left of bob
		if (((((purpleBalls[2] + 25) >= (Bobx)) && (purpleBalls[2] - 25) <= (Bobx)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop3 = 0;
			purpleBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}
		//bottom of ball, top of bob
		if (((((purpleBalls[2]) >= (Bobx)) && (purpleBalls[2]) <= (Bobx + BobWidth)) && ((time + 25 >= Boby) && (time + 25 < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop3 = 0;
			purpleBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}

		//top of ball, bottom of bob
		if (((((purpleBalls[2]) >= (Bobx)) && (purpleBalls[2]) <= (Bobx + BobWidth)) && ((time - 25 <= Boby + BobHeight) && (time - 25 >= Boby))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop3 = 0;
			purpleBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(5);
		}
		for (i; i < no_of_platforms; i++)
		{
			if ((purpleBalls[2] > platformX[i] && purpleBalls[2] < (platformX[i] + platformWidth[i])) && ((time + 25) >= platformY[i]))
			{
				collision = 1;
			}
		}

		if (collision == 1)
		{
			CP_Graphics_DrawCircle(purpleBalls[2], time, 50.0f);
			//printf("Time: %d\n", time);
			float currentElapsedTime = CP_System_GetDt();
			static float totalElapsedTime = 0;
			totalElapsedTime += currentElapsedTime;
			printf("Time: %f\n", totalElapsedTime);
			//printf("In touch\n");
			if ((int)totalElapsedTime == 2)
			{
				drop3 = 0;
				purpleBalls[2] = (rand() % (1200 + 1 - 25) + 25);
				time = 0;
				totalElapsedTime = 0;
			}
		}

		else if (time == 720)
		{
			drop3 = 0;
			purpleBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;

		}

		else
		{
			CP_Graphics_DrawCircle(purpleBalls[2], time += 3, 50.0f);
		}
	}
}

void yellowOrb()
{
	static int sec = 0;
	static int drop = 0; //
	static int drop2 = 0;//for yellow orbs
	static int drop3 = 0;//

	sec = CP_System_GetSeconds();
	CP_Color yellow = CP_Color_Create(255, 255, 0, 255);
	if ((sec % 3 == 0 && sec != 0) || drop == 1)
	{
		drop = 1;
		static int time = 0;
		CP_Settings_Fill(yellow);
		int i = 0;
		int collision = 0;
		//left of ball, right of bob
		if (((((yellowBalls[0] - 25) <= (Bobx + BobWidth)) && (yellowBalls[0] + 25) >= (Bobx + BobWidth)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop = 0;
			yellowBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}
		//right of ball, left of bob
		if (((((yellowBalls[0] + 25) >= (Bobx)) && (yellowBalls[0] - 25) <= (Bobx)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop = 0;
			yellowBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}
		//bottom of ball, top of bob
		if (((((yellowBalls[0]) >= (Bobx)) && (yellowBalls[0]) <= (Bobx + BobWidth)) && ((time + 25 >= Boby) && (time + 25 < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop = 0;
			yellowBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}

		//top of ball, bottom of bob
		if (((((yellowBalls[0]) >= (Bobx)) && (yellowBalls[0]) <= (Bobx + BobWidth)) && ((time - 25 <= Boby + BobHeight) && (time - 25 >= Boby))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop = 0;
			yellowBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}

		for (i; i < no_of_platforms; i++)
		{
			if ((yellowBalls[0] > platformX[i] && yellowBalls[0] < (platformX[i] + platformWidth[i])) && ((time + 25) >= platformY[i]))
			{
				collision = 1;
			}
		}

		if (collision == 1)
		{
			CP_Graphics_DrawCircle(yellowBalls[0], time, 50.0f);
			//printf("Time: %d\n", time);
			float currentElapsedTime = CP_System_GetDt();
			static float totalElapsedTime = 0;
			totalElapsedTime += currentElapsedTime;
			printf("Time: %f\n", totalElapsedTime);
			//printf("In touch\n");
			if ((int)totalElapsedTime == 3)
			{
				drop = 0;
				yellowBalls[0] = (rand() % (1200 + 1 - 25) + 25);
				time = 0;
				totalElapsedTime = 0;
			}
		}

		else if (time == 720)
		{
			drop = 0;
			yellowBalls[0] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;

		}

		else
		{
			CP_Graphics_DrawCircle(yellowBalls[0], time += 5, 50.0f);
		}

	}

	if ((sec % 6 == 0 && sec != 0) || drop2 == 1)
	{
		drop2 = 1;
		static int time = 0;
		CP_Settings_Fill(yellow);
		int i = 0;
		int collision = 0;
		//left of ball, right of bob
		if (((((yellowBalls[1] - 25) <= (Bobx + BobWidth)) && (yellowBalls[1] + 25) >= (Bobx + BobWidth)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop2 = 0;
			yellowBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}
		//right of ball, left of bob
		if (((((yellowBalls[1] + 25) >= (Bobx)) && (yellowBalls[1] - 25) <= (Bobx)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop2 = 0;
			yellowBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}
		//bottom of ball, top of bob
		if (((((yellowBalls[1]) >= (Bobx)) && (yellowBalls[1]) <= (Bobx + BobWidth)) && ((time + 25 >= Boby) && (time + 25 < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop2 = 0;
			yellowBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}

		//top of ball, bottom of bob
		if (((((yellowBalls[1]) >= (Bobx)) && (yellowBalls[1]) <= (Bobx + BobWidth)) && ((time - 25 <= Boby + BobHeight) && (time - 25 >= Boby))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop2 = 0;
			yellowBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}
		for (i; i < no_of_platforms; i++)
		{
			if ((yellowBalls[1] > platformX[i] && yellowBalls[1] < (platformX[i] + platformWidth[i])) && ((time + 25) >= platformY[i]))
			{
				collision = 1;
			}
		}

		if (collision == 1)
		{
			CP_Graphics_DrawCircle(yellowBalls[1], time, 50.0f);
			//printf("Time: %d\n", time);
			float currentElapsedTime = CP_System_GetDt();
			static float totalElapsedTime = 0;
			totalElapsedTime += currentElapsedTime;
			printf("Time: %f\n", totalElapsedTime);
			//printf("In touch\n");
			if ((int)totalElapsedTime == 3)
			{
				drop2 = 0;
				yellowBalls[1] = (rand() % (1200 + 1 - 25) + 25);
				time = 0;
				totalElapsedTime = 0;
			}
		}

		else if (time == 720)
		{
			drop2 = 0;
			yellowBalls[1] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;

		}

		else
		{
			CP_Graphics_DrawCircle(yellowBalls[1], time += 5, 50.0f);
		}

	}

	if ((sec % 9 == 0 && sec != 0) || drop3 == 1)
	{
		drop3 = 1;
		static int time = 0;
		CP_Settings_Fill(yellow);
		int i = 0;
		int collision = 0;
		//left of ball, right of bob
		if (((((yellowBalls[2] - 25) <= (Bobx + BobWidth)) && (yellowBalls[2] + 25) >= (Bobx + BobWidth)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop3 = 0;
			yellowBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}
		//right of ball, left of bob
		if (((((yellowBalls[2] + 25) >= (Bobx)) && (yellowBalls[2] - 25) <= (Bobx)) && ((time >= Boby) && (time < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop3 = 0;
			yellowBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}
		//bottom of ball, top of bob
		if (((((yellowBalls[2]) >= (Bobx)) && (yellowBalls[2]) <= (Bobx + BobWidth)) && ((time + 25 >= Boby) && (time + 25 < Boby + BobHeight))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop3 = 0;
			yellowBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}

		//top of ball, bottom of bob
		if (((((yellowBalls[2]) >= (Bobx)) && (yellowBalls[2]) <= (Bobx + BobWidth)) && ((time - 25 <= Boby + BobHeight) && (time - 25 >= Boby))))
		{
			//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			//CP_Graphics_DrawCircle(100, 100, 50.0f);
			drop3 = 0;
			yellowBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;
			pointsCollected(10);
		}
		for (i; i < no_of_platforms; i++)
		{
			if ((yellowBalls[2] > platformX[i] && yellowBalls[2] < (platformX[i] + platformWidth[i])) && ((time + 25) >= platformY[i]))
			{
				collision = 1;
			}
		}

		if (collision == 1)
		{
			CP_Graphics_DrawCircle(yellowBalls[2], time, 50.0f);
			//printf("Time: %d\n", time);
			float currentElapsedTime = CP_System_GetDt();
			static float totalElapsedTime = 0;
			totalElapsedTime += currentElapsedTime;
			printf("Time: %f\n", totalElapsedTime);
			//printf("In touch\n");
			if ((int)totalElapsedTime == 3)
			{
				drop3 = 0;
				yellowBalls[2] = (rand() % (1200 + 1 - 25) + 25);
				time = 0;
				totalElapsedTime = 0;
			}
		}

		else if (time == 720)
		{
			drop3 = 0;
			yellowBalls[2] = (rand() % (1200 + 1 - 25) + 25);
			time = 0;

		}

		else
		{
			CP_Graphics_DrawCircle(yellowBalls[2], time += 5, 50.0f);
		}
	}
}

