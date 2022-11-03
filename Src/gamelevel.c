#include "cprocessing.h"
#include "gamelevel.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

CP_Image Bob, BobL, heart, chest, Bomb, fail_screen, clear_screen, pause_menu;
double Bobx, Boby;
int BobWidth, BobHeight;
// Volatile Variables for Game 
int health, points, multiplier, multiplierCombo;
double gameTimer, multiplierTimer;
// Variables for Movement
int velocity, jump, gravity;
float currentElapsedTime, position;
static int jumpCounter;

// Variables for Platform Creation
float platformX[100], platformY[100], platformWidth[100], platformHeight = 50.0f;
int no_of_platforms = 6;
static double maxJump = 0, jumpCD = 0;
// Pause and BobStagnant 
bool gIsPaused, BobDirection;
//for balls
static int purpleBalls[3];
static int yellowBalls[3];
static int bomb[3];

//For Chest
int cheststate = 1;
static int chestX, chestY;

struct Items
{
	int x, y;
	float timer_to_drop, timer_on_floor;
};
struct Items pOrbs[10], yOrbs[10], bOrbs[10];

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
	chest = CP_Image_Load("Assets/Chest.png");
	Bomb = CP_Image_Load("Assets/Bomb.png");
	fail_screen = CP_Image_Load("Assets/fail.png"); clear_screen = CP_Image_Load("Assets/clear.png"); pause_menu = CP_Image_Load("Assets/pause.png");
	BobWidth = CP_Image_GetWidth(Bob), BobHeight = CP_Image_GetHeight(Bob);
	//Resets Timer/Health/Points/Multiplier/Bob Position/Unpause Game
	gameTimer = 60.0, health = 3, points = 0, multiplier = 1, multiplierTimer = 5, multiplierCombo = 0;
	gIsPaused = FALSE, BobDirection = FALSE; Bobx = 1280 / 2, Boby = 720 / 2;

	//Movements by Hafiz
	currentElapsedTime = CP_System_GetDt() * 300;
	static double maxJump = 0, jumpCD = 0;
	jumpCounter = 2;
	position = currentElapsedTime;

	//Base Platform
	createPlatformXY();
	setOrbInitialPosition();
	for (int i = 0; i < 3; i++)
	{
		purpleBalls[i] = (rand() % (1200 + 1 - 25) + 25);
	}

	for (int i = 0; i < 3; i++)
	{
		yellowBalls[i] = (rand() % (1200 + 1 - 25) + 25);
	}

	for (int i = 0; i < 3; i++)
	{
		bomb[i] = (rand() % (1200 + 1 - 25) + 25);
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

		//Draw Chest
		if (gameTimer <= 45 && gameTimer >= 40 || gameTimer <= 25 && gameTimer >= 20)
		{
			drawTreasureChest();
			CP_Settings_ImageMode(CP_POSITION_CORNER);
			CP_Image_Draw(chest, chestX, chestY, CP_Image_GetWidth(chest), CP_Image_GetHeight(chest), 255);
			ChestCollision();
			power_up();
		}

		if (gameTimer <= 40 && gameTimer >= 25)
		{
			cheststate = 1;
		}

		switch (gIsPaused) {
		case TRUE: //Game is paused
			Clear_Fail_Pause();
			break;

		case FALSE: //Game not paused
			playerMovement();			//checks input for Movement
			scoreMultiplier();	//Multiplier Logic
			gameTimer -= CP_System_GetDt();	//Game Timer Reduction
			drawOrbs();
			break;
		}
	}
	//TESTCODE
	{
		//TO REMOVE: Test Health Increment and Cap at 3
		CP_Input_KeyTriggered(KEY_1) && (health > 0) ? --health : NULL;	//CP_Input_KeyTriggered(KEY_1) can be replaced for collision w/ bomb
		CP_Input_KeyTriggered(KEY_3) ? multiplierTimer = 5.00, points += 1 * multiplier, multiplierCombo++ : multiplierTimer;
	}

	//purpleOrb();

	//yellowOrb();
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
	speed = CP_System_GetDt() * 100;
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

