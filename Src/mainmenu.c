#include "cprocessing.h"
#include "mainmenu.h"
#include "splashscreen.h"
#include "gamelevel.h"
#include "utils.h"
#include "levelselect.h"
#include <stdbool.h>

CP_Color grey;
CP_TEXT_ALIGN_HORIZONTAL horizontal = CP_TEXT_ALIGN_H_CENTER;
CP_TEXT_ALIGN_VERTICAL vertical = CP_TEXT_ALIGN_V_MIDDLE;
int level_selector;
CP_Image Bob,BobL,glow, Title, Play, HowToPlay, Credits, background, intro, mainmenu, cross, credits1, credits2, credits3;
CP_Sound mainmenusound;
static int alpha = 255;
static int mainmenustate = 0;
int static boxClick = 0;
int static credPage = 1;

void drawMenu() {
	
	CP_Settings_RectMode(CP_POSITION_CENTER);
	float halfX = (float)CP_System_GetWindowWidth() / 2;
	float halfY = (float)CP_System_GetWindowHeight() / 2;
	grey = CP_Color_Create(255, 255, 255, 0);
	//Draw Rectangle - Red
	CP_Settings_Fill(grey);
	CP_Graphics_DrawRect(halfX, halfY - 50, 300.0f, 100.0f);// Play Button
	CP_Graphics_DrawRect(halfX, halfY + 100, 300.0f, 100.0f);// Instructions Button
	CP_Graphics_DrawRect(halfX, halfY + 250, 300.0f, 100.0f);// Credits Button
	CP_Graphics_DrawRect(halfX + 500, halfY + 250, 150.0f, 60.0f);// Exit button

	//Draws the graphics for the menu screen.
	CP_Image_Draw(background, halfX, halfY, (float)CP_Image_GetWidth(background), (float)CP_Image_GetHeight(background), alpha);
	CP_Image_Draw(BobL, halfX - 450, halfY - 250, (float)CP_Image_GetWidth(BobL), (float)CP_Image_GetHeight(BobL), 255);
	CP_Image_Draw(glow, halfX - 450, halfY - 250, (float)CP_Image_GetWidth(glow), (float)CP_Image_GetHeight(glow), 255);
	CP_Image_Draw(Bob, halfX + 500, halfY - 250, (float)CP_Image_GetWidth(Bob), (float)CP_Image_GetHeight(Bob), 255);
	CP_Image_Draw(glow, halfX + 500, halfY - 250, (float)CP_Image_GetWidth(glow), (float)CP_Image_GetHeight(glow), 255);
	CP_Image_Draw(Play, halfX, halfY - 50, (float)CP_Image_GetWidth(Play), (float)CP_Image_GetHeight(Play), alpha);
	CP_Image_Draw(HowToPlay, halfX, halfY + 100, (float)CP_Image_GetWidth(HowToPlay), (float)CP_Image_GetHeight(HowToPlay), alpha);
	CP_Image_Draw(Credits, halfX, halfY + 250, (float)CP_Image_GetWidth(Credits), (float)CP_Image_GetHeight(Credits), alpha);
}

void drawIntro() {
	float halfX = (float)CP_System_GetWindowWidth() / 2;
	float halfY = (float)CP_System_GetWindowHeight() / 2;

	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 0));
	CP_Graphics_DrawRect(1200, 60, 40, 40);
	CP_Image_Draw(intro, halfX, halfY, (float)CP_Image_GetWidth(background), (float)CP_Image_GetHeight(background), 255);
	CP_Image_Draw(mainmenu, halfX + 400, halfY + 250, (float)CP_Image_GetWidth(mainmenu)/2.f, (float)CP_Image_GetHeight(mainmenu)/2.f, 255);
	CP_Image_Draw(background, halfX, halfY, (float)CP_Image_GetWidth(background), (float)CP_Image_GetHeight(background), 20);
	CP_Image_Draw(cross, halfX + 400, halfY - 500, (float)CP_Image_GetWidth(cross), (float)CP_Image_GetHeight(cross), 255);
	CP_Image_Draw(Play, halfX, halfY - 50, (float)CP_Image_GetWidth(Play), (float)CP_Image_GetHeight(Play), 20);
	CP_Image_Draw(HowToPlay, halfX, halfY + 100, (float)CP_Image_GetWidth(HowToPlay), (float)CP_Image_GetHeight(HowToPlay), 20);
	CP_Image_Draw(Credits, halfX, halfY + 250, (float)CP_Image_GetWidth(Credits), (float)CP_Image_GetHeight(Credits), 20);
	CP_Image_Draw(cross, halfX + 400, halfY - 500, (float)CP_Image_GetWidth(cross), (float)CP_Image_GetHeight(cross), 255);
	if (CP_Input_MouseClicked()) {
		if (boxClick = IsAreaClicked(halfX + 400, halfY + 250, (float)CP_Image_GetWidth(mainmenu) / 2.f, (float)CP_Image_GetHeight(mainmenu) / 2.f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			mainmenustate = 1;
		}
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		mainmenustate = 1;
	}

}

