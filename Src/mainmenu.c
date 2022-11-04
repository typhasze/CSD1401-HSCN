#include "cprocessing.h"
#include "mainmenu.h"
#include "splashscreen.h"
#include "gamelevel.h"
#include "utils.h"

CP_Color red;
CP_TEXT_ALIGN_HORIZONTAL horizontal = CP_TEXT_ALIGN_H_CENTER;
CP_TEXT_ALIGN_VERTICAL vertical = CP_TEXT_ALIGN_V_MIDDLE;
int level_selector;

void drawMenu() {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	float halfX = CP_System_GetWindowWidth() / 2;
	float halfY = CP_System_GetWindowHeight() / 2;
	red = CP_Color_Create(255, 0, 0, 255);
	//Draw Rectangle - Red
	CP_Settings_Fill(red);
	CP_Graphics_DrawRect(halfX, halfY + 100, 150.0f, 100.0f);
	CP_Graphics_DrawRect(halfX, halfY - 100, 150.0f, 100.0f);

	//Text shown over Rectangle Play/Exit - Black
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextAlignment(horizontal, vertical);
	CP_Font_DrawText("Play", halfX, halfY - 100);
	CP_Font_DrawText("Exit", halfX, halfY + 100);
}

void Main_Menu_Init()
{
	//Set window size to 1280 x 720
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(windowSizeX, windowSizeY);
	CP_Settings_TextSize(50.0f);
}

void Main_Menu_Update()
{
	CP_Graphics_ClearBackground(CP_Color_Create(192, 192, 192, 255));
	float halfX = CP_System_GetWindowWidth() / 2;
	float halfY = CP_System_GetWindowHeight() / 2;
	int static boxClick = 0;

	//Change Resolution
	CP_Input_KeyDown(KEY_1) ? CP_System_SetWindowSize(windowSizeX, windowSizeY), windowSizeX = 1280, windowSizeY = 720 : NULL;
	CP_Input_KeyDown(KEY_2) ? CP_System_SetWindowSize(windowSizeX, windowSizeY), windowSizeX = 1920, windowSizeY = 1080 : NULL;

	//Play Button
	if (CP_Input_MouseClicked()) {
		//Checks for Play button clicked to run the carlevel state
		if (boxClick = IsAreaClicked(halfX, halfY - 100, 150.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);
			level_selector = 1;
		}
	}

	//Exit Button
	if (CP_Input_MouseClicked()) {
		//Checks if Exit button clicked to shut down the program.
		if (boxClick = IsAreaClicked(halfX, halfY + 100, 150.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_Terminate();
	}
	//Draws the graphics for the menu screen.
	drawMenu();
}

void Main_Menu_Exit()
{

}