#include "cprocessing.h"
#include "mainmenu.h"
#include "splashscreen.h"
#include "gamelevel.h"
#include "utils.h"
#include "levelselect.h"

CP_Color grey;
CP_TEXT_ALIGN_HORIZONTAL horizontal = CP_TEXT_ALIGN_H_CENTER;
CP_TEXT_ALIGN_VERTICAL vertical = CP_TEXT_ALIGN_V_MIDDLE;
CP_Image Bob, Title, Play, HowToPlay, Credits;

void drawMenu() {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	float halfX = CP_System_GetWindowWidth() / 2;
	float halfY = CP_System_GetWindowHeight() / 2;
	grey = CP_Color_Create(255, 255, 255, 0);
	//Draw Rectangle - Red
	CP_Settings_Fill(grey);
	CP_Graphics_DrawRect(halfX, halfY - 50, 300.0f, 100.0f);// Play Button
	CP_Graphics_DrawRect(halfX, halfY + 100, 300.0f, 100.0f);// Instructions Button
	CP_Graphics_DrawRect(halfX, halfY + 250 , 300.0f, 100.0f);// Credits Button
	CP_Graphics_DrawRect(halfX + 500, halfY + 250, 150.0f, 60.0f);// Exit button

	//Text shown over Rectangle Play/Exit - Black
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextAlignment(horizontal, vertical);
	CP_Font_DrawText("Exit", halfX + 500, halfY + 250);
}

void Main_Menu_Init()
{
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(1280, 720);
	CP_Settings_TextSize(50.0f);
	Bob = CP_Image_Load("Assets/Bob.png");
	Title = CP_Image_Load("Assets/Title.png");
	Play = CP_Image_Load("Assets/Play.png");
	HowToPlay = CP_Image_Load("Assets/HowToPlay.png");
	Credits = CP_Image_Load("Assets/Credits.png");
}

void Main_Menu_Update()
{
	CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
	float halfX = CP_System_GetWindowWidth() / 2;
	float halfY = CP_System_GetWindowHeight() / 2;
	int static boxClick = 0;

	//Play Button
	if (CP_Input_MouseClicked()) {
		//Checks for Play button clicked to run the carlevel state
		if (boxClick = IsAreaClicked(halfX, halfY - 50, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_SetNextGameState(Level_Select_Init, Level_Select_Update, Level_Select_Exit);
	}

	//Instructions Button (SCOTT TODO)
	if (CP_Input_MouseClicked()) {
		if (boxClick = IsAreaClicked(halfX, halfY + 100, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);// PLS CHANGE
	}

	// Credits Button
	if (CP_Input_MouseClicked()) {
		if (boxClick = IsAreaClicked(halfX + 500, halfY + 250, 150.0f, 60.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);// TODO
	}

	//Exit Button
	if (CP_Input_MouseClicked()) {
		//Checks if Exit button clicked to shut down the program.
		if (boxClick = IsAreaClicked(halfX + 500, halfY + 250, 150.0f, 60.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_Terminate();
	}
	//Draws the graphics for the menu screen.
	CP_Image_Draw(Bob, 200, halfY, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255);
	CP_Image_Draw(Title, halfX, halfY - 250, CP_Image_GetWidth(Title), CP_Image_GetHeight(Title), 255);
	CP_Image_Draw(Play, halfX, halfY - 50, CP_Image_GetWidth(Play), CP_Image_GetHeight(Play), 255);
	CP_Image_Draw(HowToPlay, halfX, halfY + 100, CP_Image_GetWidth(HowToPlay), CP_Image_GetHeight(HowToPlay), 255);
	CP_Image_Draw(Credits, halfX, halfY + 250, CP_Image_GetWidth(Credits), CP_Image_GetHeight(Credits), 255);
	drawMenu();
}

void Main_Menu_Exit()
{

}