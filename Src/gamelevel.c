#include "cprocessing.h"
#include "gamelevel.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "animations.h"

CP_Image Bob, BobL, heart, chest, Bomb, fail_screen, clear_screen, pause_menu, purple_orb, hudbackplate,
yellow_orb, particle, jumpParticle, bombPic, forest, volcano, stars, picPlatform , mainMenu, banner;
CP_Sound chestopen, explosion, orb, gameover;
// Bob Variables
double Bobx, Boby;
int BobWidth, BobHeight;
// Volatile Variables for Game 
int health, points, multiplier, multiplierCombo;
double gameTimer, multiplierTimer, immune_timer;
static char playeronground = 0;
// Variables for Movement by powerUps
float maxJumpHeight = 175, speedMultiplier = 1;

// Variables for Platform Creation
//int level_selector;	//TODO: REMOVE = 1 WHEN MAIN MENU DONE
float platformX[100], platformY[100], platformWidth[100], platformHeight = 50.0f;
int no_of_platforms = 6;

// Pause and BobStagnant 
bool gIsPaused, BobDirection, BobImmune = 0;

//For Chest
int cheststate = 1;
static int chestX, chestY;
int static soundCheck = 0; //sfx for bomb 
int power = 0;

//Text
float textTimer = 0;
char textToShow[50] = {"test"};

int update_hp;
int* hp = &update_hp;
float starting_timer;

//Can Be Used for Chest, Bomb, Orbs
struct Items
{
	int x, y;
	float timer_to_drop, timer_on_floor, dropSpeed;
};
struct Items pOrbs[10], yOrbs[10], bOrbs[10];

//Initialize All Platforms for Selected Level
void initializePlatform(int level) {
	memset(platformX, 0, sizeof(platformX));
	memset(platformY, 0, sizeof(platformY));
	memset(platformWidth, 0, sizeof(platformWidth));
	if (level == 1) {
		no_of_platforms = 4 + 1;
		platformX[0] = 400, platformY[0] = CP_System_GetWindowHeight() - 50.0f, platformWidth[0] = CP_System_GetWindowWidth() - 900.f;
		platformX[1] = 950, platformY[1] = CP_System_GetWindowHeight() - 50.0f, platformWidth[1] = CP_System_GetWindowWidth() - 1000.f;
		platformX[2] = 0, platformY[2] = CP_System_GetWindowHeight() - 50.0f, platformWidth[2] = CP_System_GetWindowWidth() - 1000.f;
		platformX[3] = 300, platformY[3] = CP_System_GetWindowHeight() - 225.0f, platformWidth[3] = CP_System_GetWindowWidth() - 1000.f;
		platformX[4] = 700, platformY[4] = CP_System_GetWindowHeight() - 400.0f, platformWidth[4] = CP_System_GetWindowWidth() - 1000.f;
	}
	else if (level == 2) {
		no_of_platforms = 5 + 1;
		//Add More For More Levels
		platformX[0] = 400, platformY[0] = CP_System_GetWindowHeight() - 50.0f, platformWidth[0] = CP_System_GetWindowWidth() - 780.f;
		platformX[1] = 0, platformY[1] = CP_System_GetWindowHeight() - 200.0f, platformWidth[1] = CP_System_GetWindowWidth() - 780.f;
		platformX[2] = 950, platformY[2] = CP_System_GetWindowHeight() - 200.0f, platformWidth[2] = CP_System_GetWindowWidth() - 1030.f;
		platformX[3] = 750, platformY[3] = CP_System_GetWindowHeight() - 100.0f, platformWidth[3] = CP_System_GetWindowWidth() - 1030.f;
		platformX[4] = 850, platformY[4] = CP_System_GetWindowHeight() - 150.0f, platformWidth[4] = CP_System_GetWindowWidth() - 1030.f;
		platformX[5] = 400, platformY[5] = CP_System_GetWindowHeight() - 350.0f, platformWidth[5] = CP_System_GetWindowWidth() - 780.f;
	}
	else if (level == 3) {
		no_of_platforms = 4 + 1;
		platformX[0] = 480, platformY[0] = CP_System_GetWindowHeight() - 50.0f, platformWidth[0] = CP_System_GetWindowWidth() - 1020.f;
		platformX[1] = 0, platformY[1] = CP_System_GetWindowHeight() - 200.0f, platformWidth[1] = CP_System_GetWindowWidth() - 1000.f;
		platformX[2] = 820, platformY[2] = CP_System_GetWindowHeight() - 50.0f, platformWidth[2] = CP_System_GetWindowWidth() - 1060.f;
		platformX[3] = 0, platformY[3] = CP_System_GetWindowHeight() - 350.0f, platformWidth[3] = CP_System_GetWindowWidth() - 980.f;
		platformX[4] = 1120, platformY[4] = CP_System_GetWindowHeight() - 350.0f, platformWidth[4] = CP_System_GetWindowWidth() - 1040.f;
	}
}

