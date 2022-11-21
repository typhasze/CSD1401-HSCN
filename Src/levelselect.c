#include "cprocessing.h"
#include "mainmenu.h"
#include "gamelevel.h"
#include "utils.h"
#include "animations.h"
#include <stdio.h>


CP_Image IBobL,IBobR, Level1, Level2, Level3, MainMenu, steps, bg;
CP_Color grey;
CP_Sound mainmenusound;
int level_selector;
bool BobDirection;
double Bobx = 1280/2, Boby = 620;

/*
void drawScreen() {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	float halfX = CP_System_GetWindowWidth() / 2;
	float halfY = CP_System_GetWindowHeight() / 2;
	grey = CP_Color_Create(255, 255, 255, 0);
	//Draw Rectangle - Grey
	CP_Settings_Fill(grey);
	CP_Graphics_DrawRect(halfX - 400, halfY, 300.0f, 300.0f);// Level 1 Button
	CP_Graphics_DrawRect(halfX, halfY, 300.0f, 300.0f);// Level 2 Button
	CP_Graphics_DrawRect(halfX + 400, halfY, 300.0f, 300.0f);// Level 3 Button
	CP_Graphics_DrawRect(halfX + 500, halfY - 250, 150.0f, 60.0f);// Main Menu button

	//Text shown over Rectangle Play/Exit - Black
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextAlignment(1280, 720);
	//CP_Font_DrawText("1", halfX - 400, halfY);
	//CP_Font_DrawText("2", halfX, halfY);
	//CP_Font_DrawText("3", halfX + 400, halfY);
	//CP_Font_DrawText("Main Menu", halfX + 400, halfY - 250);
}*/

void Level_Select_Init()
{
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(1280, 720);
	CP_Settings_TextSize(50.0f);
	bg = CP_Image_Load("Assets/pregame_bg.jpg");
	steps = CP_Image_Load("Assets/steps.png");
	IBobL = CP_Image_Load("Assets/BobL.png");
	IBobR = CP_Image_Load("Assets/Bob.png");
	Level1 = CP_Image_Load("Assets/Level1.png");
	Level2 = CP_Image_Load("Assets/Level2.png");
	Level3 = CP_Image_Load("Assets/Level3.png");
	MainMenu = CP_Image_Load("Assets/MainMenu.png");
}

void Level_Select_Update()
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	float halfX = CP_System_GetWindowWidth() / 2;
	float halfY = CP_System_GetWindowHeight() / 2;
	//int static boxClick = 0;

	//if (boxClick == 0);

	//Level 1 Button
	if (CP_Input_MouseClicked()) {
		//Checks for Play button clicked to run the carlevel state
		if (IsAreaClicked(halfX - 400, halfY-150, CP_Image_GetWidth(Level1) / 1.5, CP_Image_GetHeight(Level1) / 1.5, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			level_selector = 1;
			printf("%d", level_selector);
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);
		}

	}

	//Level 2 Button
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(halfX, halfY-150, CP_Image_GetWidth(Level2) / 1.5, CP_Image_GetHeight(Level2) / 1.5, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			level_selector = 2;
			printf("%d", level_selector);
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);// PLS CHANGE
		}

	}

	//Level 3 Button
	if (CP_Input_MouseClicked()) {
		//if (boxClick = IsAreaClicked(halfX + 400, halfY, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		if (IsAreaClicked(halfX + 400, halfY- 150, CP_Image_GetWidth(Level3) / 1.5, CP_Image_GetHeight(Level3) / 1.5, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			level_selector = 3;
		printf("%d", level_selector);
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);// PLS CHANGE
		}
	}

	//Main Menu Button
	if (CP_Input_MouseClicked()) {
		//Checks if Main Menu button clicked to shut down the program.
		if (IsAreaClicked(halfX + 500, halfY + 250, 150.0f, 60.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

	//Draws the graphics for the menu screen.

	CP_Image_Draw(bg, halfX, halfY, CP_Image_GetWidth(bg), CP_Image_GetHeight(bg), 255);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(30);
	CP_Font_DrawText("Press 'A' & 'D' to move left and right", halfX-200, halfY + 150);
	CP_Font_DrawText("SPACEBAR to jump", halfX- 100, halfY + 180);


	//GlowingBob(halfX, halfY + 250, CP_Image_GetWidth(IBobR) + 25, CP_Image_GetWidth(IBobR) + 25);
	

	levelBoxesGlow(halfX - 400, halfY-150, CP_Image_GetWidth(Level1)/1.5, CP_Image_GetHeight(Level1)/1.5);
	CP_Image_Draw(Level1, halfX - 400, halfY-150, CP_Image_GetWidth(Level1)/1.5, CP_Image_GetHeight(Level1)/1.5, 255);

	levelBoxesGlow(halfX, halfY-150, CP_Image_GetWidth(Level2)/1.5, CP_Image_GetHeight(Level2)/1.5);
	CP_Image_Draw(Level2, halfX, halfY-150, CP_Image_GetWidth(Level2)/1.5, CP_Image_GetHeight(Level2)/1.5, 255);

	levelBoxesGlow(halfX+400, halfY-150, CP_Image_GetWidth(Level2)/1.5, CP_Image_GetHeight(Level2)/1.5);
	CP_Image_Draw(Level3, halfX + 400, halfY-150, CP_Image_GetWidth(Level3)/1.5, CP_Image_GetHeight(Level3)/1.5, 255);

	
	CP_Image_Draw(MainMenu, halfX + 500, halfY +250, 150, 60.0f, 255);

	//CP_Graphics_DrawRect(halfX, 720, 1280, 120, 255);
	CP_Image_Draw(steps,halfX, 690, CP_Image_GetWidth(steps), CP_Image_GetHeight(steps) + 20, 255);


	float velocity = CP_System_GetDt() * 400;
	float gravity = CP_System_GetDt() * 400;
	static int jumpCounter = 1, maxJump;
	float jump = CP_System_GetDt() * 1400;

	if (BobDirection == FALSE) {
		CP_Image_Draw(IBobR, Bobx, Boby, CP_Image_GetWidth(IBobR), CP_Image_GetHeight(IBobR), 255);
	}
	else { CP_Image_Draw(IBobL, Bobx, Boby, CP_Image_GetWidth(IBobL), CP_Image_GetHeight(IBobL), 255); }

	if (CP_Input_KeyDown(KEY_A)) {
		Bobx -= velocity;
		BobDirection = TRUE;

	}

	if (CP_Input_KeyDown(KEY_D)) {
		Bobx += velocity;
		BobDirection = FALSE;

	}
	if (Boby <= 620) {
		Boby += gravity;
	}
	if (CP_Input_KeyTriggered(KEY_SPACE) && jumpCounter != 0) {
		//particleEffect(Bobx, Boby, "Jump!");
		--jumpCounter;
		//Boby -= jump;
		maxJump = 175;

	}
	if (Boby >= 620) {
		jumpCounter = 1;
	}
	//Jump CD Decrement every deltaTime
	//jumpCD -= (jumpCD >= 0) ? CP_System_GetDt() : jumpCD;
	maxJump = (Boby <= 10) ? 0 : maxJump;	 //To stop at ceiling
	if (maxJump > 0) {

		Boby -= jump;
		maxJump -= jump;

	}
	
	
}

void Level_Select_Exit()
{
	CP_Sound_Free(&mainmenusound);
}