// Logic For Player Movement
void playerMovement() {
	jump = CP_System_GetDt() * 1500;
	gravity = CP_System_GetDt() * 500;

	//For Player if not jumping and in air
	int collidedPlatform = -1;
	Boby += gravity;
	collidedPlatform = playerPlatformCollision();
	printf("collision is %d", collidedPlatform);
	if (collidedPlatform >= 0)
	{
		Boby = platformY[collidedPlatform] - BobHeight - 1;
		//Boby -= gravity;
	}
	collidedPlatform = -1;

	if (CP_Input_KeyDown(KEY_A))
	{
		Bobx -= position;
		BobDirection = TRUE;

		collidedPlatform = playerPlatformCollision();
		if (collidedPlatform >= 0) {
			Bobx = platformX[collidedPlatform] + platformWidth[collidedPlatform] + 1;
		}
		collidedPlatform = -1;
	}

	if (CP_Input_KeyDown(KEY_D))
	{
		Bobx += position;
		BobDirection = FALSE;

		collidedPlatform = playerPlatformCollision();
		if (collidedPlatform >= 0) {
			Bobx = platformX[collidedPlatform] - 1 - BobWidth;
		}
		collidedPlatform = -1;
	}

	//Only Jump when
	if (CP_Input_KeyTriggered(KEY_SPACE) && jumpCD <= 0 && jumpCounter != 0)
	{
		--jumpCounter;
		maxJump = 200;
		if (jumpCounter == 0) {
			jumpCD = .75;
			jumpCounter = 2;
		}
	}
	//Jump CD Decrement every deltaTime
	jumpCD -= (jumpCD >= 0) ? CP_System_GetDt() : jumpCD;

	if (maxJump > 0) {
		if (playerPlatformCollision() == 3) {
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
		printf("%f\n", jumpCD);
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

void drawOrbs() {
	CP_Color purple = CP_Color_Create(255, 0, 255, 255);
	CP_Color yellow = CP_Color_Create(255, 255, 0, 255);
	CP_Color red = CP_Color_Create(255, 0, 0, 255);
	CP_Settings_Fill(purple);
	for (int i = 0; i < 10; i++) {
		CP_Settings_Fill(purple);
		CP_Graphics_DrawCircle(pOrbs[i].x, pOrbs[i].y, 50);
		CP_Settings_Fill(yellow);
		CP_Graphics_DrawCircle(yOrbs[i].x, yOrbs[i].y, 50);
		{
			CP_Settings_Fill(red);
			CP_Graphics_DrawCircle(bOrbs[i].x, bOrbs[i].y, 50);
			CP_Image_Draw(Bomb, bOrbs[i].x - 25, bOrbs[i].y -25, CP_Image_GetWidth(Bomb), CP_Image_GetHeight(Bomb), 255);
		}

	}
	makeOrbsFall();
	orbOnFloor();
	orbsCollected();
}

void setOrbInitialPosition() {
	for (int i = 0; i < 10; i++) {
		pOrbs[i].x = rand() % 1280, pOrbs[i].y = 0 - 25;
		yOrbs[i].x = rand() % 1280, yOrbs[i].y = 0 - 25;
		bOrbs[i].x = rand() % 1280, bOrbs[i].y = 0 - 25;
		pOrbs[i].timer_to_drop = rand() % 20;
		yOrbs[i].timer_to_drop = rand() % 20;
		bOrbs[i].timer_to_drop = rand() % 20;
		pOrbs[i].timer_on_floor = 2;
		yOrbs[i].timer_on_floor = 5;
		bOrbs[i].timer_on_floor = 0;
	}
}

void makeOrbsFall() {
	int yDropSpeed = CP_System_GetDt() * 250;
	int pDropSpeed = CP_System_GetDt() * 200;
	int bDropSpeed = CP_System_GetDt() * 150;
	for (int i = 0; i < 10; i++) {
		if (pOrbs[i].timer_to_drop < 0 && pOrbs[i].timer_on_floor == 2) {
			pOrbs[i].y += pDropSpeed;
		} 
		else pOrbs[i].timer_to_drop -= CP_System_GetDt();
		if (yOrbs[i].timer_to_drop < 0 && yOrbs[i].timer_on_floor == 5) {
			yOrbs[i].y += yDropSpeed;
		} 
		else yOrbs[i].timer_to_drop -= CP_System_GetDt();
		if (bOrbs[i].timer_to_drop < 0 && bOrbs[i].timer_on_floor == 0)
		{
			bOrbs[i].y += bDropSpeed;
		}
		else bOrbs[i].timer_to_drop -= CP_System_GetDt();

		//if DISSAPEARED(OOB or Too Long On Floor) or COLLECTED (TODO)
		(pOrbs[i].y > 720 || pOrbs[i].timer_on_floor < 0) ? pOrbs[i].timer_to_drop = rand() % 20,
			pOrbs[i].y = -25, pOrbs[i].x = rand() % 1280, pOrbs[i].timer_on_floor = (float)2 : 0;
		(yOrbs[i].y > 720 || yOrbs[i].timer_on_floor < 0) ? yOrbs[i].timer_to_drop = rand() % 20,
			yOrbs[i].y = -25, yOrbs[i].x = rand() % 1280, yOrbs[i].timer_on_floor = (float)5 : 0;
		(bOrbs[i].y > 720 || pOrbs[i].timer_on_floor < 0) ? pOrbs[i].timer_to_drop = rand() % 5,
			bOrbs[i].y = -25, pOrbs[i].x = rand() % 1280, pOrbs[i].timer_on_floor = (float)1 : 0;
	}
}

void orbOnFloor() {
	for (int i = 0; i < 10; i++) {
		for (int x = 0; x < no_of_platforms; x++) {
			//for purple
			if (circleCollision(pOrbs[i].x, pOrbs[i].y, 50, platformX[x], platformY[x], platformWidth[x], platformHeight) == 1) {
				pOrbs[i].timer_on_floor -= CP_System_GetDt();
			}
			//for yellow
			if (circleCollision(yOrbs[i].x, yOrbs[i].y, 50, platformX[x], platformY[x], platformWidth[x], platformHeight) == 1) {
				yOrbs[i].timer_on_floor -= CP_System_GetDt();
			}
				//for bomb
			if (circleCollision(yOrbs[i].x, bOrbs[i].y, 50, platformX[x], platformY[x], platformWidth[x], platformHeight) == 1) {
				bOrbs[i].timer_on_floor -= CP_System_GetDt();
			}
		}
	}
}

void orbsCollected(void) {
	for (int i = 0; i < 10; i++) {
		//for purple
		if (circleCollision(pOrbs[i].x, pOrbs[i].y, 50, Bobx, Boby, BobWidth, BobHeight) == 1) {
			pointsCollected(5);
			pOrbs[i].timer_to_drop = rand() % 20,
				pOrbs[i].y = -25, pOrbs[i].x = rand() % 1280, pOrbs[i].timer_on_floor = (float)2;			
		}
		//for yellow
		if (circleCollision(yOrbs[i].x, yOrbs[i].y, 50, Bobx, Boby, BobWidth, BobHeight) == 1) {
			pointsCollected(5);
			yOrbs[i].timer_to_drop = rand() % 20,
				yOrbs[i].y = -25, yOrbs[i].x = rand() % 1280, yOrbs[i].timer_on_floor = (float)5;
		}
		if (circleCollision(bOrbs[i].x, bOrbs[i].y, 50, Bobx, Boby, BobWidth, BobHeight) == 1) {
			health--;
			bOrbs[i].timer_to_drop = rand() % 30,
				bOrbs[i].y = -25, bOrbs[i].x = rand() % 1280, bOrbs[i].timer_on_floor = (float)10;
		}
	}
}

//Treasure Chest
void drawTreasureChest()
{
	if (cheststate == 1)
	{
		chestX = 500;
		chestY = 620;
		CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
		CP_Graphics_DrawRect(chestX, chestY, 50, 50);
	}
	if (cheststate == 0)
	{
		chestX = 2000, chestY = 2000;
		CP_Graphics_DrawRect(chestX, chestY, 0, 0);
	}
}

int ChestCollision()
{
	if (Boby + BobHeight >= 620 && Boby <= 670 && Bobx + BobWidth > 500 && Bobx < 550)
	{
		//power_up();
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
			immunity();
		}
		else if (powerup == 2)
		{
			add_health();
		}
		else if (powerup == 3)
		{
			jump_high();
		}
		else if (powerup == 4)
		{
			move_fast();
		}
		else if (powerup == 5)
		{
			time_extension();
		}
}

void immunity(void)
{
	for (int i = 0; i < 10; i++) {
		if (circleCollision(bOrbs[i].x, bOrbs[i].y, 50, Bobx, Boby, BobWidth, BobHeight) == 1) {
			health-=0;
			bOrbs[i].timer_to_drop = rand() % 30,
				bOrbs[i].y = -25, bOrbs[i].x = rand() % 1280, bOrbs[i].timer_on_floor = (float)10;
		}
	}
}

void add_health(void)
{
	(health < 3) ? ++health : NULL;
}

void jump_high(void)
{
	maxJump = 350;
}

void move_fast(void)
{
	position *= 1.5;
}

void time_extension(void)
{
	gameTimer += 10.0;
}