//Drawing the background of the different levels
void drawBackground()
{
	if (level_selector == 1) {
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		CP_Image_Draw(forest, 0, 0, 1280, 720, 255);
	}
	if (level_selector == 2) {
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		CP_Image_Draw(volcano, 0, 0, 1280, 720, 255);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 80));
		CP_Graphics_DrawRect(0, 0, 1280, 720);
	}
	if (level_selector == 3) {
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		CP_Image_Draw(stars, 0, 0, 1280, 720, 255);
	}
}

//Initializing variables to be used while game is played
void Game_Level_Init() {
	CP_System_SetFrameRate(60); CP_System_SetWindowSize(1280, 720); CP_Settings_TextSize(25.0);
	chestopen = CP_Sound_Load("Assets/chestOpen.wav"); explosion = CP_Sound_Load("Assets/Explosion.wav"); orb = CP_Sound_Load("Assets/Orb.wav"); gameover = CP_Sound_Load("Assets/GameOver.wav");
	Bob = CP_Image_Load("Assets/Bob.png"); BobL = CP_Image_Load("Assets/BobL.png"); heart = CP_Image_Load("Assets/heart.png"); hudbackplate = CP_Image_Load("Assets/hudbackplate.png");
	chest = CP_Image_Load("Assets/Chest.png"); bombPic = CP_Image_Load("Assets/Bomb.png"); purple_orb = CP_Image_Load("Assets/porbs.png"); yellow_orb = CP_Image_Load("Assets/yorbs.png");
	fail_screen = CP_Image_Load("Assets/fail.png"); clear_screen = CP_Image_Load("Assets/clear.png"); pause_menu = CP_Image_Load("Assets/pause.png"); mainMenu = CP_Image_Load("Assets/MainMenu.png");

	banner = CP_Image_Load("Assets/banner.png");
	BobWidth = CP_Image_GetWidth(Bob), BobHeight = CP_Image_GetHeight(Bob);
	//Background
	forest = CP_Image_Load("Assets/Level2bg.png");
	volcano = CP_Image_Load("Assets/volcano.png");
	stars = CP_Image_Load("Assets/Level3bg.png");
	picPlatform = CP_Image_Load("Assets/steps.png");//platform picture
	//Resets Timer/Health/Points/Multiplier/Bob Position/Unpause Game
	gameTimer = 60.0, health = 3, points = 0, multiplier = 1, multiplierTimer = 5, multiplierCombo = 0; update_hp = 3; Bobx = 1280.f / 2, Boby = 720.f / 2; starting_timer = 3;
	textAbovePlayer((float)Bobx, (float)Boby, "");
	//Power Up Modifiers Reset
	speedMultiplier = 1, maxJumpHeight = 175, BobImmune = FALSE;
	gIsPaused = FALSE, BobDirection = FALSE;

	//Base Platform
	initializePlatform(level_selector);
	initializeOrbs();
}

void Game_Level_Update() {
	//Main Code 
	{
		drawBackground(); //draw level background
		//Press P to Pause
		gIsPaused = CP_Input_KeyTriggered(KEY_P) ? !gIsPaused : gIsPaused;
		//Conditions for Pausing the Game (Dying, Time finish, Player Pause game)
		gIsPaused = (health == 0 || gameTimer <= 0.10 || gIsPaused == TRUE || Boby > 720) ? TRUE : FALSE;
		//Press Q to Terminate
		CP_Input_KeyTriggered(KEY_Q) ? CP_Engine_Terminate() : 0;
		//Rendering
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	
		//Spawning of Chests based on current game timer.
		if (gameTimer <= 45 && gameTimer >= 40 || gameTimer <= 25 && gameTimer >= 20)
		{
			drawTreasureChest();
			CP_Settings_ImageMode(CP_POSITION_CORNER);
			CP_Image_Draw(chest, (float)chestX, (float)chestY, (float)CP_Image_GetWidth(chest), (float)CP_Image_GetHeight(chest), 255);
			ChestCollision();
		}

		if (gameTimer <= 40 && gameTimer >= 25)
		{
			power = 0;
			cheststate = 1;
		}

		//Shorter Ver. of Immunity
		//Give Immunity Status based on immunity timer.
		immune_timer -= CP_System_GetDt();
		BobImmune = (immune_timer > 0) ? TRUE : FALSE;

		//Actual Game Code Flow
		HUD();														//Display HUD
		drawBob((float)Bobx, (float)Boby, BobDirection, BobImmune); //Display Player.
		lostHealth(health, hp);										//Checks for Decrement in healths to show red flash
		textAbovePlayer((float)Bobx, (float)Boby, textToShow);		//To Inform players of current multiplier/powerups gained
		drawPlatform();												//Display Platforms 
		switch (gIsPaused) {
		case TRUE: //Game is paused
			Clear_Fail_Pause();					//Pause / Fail / Clear Screen
			break;

		case FALSE:								//Game not paused
			if (starting_timer <= 0) {
				playerMovement();				//Movement Input
				scoreMultiplier();				//Score and Multiplier
				drawOrbs();						//Orbs and Points
				gameTimer -= CP_System_GetDt();	//Game Timer Reduction
			}
			else start_timer();					//3 Second timer before game starts per level.
			break;
		}
	}
}

