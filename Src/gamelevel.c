#include "cprocessing.h"
#include "gamelevel.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

CP_Image Bob, BobL, heart, chest, Bomb, fail_screen, clear_screen, pause_menu, purple_orb, yellow_orb, particle, jumpParticle, bombPic;
CP_Sound chestopen, explosion, orb, gameover;
// Bob Variables
double Bobx, Boby;
int BobWidth, BobHeight;
// Volatile Variables for Game 
int health, points, multiplier, multiplierCombo;
double gameTimer, multiplierTimer, immune_timer;
// Variables for Movement by powerUps
float maxJumpHeight = 200, speedMultiplier = 1;

// Variables for Platform Creation
int level_selector = 1;	//TODO: REMOVE = 1 WHEN MAIN MENU DONE
float platformX[100], platformY[100], platformWidth[100], platformHeight = 50.0f;
int no_of_platforms = 6;

// Pause and BobStagnant 
bool gIsPaused, BobDirection, BobImmune = 0;

//For Chest
int cheststate = 1;
static int chestX, chestY;
int static soundCheck = 0; //sfx for bomb 
int power = 0;

//Text (Testing)
float textTimer = 0;
char textToShow[50] = {"Test"};

//Can Be Used for Chest, Bomb, Orbs
struct Items
{
	int x, y;
	float timer_to_drop, timer_on_floor, dropSpeed;
};
struct Items pOrbs[10], yOrbs[10], bOrbs[10];

//Initialize All Platforms for Selected Level
void initializePlatform(int level) {
	if (level == 1) {
		platformX[0] = 400, platformY[0] = CP_System_GetWindowHeight() - 50.0f, platformWidth[0] = CP_System_GetWindowWidth() - 900;
		platformX[1] = 950, platformY[1] = CP_System_GetWindowHeight() - 50.0f, platformWidth[1] = CP_System_GetWindowWidth() - 1000;
		platformX[2] = 0, platformY[2] = CP_System_GetWindowHeight() - 50.0f, platformWidth[2] = CP_System_GetWindowWidth() - 1000;
		platformX[3] = 300, platformY[3] = CP_System_GetWindowHeight() - 225.0f, platformWidth[3] = CP_System_GetWindowWidth() - 1000;
		platformX[4] = 700, platformY[4] = CP_System_GetWindowHeight() - 400.0f, platformWidth[4] = CP_System_GetWindowWidth() - 1000;
	}
	else if (level == 2) {
		memset(platformX, 0, sizeof(platformX)); 
		memset(platformY, 0, sizeof(platformY));
		memset(platformWidth, 0, sizeof(platformWidth));
		//Add More For More Levels
		platformX[0] = 400, platformY[0] = CP_System_GetWindowHeight() - 50.0f, platformWidth[0] = CP_System_GetWindowWidth() - 780;
		platformX[1] = 0, platformY[1] = CP_System_GetWindowHeight() - 200.0f, platformWidth[1] = CP_System_GetWindowWidth() - 780;
		platformX[2] = 950, platformY[2] = CP_System_GetWindowHeight() - 200.0f, platformWidth[2] = CP_System_GetWindowWidth() - 1030;
		platformX[3] = 750, platformY[3] = CP_System_GetWindowHeight() - 100.0f, platformWidth[3] = CP_System_GetWindowWidth() - 1030;
		platformX[4] = 850, platformY[4] = CP_System_GetWindowHeight() - 150.0f, platformWidth[4] = CP_System_GetWindowWidth() - 1030;
		platformX[5] = 400, platformY[5] = CP_System_GetWindowHeight() - 350.0f, platformWidth[5] = CP_System_GetWindowWidth() - 780;
	}
}