void drawCred() {
	float halfX = (float)CP_System_GetWindowWidth() / 2;
	float halfY = (float)CP_System_GetWindowHeight() / 2;
	if (CP_Input_MouseClicked())
	{
		if (IsAreaClicked(1225, 475, 50, 50, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
		{
			credPage++;
		}
	}
	if (credPage == 1)
	{
		CP_Image_Draw(credits1, halfX, halfY, 1280, 720, 255);
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 0));
		CP_Graphics_DrawRect(1200, 60, 40, 40);
		CP_Image_Draw(cross, 1200, 60, 40, 40, 255);
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
		CP_Graphics_DrawTriangle(1200, 500, 1200, 450, 1250, 475);
	}
	if (credPage == 2)
	{
		CP_Image_Draw(credits2, halfX, halfY, 1280, 720, 255);
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 0));
		CP_Graphics_DrawRect(1200, 60, 40, 40);
		CP_Image_Draw(cross, 1200, 60, 40, 40, 255);
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
		CP_Graphics_DrawTriangle(1200, 500, 1200, 450, 1250, 475);
	}
	if (credPage == 3)
	{
		CP_Image_Draw(credits3, halfX, halfY, 1280, 720, 255);
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 0));
		CP_Graphics_DrawRect(1200, 60, 40, 40);
		CP_Image_Draw(cross, 1200, 60, 40, 40, 255);
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
		CP_Graphics_DrawTriangle(1200, 500, 1200, 450, 1250, 475);
	}
	if (credPage > 3)
	{
		credPage = 1;
	}
	if (CP_Input_MouseClicked()) {
		if (boxClick = IsAreaClicked(1200, 60, 40, 40, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			mainmenustate = 1;
		}
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		mainmenustate = 1;
	}

}

void Main_Menu_Init()
{
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(1280, 720);
	CP_Settings_TextSize(50.0f);
	glow = CP_Image_Load("Assets/glowdefault.png");
	Bob = CP_Image_Load("Assets/Bob.png");
	BobL = CP_Image_Load("Assets/BobL.png");
	Title = CP_Image_Load("Assets/thoseorbsarefine.png");
	Play = CP_Image_Load("Assets/Play.png");
	HowToPlay = CP_Image_Load("Assets/HowToPlay.png");
	Credits = CP_Image_Load("Assets/Credits.png");
	background = CP_Image_Load("Assets/mainbackground.jpg");
	intro = CP_Image_Load("Assets/Tutorial.jpg");
	mainmenu = CP_Image_Load("Assets/MainMenu.png");
	cross = CP_Image_Load("Assets/Cross.png");
	credits1 = CP_Image_Load("Assets/Credits1.png");
	credits2 = CP_Image_Load("Assets/Credits2.png");
	credits3 = CP_Image_Load("Assets/Credits3.png");
	mainmenusound = CP_Sound_Load("Assets/jazzyfrenchy.mp3");
	CP_Sound_PlayAdvanced(mainmenusound, 0.1f, 1.0f, TRUE, CP_SOUND_GROUP_0);
	mainmenustate = 1;
}

void Main_Menu_Update()
{
	CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
	float halfX = (float)CP_System_GetWindowWidth() / 2;
	float halfY = (float)CP_System_GetWindowHeight() / 2;

	//Play Button
	if (CP_Input_MouseClicked()) {
		//Checks for Play button clicked to run the carlevel state
		if (boxClick = IsAreaClicked(halfX, halfY - 100, 150.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);
			level_selector = 1;
		}
		if (boxClick = IsAreaClicked(halfX, halfY - 50, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			CP_Engine_SetNextGameState(Level_Select_Init, Level_Select_Update, Level_Select_Exit);
	}

	//Instructions Button
	if (CP_Input_MouseClicked()) {
		if (boxClick = IsAreaClicked(halfX, halfY + 100, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			mainmenustate = 2;
		}

	}

	// Credits Button
	if (CP_Input_MouseClicked()) {
		if (boxClick = IsAreaClicked(halfX, halfY + 250, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
			mainmenustate = 3;
	}

	//Exit Button
	if(CP_Input_KeyTriggered(KEY_Q)) {
		CP_Engine_Terminate();
	}

	if (mainmenustate == 1) {
		drawMenu();
	}
	else if (mainmenustate == 2) {
		drawIntro();
	}
	else if (mainmenustate == 3) {
		drawCred();
	}
}

void Main_Menu_Exit()
{

}