void Game_Level_Exit() {
	CP_Image_Free(&heart);
	CP_Sound_Free(&gameover);
	CP_Sound_Free(&explosion);
	CP_Sound_Free(&chestopen);
}

//rendering createPlatformXY();
void drawPlatform() {
	//Test Platform Appearing/Dissapearing
	static float alpha = 255, toggle = 0;

	if (toggle == 0) {
		if (alpha > 0) alpha -= 85 * CP_System_GetDt();
		else toggle = 1;
	}
	else {
		if (alpha < 255) alpha += 85 * CP_System_GetDt();
		else toggle = 0;
	}
	//Logic To Draw All Platform
	CP_Settings_RectMode(CP_POSITION_CORNER);
	for (int i = 0; i < no_of_platforms; i++) {

		 //For Drawing Image As Platform (Will Include Lvl 3 Fade In/Out)
		if (level_selector == 1 || level_selector == 2)
		{
			CP_Image_Draw(picPlatform, platformX[i], platformY[i], platformWidth[i], platformHeight, 255);
		}
		if (level_selector == 3) {
		//Set Fade In and Out for Platform 2 & 3
			if (i == 2 || i == 3) CP_Image_Draw(picPlatform, platformX[i], platformY[i], platformWidth[i], platformHeight, (int)alpha);
			else CP_Image_Draw(picPlatform, platformX[i], platformY[i], platformWidth[i], platformHeight, 255);
		}
	}
	platformMovement();
}

//Displays Timer, Score, Score Multiplier, Health Remaining
void HUD() {
	//Backplate For HUD
	CP_Settings_Fill(CP_Color_Create(153, 217, 234, 150));
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(hudbackplate, 0, 0, (float)CP_Image_GetWidth(hudbackplate), (float)CP_Image_GetHeight(hudbackplate), 255);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	char Timer[10] = { 0 }, Points[50] = { 0 }, Multiplier[15] = { 0 };
	CP_Settings_TextSize(30);
	//Combine String and integer 
	sprintf_s(Timer, _countof(Timer), "Timer: %.0f", gameTimer);
	sprintf_s(Points, _countof(Points), "Points: %i", points);
	sprintf_s(Multiplier, _countof(Multiplier), "Multiplier: %ix", multiplier);

	//Timer and Points Display
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, 0);
	CP_Font_DrawText(Timer, 5, 25);
	//CP_Font_DrawText(Points, 5, 50);
	pointsToReach(points);

	//Multiplier Display
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, 0);
	CP_Font_DrawText(Multiplier, 1270, 25);

	//Health (1 to 3 Heart Icons)
	for (int x = 1240, i = 0; i < health; i++, x -= 45) {
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(heart, (float)x, 35, 30, 30, 255);
	}

	//Multiplier Timer Bar
	static float x;
	x = (float)multiplierTimer * 100;
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(CP_Color_Create(145, 224, 255, 255));
	CP_Graphics_DrawRectAdvanced(1280.f / 2, 20, x, 20, 0, 15);
}

int playerPlatformCollision(void) {
	for (int i = 0; i < no_of_platforms; i++) {
		if (Boby + BobHeight > platformY[i] && Bobx + BobWidth > platformX[i] && Bobx < platformX[i] + platformWidth[i] && Boby < platformY[i] + platformHeight) {
			return i;
		}
	}
	return -1;
}

