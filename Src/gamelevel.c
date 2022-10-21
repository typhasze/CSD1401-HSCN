#include "cprocessing.h"
#include "gamelevel.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include <stdlib.h>



CP_Image Bob,BobL;
double static Bobx = 1280 / 2, Boby = 720 - 90.0f;

void Game_Level_Init() {
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(1280, 720);
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	Bob = CP_Image_Load("Assets/Bob.png");
	BobL = CP_Image_Load("Assets/BobL.png");
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	
	
	
}

void Game_Level_Update() {
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	//CP_Input_KeyDown(KEY_1) ? CP_System_SetWindowSize(windowSizeX, windowSizeY), windowSizeX = 1280, windowSizeY = 720 : 0;
	//CP_Input_KeyDown(KEY_2) ? CP_System_SetWindowSize(windowSizeX, windowSizeY), windowSizeX = 1920, windowSizeY = 1080 : 0;
	//CP_Input_KeyDown(KEY_Q) ? CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit) : 0;
	int currentElapsedTime = CP_System_GetDt()*300;
	int jump = CP_System_GetDt() * 10000;
	int gravity = CP_System_GetDt() * 400;
	float position = currentElapsedTime;
	float jumpposition = jump;
	float gravityposition = gravity;
	CP_Graphics_DrawRect(0, CP_System_GetWindowHeight()-50.0f, CP_System_GetWindowWidth()-280, 50.0f);
	//CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() - 130.0f, 80.0f, 80.0f);
	CP_Image_Draw(Bob, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255);
	if (CP_Input_KeyDown(KEY_A))
	{
		if (Bobx > 40) {
			Bobx -= position;
			CP_Image_Draw(BobL, Bobx, Boby, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255);
		}
		
	}

	if (CP_Input_KeyDown(KEY_D))
	{
		if (Bobx < CP_System_GetWindowWidth() - 40) {
			Bobx += position;
		}
	}
	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		Boby -= jumpposition;
		
	}

	if (Bobx > 1040) {
		if (Boby < CP_System_GetWindowHeight() - 40) {
			Boby += position;
		}
	}

	if (Boby < CP_System_GetWindowHeight() - 90) {
		Boby += gravityposition;
	}

	
	

}

void Game_Level_Exit() {

}