void Game_Level_Init() {
	CP_System_SetFrameRate(60); CP_System_SetWindowSize(1280, 720); CP_Settings_TextSize(25.0);
	chestopen = CP_Sound_Load("Assets/ChestOpen.wav");
	explosion = CP_Sound_Load("Assets/Explosion.wav");
	orb = CP_Sound_Load("Assets/Orb.wav");
	gameover = CP_Sound_Load("Assets/GameOver.wav");
	Bob = CP_Image_Load("Assets/Bob.png"); BobL = CP_Image_Load("Assets/BobL.png");
	heart = CP_Image_Load("Assets/heart.png");
	chest = CP_Image_Load("Assets/Chest.png"); bombPic = CP_Image_Load("Assets/Bomb.png");
	purple_orb = CP_Image_Load("Assets/porbs.png"); yellow_orb = CP_Image_Load("Assets/yorbs.png");
	fail_screen = CP_Image_Load("Assets/fail.png"); clear_screen = CP_Image_Load("Assets/clear.png"); pause_menu = CP_Image_Load("Assets/pause.png");
	particle = CP_Image_Load("Assets/particle.png"); jumpParticle = CP_Image_Load("Assets/particle1.png");
	BobWidth = CP_Image_GetWidth(Bob), BobHeight = CP_Image_GetHeight(Bob);
	//Resets Timer/Health/Points/Multiplier/Bob Position/Unpause Game
	gameTimer = 60.0, health = 3, points = 0, multiplier = 1, multiplierTimer = 5, multiplierCombo = 0;
	gIsPaused = FALSE, BobDirection = FALSE; Bobx = 1280 / 2, Boby = 720 / 2;

	//Base Platform
	initializePlatform(level_selector);
	initializeOrbs();
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


		//Draw Chest
		if (gameTimer <= 60 && gameTimer >= 40 || gameTimer <= 25 && gameTimer >= 20)
		{
			drawTreasureChest();
			CP_Settings_ImageMode(CP_POSITION_CORNER);
			CP_Image_Draw(chest, chestX, chestY, CP_Image_GetWidth(chest), CP_Image_GetHeight(chest), 255);
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
		if (BobImmune == FALSE)
		{
			Bob = CP_Image_Load("Assets/Bob.png");
			BobL = CP_Image_Load("Assets/BobL.png");
		}

		switch (gIsPaused) {
		case TRUE: //Game is paused
			Clear_Fail_Pause();				//Pause / Fail / Clear Screen
			break;

		case FALSE: //Game not paused
			playerMovement();				//Movement Input
			scoreMultiplier();				//Score and Multiplier
			gameTimer -= CP_System_GetDt();	//Game Timer Reduction
			drawOrbs();						//Orbs and Points
			textAbovePlayer(Bobx, Boby, textToShow);
			break;
		}
	}
	//TESTCODE
	{
		//TO REMOVE: Test Health Increment and Cap at 3
		CP_Input_KeyTriggered(KEY_1) && (health > 0) ? --health : NULL;	//CP_Input_KeyTriggered(KEY_1) can be replaced for collision w/ bomb
		CP_Input_KeyTriggered(KEY_3) ? multiplierTimer = 5.00, points += 1 * multiplier, multiplierCombo++ : multiplierTimer;
	}
}

void Game_Level_Exit() {
	CP_Image_Free(&heart);
	CP_Sound_Free(gameover);
	CP_Sound_Free(explosion);
	CP_Sound_Free(chestopen);
}
//rendering createPlatformXY();
void drawPlatform() {
	//Test Platform Appearing/Dissapearing
	static float alpha = 255, toggle = 0;
	/*
	if (toggle == 0) {
		if (alpha > 0) alpha -= 85 * CP_System_GetDt();
		else toggle = 1;
	}
	else {
		if (alpha < 255) alpha += 85 * CP_System_GetDt();
		else toggle = 0;
	}*/
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, alpha));
	CP_Settings_RectMode(CP_POSITION_CORNER);
	for (int i = 0; i < no_of_platforms; i++) {
		if (i == 5) CP_Settings_Fill(CP_Color_Create(255, 0, 0, alpha));
		CP_Graphics_DrawRect(platformX[i], platformY[i], platformWidth[i], platformHeight);
	}
	platformMovement();
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