//To move platforms based on the different levels.
void platformMovement() {
	float velocity = CP_System_GetDt() * 125;
	float position = speedMultiplier * velocity;
	float speed, speed2;
	static int toggle = 0, toggle2 = 0;
	speed = CP_System_GetDt() * 200;
	speed2 = CP_System_GetDt() * 300;
	if (level_selector == 2) {
		if (toggle == 0) {
			platformX[5] -= speed;
			if (Boby >= platformY[5] - BobHeight - 1 && Boby <= platformY[5] - BobHeight - 1 && (Bobx + BobWidth) >= platformX[5] && Bobx <= (platformX[5] + platformWidth[5])) {
				Bobx -= speed;
			}
			if (Boby >= platformY[5] - BobHeight - 1 && Boby <= platformY[5] - BobHeight - 1 && CP_Input_KeyDown(KEY_A) && (Bobx + BobWidth) >= platformX[5] && Bobx <= (platformX[5] + platformWidth[5])) {
				Bobx -= (position - speed);
				BobDirection = TRUE;
			}
			if (Boby >= platformY[5] - BobHeight - 1 && Boby <= platformY[5] - BobHeight - 1 && CP_Input_KeyDown(KEY_D) && (Bobx + BobWidth) >= platformX[5] && Bobx <= (platformX[5] + platformWidth[5])) {
				Bobx += (position + speed);
				BobDirection = FALSE;
			}
			if (platformX[5] < 0) {
				toggle = 1;
			}
		}
		if (toggle == 1) {
			platformX[5] += speed;
			if (Boby >= platformY[5] - BobHeight - 1 && Boby <= platformY[5] - BobHeight - 1 && (Bobx + BobWidth) >= platformX[5] && Bobx <= (platformX[5] + platformWidth[5])) {
				Bobx += speed;
			}
			if (Boby >= platformY[5] - BobHeight - 1 && Boby <= platformY[5] - BobHeight - 1 && CP_Input_KeyDown(KEY_A) && (Bobx + BobWidth) >= platformX[5] && Bobx <= (platformX[5] + platformWidth[5])) {
				Bobx -= (position + speed);
				BobDirection = TRUE;
			}
			if (Boby >= platformY[5] - BobHeight - 1 && Boby <= platformY[5] - BobHeight - 1 && CP_Input_KeyDown(KEY_D) && (Bobx + BobWidth) >= platformX[5] && Bobx <= (platformX[5] + platformWidth[5])) {
				Bobx += (position - speed);
				BobDirection = FALSE;
			}
			if (platformX[5] + platformWidth[5] > 1280) {
				toggle = 0;
			}
		}
	}
	else if (level_selector == 3) {
		//To move platform up and down
		if (toggle == 0) {
			platformY[2] -= speed;
			if (platformY[2] < CP_System_GetWindowHeight() - 350.0f) {
				toggle = 1;
			}
		}
		if (toggle == 1) {
			platformY[2] += speed;
			if (platformY[2] + platformHeight > 720) {
				toggle = 0;
			}
		}

		//To move platform left and right
		if (toggle2 == 0) {
			platformX[3] -= speed2;
			if (Boby >= platformY[3] - BobHeight - 1 && Boby <= platformY[3] - BobHeight - 1 && (Bobx + BobWidth) >= platformX[3] && Bobx <= (platformX[3] + platformWidth[3])) {
				Bobx -= speed2;
			}
			if (Boby >= platformY[3] - BobHeight - 1 && Boby <= platformY[3] - BobHeight - 1 && CP_Input_KeyDown(KEY_A) && (Bobx + BobWidth) >= platformX[3] && Bobx <= (platformX[3] + platformWidth[3])) {
				Bobx -= (position - speed2);
				BobDirection = TRUE;
			}
			if (Boby >= platformY[3] - BobHeight - 1 && Boby <= platformY[3] - BobHeight - 1 && CP_Input_KeyDown(KEY_D) && (Bobx + BobWidth) >= platformX[3] && Bobx <= (platformX[3] + platformWidth[3])) {
				Bobx += (position + speed2);
				BobDirection = FALSE;
			}
			if (platformX[3] < 0) {
				
				toggle2 = 1;
			}
		}
		if (toggle2 == 1) {
			platformX[3] += speed2;
			if (Boby >= platformY[3] - BobHeight - 1 && Boby <= platformY[3] - BobHeight - 1 && (Bobx + BobWidth) >= platformX[3] && Bobx <= (platformX[3] + platformWidth[3])) {
				Bobx += speed2;
			}
			if (Boby >= platformY[3] - BobHeight - 1 && Boby <= platformY[3] - BobHeight - 1 && CP_Input_KeyDown(KEY_A) && (Bobx + BobWidth) >= platformX[3] && Bobx <= (platformX[3] + platformWidth[3])) {
				Bobx -= (position + speed2);
				BobDirection = TRUE;
			}
			if (Boby >= platformY[3] - BobHeight - 1 && Boby <= platformY[3] - BobHeight - 1 && CP_Input_KeyDown(KEY_D) && (Bobx + BobWidth) >= platformX[3] && Bobx <= (platformX[3] + platformWidth[3])) {
				Bobx += (position - speed2);
				BobDirection = FALSE;
			}
			if (platformX[3] + platformWidth[3] > 740) {

				toggle2 = 0;
			}
		}



	}

}

