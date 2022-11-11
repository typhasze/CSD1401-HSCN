#include "cprocessing.h"
#include "mainmenu.h"
#include "gamelevel.h"
#include "utils.h"


CP_Image IBob, Level1, Level2, Level3, MainMenu;
CP_Color grey;
int level_selector;

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
}

void Level_Select_Init()
{
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(1280, 720);
	CP_Settings_TextSize(50.0f);
	IBob = CP_Image_Load("Assets/IBob.png");
	Level1 = CP_Image_Load("Assets/Level1.png");
	Level2 = CP_Image_Load("Assets/Level2.png");
	Level3 = CP_Image_Load("Assets/Level3.png");
	MainMenu = CP_Image_Load("Assets/MainMenu.png");
}

void Level_Select_Update()
{
	CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
	float halfX = CP_System_GetWindowWidth() / 2;
	float halfY = CP_System_GetWindowHeight() / 2;
	int static boxClick = 0;

	//Level 1 Button
	if (CP_Input_MouseClicked()) {
		//Checks for Play button clicked to run the carlevel state
		if (boxClick = IsAreaClicked(halfX - 400, halfY, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			level_selector = 1;
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);
		}

	}

	//Level 2 Button
	if (CP_Input_MouseClicked()) {
		if (boxClick = IsAreaClicked(halfX, halfY, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			level_selector = 2;
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);// PLS CHANGE
		}

	}

	//Level 3 Button
	if (CP_Input_MouseClicked()) {
		if (boxClick = IsAreaClicked(halfX + 400, halfY, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			level_selector = 3;
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);// PLS CHANGE
	}

	//Main Menu Button
	if (CP_Input_MouseClicked()) {
		//Checks if Main Menu button clicked to shut down the program.
		if (boxClick = IsAreaClicked(halfX + 500, halfY - 250, 150.0f, 60.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	//Draws the graphics for the menu screen.
	CP_Image_Draw(IBob, halfX , halfY + 250 , CP_Image_GetWidth(IBob), CP_Image_GetHeight(IBob), 255);
	CP_Image_Draw(Level1, halfX - 400, halfY, CP_Image_GetWidth(Level1), CP_Image_GetHeight(Level1), 255);
	CP_Image_Draw(Level2, halfX, halfY, CP_Image_GetWidth(Level2), CP_Image_GetHeight(Level2), 255);
	CP_Image_Draw(Level3, halfX + 400, halfY, CP_Image_GetWidth(Level3), CP_Image_GetHeight(Level3), 255);
	CP_Image_Draw(MainMenu, halfX + 500, halfY - 250, CP_Image_GetWidth(MainMenu), CP_Image_GetHeight(MainMenu), 255);
	drawScreen();
}

void Level_Select_Exit()
{

}