int playerPlatformCollision(void) {
	for (int i = 0; i < no_of_platforms; i++) {
		if (Boby + BobHeight > platformY[i] && Bobx + BobWidth > platformX[i] && Bobx < platformX[i] + platformWidth[i] && Boby < platformY[i] + platformHeight) {
			return i;
		}
	}
	return -1;
}

void platformMovement() {
	float speed;
	static int toggle = 0;
	speed = CP_System_GetDt() * 200;
	if (toggle == 0) {
		platformX[5] -= speed;
		if (platformX[5] < 0) {
			toggle = 1;
		}
	}
	if (toggle == 1) {
		platformX[5] += speed;
		if (platformX[5] + platformWidth[5] > 1280) {
			toggle = 0;
		}
	}
}

//scoreMultiplier Sets the Multiplier based on the game state.
void scoreMultiplier(void) {
	//Only Decrease if Timer > 0
	//(multiplierTimer > 0 && multiplier == 1) ? multiplierTimer -= CP_System_GetDt() : multiplier;

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

//When Collected Orb, takes in amount of Points
int pointsCollected(int x) {
	multiplierTimer = 5.00, points += x * multiplier, multiplierCombo++;
	return x*multiplier;
}

// Logic For Player Movement
void playerMovement() {
	float velocity = CP_System_GetDt() * 350;
	static double maxJump = 0, jumpCD = 0;
	static int jumpCounter = 2;
	float jump = CP_System_GetDt() * 1500;
	float gravity = CP_System_GetDt() * 500;
	float position = speedMultiplier * velocity;

	//For Player if not jumping and in air
	int collidedPlatform = -1;
	Boby += gravity;
	collidedPlatform = playerPlatformCollision();
	//printf("collision is %d", collidedPlatform);
	if (collidedPlatform >= 0) {
		Boby = platformY[collidedPlatform] - BobHeight - 1;
	}
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
	if (CP_Input_KeyTriggered(KEY_SPACE) && jumpCD <= 0 && jumpCounter != 0) {
		//particleEffect(Bobx, Boby, "Jump!");
		--jumpCounter;
		maxJump = maxJumpHeight;
		if (jumpCounter == 0) {
			jumpCD = 1.00;
			jumpCounter = 2;
		}
	}
	//Jump CD Decrement every deltaTime
	jumpCD -= (jumpCD >= 0) ? CP_System_GetDt() : jumpCD;
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
				level_selector = 2;
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
		if(health == 0)
		CP_Sound_Play(gameover);
		health--;
		CP_Image_Draw(fail_screen, 0, 0, CP_Image_GetWidth(fail_screen), CP_Image_GetHeight(fail_screen), 255);
		CP_Font_DrawText(Points, CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() / 2 - 45);
		if (CP_Input_MouseClicked()) {
			if (isRectangleClicked(550, 360, 180, 80, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				level_selector = 2;
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
float yDespawn = 2, pDespawn = 3.5, bDespawn = 0.1, pDropSpeed, yDropSpeed, bDropSpeed;

//To Initialize Orbs at start of level
void initializeOrbs() {
	for (int i = 0; i < no_of_orbs; i++) {
		pOrbs[i].x = rand() % +start_pos_x, pOrbs[i].y = start_pos_y;
		yOrbs[i].x = rand() % start_pos_x, yOrbs[i].y = start_pos_y;
		bOrbs[i].x = rand() % 1280, bOrbs[i].y = 0 - 25;
		pOrbs[i].timer_to_drop = rand() % respawn_timer;
		yOrbs[i].timer_to_drop = rand() % respawn_timer;
		bOrbs[i].timer_to_drop = rand() % respawn_timer;	
		pOrbs[i].timer_on_floor = pDespawn;
		yOrbs[i].timer_on_floor = yDespawn;
		bOrbs[i].timer_on_floor = bDespawn;
	}
}

//Orbs + Bombs
void drawOrbs() {
	makeOrbsFall();
	orbOnFloor();
	orbsCollected();
	for (int i = 0; i < no_of_orbs; i++) {
		CP_Image_Draw(purple_orb, pOrbs[i].x - 25 , pOrbs[i].y - 25, 50, 50, pOrbs[i].timer_on_floor * (255 / pDespawn));
		CP_Image_Draw(yellow_orb, yOrbs[i].x - 25, yOrbs[i].y - 25, 50, 50, yOrbs[i].timer_on_floor * (255 / pDespawn));
		CP_Image_Draw(bombPic, bOrbs[i].x - 25, bOrbs[i].y - 25, 50, 50, 255);
	}
}
//Orbs + Bombs
//Orbs will Fall as Long as Timer_to_drop = 0
void makeOrbsFall() {
	for (int i = 0; i < no_of_orbs; i++) {
		if (pOrbs[i].timer_to_drop < 0) {
			pOrbs[i].y += pOrbs[i].dropSpeed;
		}
		else pOrbs[i].timer_to_drop -= CP_System_GetDt();
		if (yOrbs[i].timer_to_drop < 0) {
			yOrbs[i].y += yOrbs[i].dropSpeed;
		}
		else yOrbs[i].timer_to_drop -= CP_System_GetDt();
		if (bOrbs[i].timer_to_drop < 0) {
			bOrbs[i].y += bOrbs[i].dropSpeed;
		}
		else bOrbs[i].timer_to_drop -= CP_System_GetDt();

		//Reinitialize Orbs when OOB or Too Long on Floor
		(pOrbs[i].y > 720 || pOrbs[i].timer_on_floor < 0) ? pOrbs[i].timer_to_drop = rand() % respawn_timer,
			pOrbs[i].y = start_pos_y, pOrbs[i].x = 25 + rand() % start_pos_x, pOrbs[i].timer_on_floor = pDespawn : 0;
		(yOrbs[i].y > 720 || yOrbs[i].timer_on_floor < 0) ? yOrbs[i].timer_to_drop = rand() % respawn_timer,
			yOrbs[i].y = start_pos_y, yOrbs[i].x = 25 + rand() % start_pos_x, yOrbs[i].timer_on_floor = yDespawn : 0;
		/*
		(bOrbs[i].y > 720 || bOrbs[i].timer_on_floor < 0) ? bOrbs[i].timer_to_drop = rand() % respawn_timer,
			bOrbs[i].y = start_pos_y, bOrbs[i].x = 25 + rand() % start_pos_x, bOrbs[i].timer_on_floor = bDespawn : 0;
		*/

		if (bOrbs[i].y > 720 || bOrbs[i].timer_on_floor < 0)
		{
			if (circleCollision(bOrbs[i].x, bOrbs[i].y, 200, Bobx, Boby, BobWidth, BobHeight) == 1)
			{
				health--;
				CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
				CP_Graphics_DrawCircle(bOrbs[i].x, bOrbs[i].y, 200);
				bOrbs[i].timer_to_drop = rand() % respawn_timer;
				bOrbs[i].y = start_pos_y; bOrbs[i].x = rand() % start_pos_x;
				bOrbs[i].timer_on_floor = bDespawn;
				soundCheck = 0;
			}
			else
			{
				CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
				CP_Graphics_DrawCircle(bOrbs[i].x, bOrbs[i].y, 200);
				bOrbs[i].timer_to_drop = rand() % respawn_timer;
				bOrbs[i].y = start_pos_y; bOrbs[i].x = rand() % start_pos_x;
				bOrbs[i].timer_on_floor = bDespawn;
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
			if (circleToPlatform(pOrbs[i].x, pOrbs[i].y, 50, platformX[x], platformY[x], platformWidth[x], platformHeight) == 1) {
				pOrbs[i].timer_on_floor -= CP_System_GetDt();
				pOrbs[i].dropSpeed = 0;
			}
			//for yellow
			if (circleToPlatform(yOrbs[i].x, yOrbs[i].y, 50, platformX[x], platformY[x], platformWidth[x], platformHeight) == 1) {
				yOrbs[i].timer_on_floor -= CP_System_GetDt();
				yOrbs[i].dropSpeed = 0;
			}
			//for bomb
			if (circleToPlatform(bOrbs[i].x, bOrbs[i].y, 50, platformX[x], platformY[x], platformWidth[x], platformHeight) == 1) {
				if (soundCheck == 0)
				{
					CP_Sound_Play(explosion);
					soundCheck = 1;
				}
				bOrbs[i].timer_on_floor -= CP_System_GetDt();
				bOrbs[i].dropSpeed = 0;
			}
		}
	}
}
//Orbs + Bombs
//Collecting Orbs Give Points
void orbsCollected(void) {
	int x;
	char Points[50] = { 0 };
	for (int i = 0; i < no_of_orbs; i++) {
		//for purple
		if (circleCollision(pOrbs[i].x, pOrbs[i].y, 50, Bobx, Boby, BobWidth, BobHeight) == 1) {
			x = pointsCollected(5);
			CP_Sound_Play(orb);
			sprintf_s(Points, _countof(Points), "+ %i", x);
			setText(Points);
			//Reinitialize when collected
			pOrbs[i].timer_to_drop = rand() % respawn_timer,
				pOrbs[i].y = start_pos_y, pOrbs[i].x = 25 + rand() % start_pos_x, pOrbs[i].timer_on_floor = pDespawn;
		}
		//for yellow
		if (circleCollision(yOrbs[i].x, yOrbs[i].y, 50, Bobx, Boby, BobWidth, BobHeight) == 1) {
			x = pointsCollected(10);
			CP_Sound_Play(orb);
			sprintf_s(Points, _countof(Points), "+ %i", x);
			setText(Points);
			//Reinitialize when collected
			yOrbs[i].timer_to_drop = rand() % respawn_timer,
				yOrbs[i].y = start_pos_y, yOrbs[i].x = 25 + rand() % start_pos_x, yOrbs[i].timer_on_floor = yDespawn;
		}
		//for Bombs
		if (circleCollision(bOrbs[i].x, bOrbs[i].y, 50, Bobx, Boby, BobWidth, BobHeight) == 1) {
			if (BobImmune != 1) {
				setText("Dodge plssssss");
				health--;
			}
			//Reinitialize when collected
			bOrbs[i].timer_to_drop = rand() % respawn_timer,
				bOrbs[i].y = start_pos_y, bOrbs[i].x = 25 + rand() % start_pos_x, bOrbs[i].timer_on_floor = bDespawn;
		}
	}
}

//Treasure Chest
void drawTreasureChest()
{
	if (cheststate == 1)
	{
		if (level_selector == 1)
		{
			chestX = 800;
			chestY = 260;
			CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
			CP_Graphics_DrawRect(chestX, chestY, 70, 60);
		}
		if (level_selector == 2)
		{
			chestX = 500;
			chestY = 610;
			CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
			CP_Graphics_DrawRect(chestX, chestY, 70, 60);
		}
	}
	if (cheststate == 0)
	{
		chestX = 2000, chestY = 2000;
		CP_Graphics_DrawRect(chestX, chestY, 0, 0);
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

// Power - Ups
void power_up() {

	int powerup = rand() % 5;
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
	Bob = CP_Image_Load("Assets/IBob.png");
	BobL = CP_Image_Load("Assets/IBobL.png");
	immune_timer = 5;
}

void add_health(void)
{
	(health < 3) ? ++health : NULL;
}

void jump_high(void)
{
	maxJumpHeight += 25;
}

void move_fast(void)
{
	speedMultiplier += 0.5;
}

void time_extension(void)
{
	gameTimer += 10.0;
}

void textAbovePlayer(float x, float y, char *text) {
	//For Text On top of Bob
	if (textTimer >= 0) {
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Font_DrawText(text, x + 40, y - 20);
		textTimer -= CP_System_GetDt();
	}
}

void setText(char* text) {
	textTimer = 3;
	memset(textToShow, 0, sizeof(textToShow));
	for (int i = 0; text[i] != '\0'; i++) {
		textToShow[i] = text[i];
	}
}