//Sets the Multiplier based on the combo.
void scoreMultiplier(void) {
	if (multiplierTimer >= 0) {
		(multiplierCombo < 5) ? multiplier = 1, multiplierTimer -= CP_System_GetDt() : multiplier;
		(multiplierCombo >= 5 && multiplierCombo < 10) ? multiplier = 2, multiplierTimer -= 1.2*CP_System_GetDt() : multiplier;
		(multiplierCombo >= 10 && multiplierCombo < 20) ? multiplier = 3, multiplierTimer -= 1.4*CP_System_GetDt() : multiplier;
		(multiplierCombo >= 20 && multiplierCombo < 30) ? multiplier = 4, multiplierTimer -= 1.8*CP_System_GetDt() : multiplier;
		(multiplierCombo >= 30) ? multiplier = 5, multiplierTimer -= 2*CP_System_GetDt() : multiplier;
	}
	else {
		multiplier = 1;
		multiplierCombo = 0;
	}
}

//When Collected Orb, takes in amount of Points and returns the multiplied points to be added to total score
int pointsCollected(int x) {
	multiplierTimer = 5.00, points += x * multiplier, multiplierCombo++;
	return x*multiplier;
}


// Logic For Player Movement
void playerMovement() {
	float velocity = CP_System_GetDt() * 350;
	static double maxJump = 0, jumpCD = 0;
	static int jumpCounter = 1;
	float jump = CP_System_GetDt() * 1400;
	float gravity = CP_System_GetDt() * 400;
	float position = speedMultiplier * velocity;

	//For Player if not jumping and in air
	int collidedPlatform = -1;
	Boby += gravity;
	collidedPlatform = playerPlatformCollision();
	//printf("collision is %d", collidedPlatform);
	if (collidedPlatform >= 0) {
		Boby = platformY[collidedPlatform] - BobHeight - 1;
		playeronground = 1;
	}
	else { playeronground = 0;}
	collidedPlatform = -1;

	if (CP_Input_KeyDown(KEY_A)) {
		Bobx -= position;
		BobDirection = TRUE;

		collidedPlatform = playerPlatformCollision();
		if (collidedPlatform >= 0) {
			Bobx = platformX[collidedPlatform] + platformWidth[collidedPlatform] + 1;
		}
		collidedPlatform = -1;
	}

	if (CP_Input_KeyDown(KEY_D)) {
		Bobx += position;
		BobDirection = FALSE;

		collidedPlatform = playerPlatformCollision();
		if (collidedPlatform >= 0) {
			Bobx = platformX[collidedPlatform] - 1 - BobWidth;
		}
		collidedPlatform = -1;
	}

	//Jumping
	if (CP_Input_KeyTriggered(KEY_SPACE) && jumpCounter != 0) {
		//particleEffect(Bobx, Boby, "Jump!");
		--jumpCounter;
		//Boby -= jump;
		maxJump = maxJumpHeight;
		
	}
	if (playeronground) {
		jumpCounter = 1;
	}
	//Jump CD Decrement every deltaTime
	//jumpCD -= (jumpCD >= 0) ? CP_System_GetDt() : jumpCD;
	maxJump = (Boby <= 10) ? 0 : maxJump;	 //To stop at ceiling
	if (maxJump > 0) {
		if (playerPlatformCollision() == 1) {
			Boby += gravity;
		}
		else {
			Boby -= jump;
			collidedPlatform = playerPlatformCollision();
			if (collidedPlatform >= 0) {
				Boby = platformY[collidedPlatform] + platformHeight + 1;
			}
			collidedPlatform = -1;
		}
		maxJump -= jump;
	}
}

//Shows the Clear/Fail/Pause when it occurs.
void Clear_Fail_Pause(void) {
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	char Points[50] = { 0 };
	sprintf_s(Points, _countof(Points), "%i", points);
	CP_Settings_TextSize(50); CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//Clear Condition
	if (gameTimer <= 0.10 || (points >= 750 && health <= 0)) {
		//TODO: SHOW MENU FOR CLEAR - CLEAR! => POINTS EARNED, HEALTH REMAINING, RETRY STAGE / GOTO NEXT STAGE
		CP_Image_Draw(clear_screen, 0, 0, (float)CP_Image_GetWidth(clear_screen), (float)CP_Image_GetHeight(clear_screen), 255);
		(level_selector == 3) ? CP_Image_Draw(mainMenu, 550-5, 360-5, 185+5, 85+5, 255) : 0;	//for fiinal level
		CP_Font_DrawText(Points, (float)(CP_System_GetWindowWidth() / 2 + 65), (float)(CP_System_GetWindowHeight() / 2 - 105));
		addStarsRating();
		if (CP_Input_MouseClicked()) {
			if (isRectangleClicked(550, 360, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				//For Final Level > 3
				if (level_selector == 3) {
					CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
				}
				else {
					level_selector += 1;
					Game_Level_Init();
				}
			}
			if (isRectangleClicked(550, 455, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				Game_Level_Init();
			}
		}
	}

	//Fail Condition
	else if (health <= 0 || Boby > 720) {
		//TODO: SHOW MENU FOR FAIL - YOU DIED => POINTS EARNED, 0 HEALTH, RETRY STAGE / GOTO NEXT STAGE 
		if (Boby > 720)
		{
			CP_Sound_Play(gameover);
			Boby = 720;
		}
		if(health == 0)
		CP_Sound_Play(gameover);
		health = -1;
		CP_Image_Draw(fail_screen, 0, 0, (float)CP_Image_GetWidth(fail_screen), (float)CP_Image_GetHeight(fail_screen), 255);
		CP_Font_DrawText(Points, (float)(CP_System_GetWindowWidth() / 2 + 65), (float)(CP_System_GetWindowHeight() / 2 - 105));
		addStarsRating();
		if (CP_Input_MouseClicked()) {
			//Btn to Return Home
			if (isRectangleClicked(550, 360, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
			//Btn to Retry
			if (isRectangleClicked(550, 455, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				Game_Level_Init();
			}
		}
	}
	//Game Pause
	else {
		CP_Image_Draw(pause_menu, 0, 0, (float)CP_Image_GetWidth(pause_menu), (float)CP_Image_GetHeight(pause_menu), 255);
		if (CP_Input_MouseClicked()) {
			//Retry
			if (IsCircleClicked(575, 330, 75, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				Game_Level_Init();
			}
			//Menu
			if (IsCircleClicked(684.5 + 37.5, 330, 75, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				CP_Settings_ImageMode(CP_POSITION_CENTER);
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
			//Resume
			if (isRectangleClicked(540, 391, 200, 75, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
				gIsPaused = !gIsPaused;
		}
	}
}

//Orbs Variable to Change(game feel)
int no_of_orbs = 5, start_pos_x = 1280 - 25, start_pos_y = -35, respawn_timer = 10;
float yDespawn = 2, pDespawn = 3.5, bDespawn = 0.1f, pDropSpeed, yDropSpeed, bDropSpeed;

//To Initialize Orbs at start of level
void initializeOrbs() {
	for (int i = 0; i < no_of_orbs; i++) {
		resetyOrb(i);
		resetpOrb(i);
		resetBomb(i);
	}
}

//Orbs + Bombs Rendering
void drawOrbs() {
	makeOrbsFall();
	orbOnFloor();
	orbsCollected();
	for (int i = 0; i < no_of_orbs; i++) {
		CP_Image_Draw(purple_orb, (float)(pOrbs[i].x - 25) , (float)(pOrbs[i].y - 25), 50.f, 50.f, (int)(pOrbs[i].timer_on_floor * (255 / pDespawn)));
		CP_Image_Draw(yellow_orb, (float)(yOrbs[i].x - 25), (float)(yOrbs[i].y - 25), 50.f, 50.f, (int)(yOrbs[i].timer_on_floor * (255 / pDespawn)));
		CP_Image_Draw(bombPic, (float)(bOrbs[i].x - 25), (float)(bOrbs[i].y - 25), 50.f, 50.f, 255);
	}
}

//Logic to make orbs fall.
void makeOrbsFall() {
	for (int i = 0; i < no_of_orbs; i++) {
		if (pOrbs[i].timer_to_drop < 0) {
			pOrbs[i].y += (int)pOrbs[i].dropSpeed;
		}
		else pOrbs[i].timer_to_drop -= CP_System_GetDt();
		if (yOrbs[i].timer_to_drop < 0) {
			yOrbs[i].y += (int)yOrbs[i].dropSpeed;
		}
		else yOrbs[i].timer_to_drop -= CP_System_GetDt();
		if (bOrbs[i].timer_to_drop < 0) {
			bOrbs[i].y += (int)bOrbs[i].dropSpeed;
		}
		else bOrbs[i].timer_to_drop -= CP_System_GetDt();

		//Reinitialize Orbs when OOB or Too Long on Floor
		(pOrbs[i].y > 720 || pOrbs[i].timer_on_floor < 0) ? resetpOrb(i) : 0;
		(yOrbs[i].y > 720 || yOrbs[i].timer_on_floor < 0) ? resetyOrb(i) : 0;

		//To Draw Blast Radius when Bomb Hit a Platform
		if (bOrbs[i].y > 720 || bOrbs[i].timer_on_floor < 0)
		{
			if (circleCollision((float)bOrbs[i].x, (float)bOrbs[i].y, 150.f, (float)Bobx, (float)Boby, (float)BobWidth, (float)BobHeight) == 1)
			{
				if (BobImmune != 1)
				{
					health--;
				}
				CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255)); //red circle
				CP_Graphics_DrawCircle((float)bOrbs[i].x, (float)bOrbs[i].y, 150.f);
				resetBomb(i);
				soundCheck = 0;
			}
			else
			{
				CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
				CP_Graphics_DrawCircle((float)bOrbs[i].x, (float)bOrbs[i].y, 150.f);
				resetBomb(i);
				soundCheck = 0;
			}
		}
	}
}
//Orbs + Bombs
//Sets Orb Velocity, If detect collision = 0, no Collision = Velocity
//Resets drop velocity > checks for collision and set new speed(if collision)
void orbOnFloor() {
	pDropSpeed = CP_System_GetDt() * 250;
	yDropSpeed = CP_System_GetDt() * 200;
	bDropSpeed = CP_System_GetDt() * 400;
	
	for (int i = 0; i < no_of_orbs; i++) {
		pOrbs[i].dropSpeed = pDropSpeed;
		yOrbs[i].dropSpeed = yDropSpeed;
		bOrbs[i].dropSpeed = bDropSpeed;
		for (int x = 0; x < no_of_platforms; x++) {
			//for purple
			if (circleToPlatform((float)pOrbs[i].x, (float)pOrbs[i].y, 50.f, (float)platformX[x], (float)platformY[x], (float)platformWidth[x], (float)platformHeight) == 1) {
				pOrbs[i].timer_on_floor -= CP_System_GetDt();
				pOrbs[i].y = (int)platformY[x] - 25;
				pOrbs[i].dropSpeed = 0;
			}
			//for yellow
			if (circleToPlatform((float)yOrbs[i].x, (float)yOrbs[i].y, 50.f, (float)platformX[x], (float)platformY[x], (float)platformWidth[x], (float)platformHeight) == 1) {
				yOrbs[i].timer_on_floor -= CP_System_GetDt();
				yOrbs[i].y = (int)platformY[x] - 25;
				yOrbs[i].dropSpeed = 0;
			}
			//for bomb
			if (circleToPlatform((float)bOrbs[i].x, (float)bOrbs[i].y, 50.f, (float)platformX[x], (float)platformY[x], (float)platformWidth[x], (float)platformHeight) == 1) {
				if (soundCheck == 0)
				{
					CP_Sound_PlayAdvanced(explosion, 0.3f, 1.0f, FALSE, CP_SOUND_GROUP_2);
					soundCheck = 1;
				}
				bOrbs[i].timer_on_floor -= 50 * CP_System_GetDt();
				bOrbs[i].dropSpeed = 0;
			}
		}
	}
}

//Logic to give points when player collects orbs.
void orbsCollected(void) {
	char Multiplier[3] = {0};
	for (int i = 0; i < no_of_orbs; i++) {
		//Collision with orb to give 5 points.
		if (circleCollision((float)pOrbs[i].x, (float)pOrbs[i].y, 50.f, (float)Bobx, (float)Boby, (float)BobWidth, (float)BobHeight) == 1) {
			pointsCollected(5);
			CP_Sound_Play(orb);
			sprintf_s(Multiplier, _countof(Multiplier), "%ix", multiplier);
			setText(Multiplier);	
			resetpOrb(i);	//Reinitialize when collected
		}
		//Collision with orb to give 10 points.
		if (circleCollision((float)yOrbs[i].x, (float)yOrbs[i].y, 50.f, (float)Bobx, (float)Boby, (float)BobWidth, (float)BobHeight) == 1) {
			pointsCollected(10);
			CP_Sound_Play(orb);
			sprintf_s(Multiplier, _countof(Multiplier), "%ix", multiplier);
			setText(Multiplier);
			resetyOrb(i);	//Reinitialize when collected
		}
		//for Bombs
		if (circleCollision((float)bOrbs[i].x, (float)bOrbs[i].y, 50.f, (float)Bobx, (float)Boby, (float)BobWidth, (float)BobHeight) == 1) {
			if (BobImmune != 1) {
				health--;
			}
			resetBomb(i);	//Reinitialize when collected
		}
	}
}

//Rendering of Treasure Chest
void drawTreasureChest()
{
	if (cheststate == 1)
	{
		if (level_selector == 1)
		{
			chestX = 800;
			chestY = 260;
			CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
			CP_Graphics_DrawRect((float)chestX, (float)chestY, 70.f, 60.f);
		}
		if (level_selector == 2)
		{
			chestX = 500;
			chestY = 610;
			CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
			CP_Graphics_DrawRect((float)chestX, (float)chestY, 70.f, 60.f);
		}
		if (level_selector == 3)
		{
			chestX = 100;
			chestY = 460;
			CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
			CP_Graphics_DrawRect((float)chestX, (float)chestY, 70.f, 60.f);
		}
	}
	if (cheststate == 0)
	{
		chestX = 2000, chestY = 2000;
		CP_Graphics_DrawRect((float)chestX, (float)chestY, 0.f, 0.f);
	}
}

int ChestCollision()
{
	if (Boby + BobHeight >= chestY && Boby <= chestY + 60 && Bobx + BobWidth > chestX && Bobx < chestX + 70)
	{
		if (power == 0)
		{
			power_up();
		}
		CP_Sound_Play(chestopen);
		cheststate = 0;
		return 1;
	}
	else { return 0; }
}

// Different Types of Powerups(effects) for Treasure Chest
void power_up() {

	int powerup = random_int(1,5);
	if (powerup == 1)
	{
		setText("Immunity");
		immunity();
		power = 1;
	}
	else if (powerup == 2)
	{
		setText("Add. Health");
		add_health();
		power = 1;
	}
	else if (powerup == 3)
	{
		setText("Higher Jump");
		jump_high();
		power = 1;
	}
	else if (powerup == 4)
	{
		setText("Increased Speed");
		move_fast();
		power = 1;
	}
	else if (powerup == 5)
	{
		setText("Add. Time");
		time_extension();
		power = 1;
	}
}

void immunity(void)
{
	immune_timer = 5;
}

void add_health(void)
{
	(health < 3) ? ++health : 0;
}

void jump_high(void)
{
	maxJumpHeight += 35;
}

void move_fast(void)
{
	speedMultiplier += 0.5;
}

void time_extension(void)
{
	gameTimer += 10.0;
}

//For Utility, to show text above the player regarding current situation. 
void textAbovePlayer(float x, float y, char *text) {
	//For Text On top of Bob
	if (textTimer >= 0) {
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		if (strcmp(text, "Add. Health") == 0)
		CP_Settings_Fill(CP_Color_Create(0, 255, 0, 255));
		else CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Font_DrawText(text, x + 40, y - 20);
		textTimer -= CP_System_GetDt();
	}
}

//For Utility, to set text to be displayed next.
void setText(char* text) {
	textTimer = 3;
	memset(textToShow, 0, sizeof(textToShow));
	for (int i = 0; text[i] != '\0'; i++) {
		textToShow[i] = text[i];
	}
}

//Resetting Bomb
void resetBomb(int i) {
	bOrbs[i].timer_to_drop = (float)random_int(3, 5),
		bOrbs[i].y = start_pos_y,
		bOrbs[i].x = random_int(25, 1280 - 25),
		bOrbs[i].timer_on_floor = bDespawn;
}
//Resetting red Orb
void resetyOrb(int i) {
	yOrbs[i].timer_to_drop = (float)random_int(3 , 5),
		yOrbs[i].y = start_pos_y, 
		yOrbs[i].x = random_int(25, 1280 - 25),
		yOrbs[i].timer_on_floor = yDespawn;
}
//Resetting purple Orb
void resetpOrb(int i) {
	pOrbs[i].timer_to_drop = (float)random_int(0, 3),
		pOrbs[i].y = start_pos_y,
		pOrbs[i].x = random_int(25 , 1280-25),
		pOrbs[i].timer_on_floor = pDespawn;
}

//Logic to start the game after 3 seconds upon selecting a level. And Display of text.
void start_timer(void) {
	char showtime[3] = { 0 };
	sprintf_s(showtime, _countof(showtime), "%d", (int)starting_timer + 1);
	CP_Image_Draw(banner, 0, 0, 1280, 720, 255);
	CP_Settings_TextSize(100);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_BASELINE);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText(showtime, 1280.f / 2, 720 / 2 + 50);
	starting_timer -= CP_System_GetDt();
}

//For Utility, to display upon clearing a level under pass/fail screen the stars earned for the current points.
void addStarsRating(void) {
	CP_Image stars = CP_Image_Load("Assets/IBob.png");
	static int i;
	int star = 0;
	//750, 1200, 1600
	star = (points >= 750) ? 1 : star;
	star = (points >= 1200) ? 2 : star;
	star = (points >= 1600) ? 3 : star;
	for (int x = 1280 / 2 - 100, i = 1; i <= star; i++, x += 75) {
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(stars, (float)x, 720 / 2 - 65, 50, 50, 255);
		GlowingBob((float)(x-7.5), 720 / 2 - 72.5, 65, 65);
	}
}

