#include "cprocessing.h"
#include "gamelevel.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include <stdlib.h>

typedef struct Items {
	int x[50], y[50], d;
}Items;

Items yellowOrb, purpleOrb;
int score;

void Game_Level_Init() {
	CP_System_SetFrameRate(60);
	CP_System_SetWindowSize(1920, 1080);
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	yellowOrb.x[0] = randNum(), yellowOrb.y[0] = randNum();
}

void Game_Level_Update() {
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	//CP_Input_KeyDown(KEY_1) ? CP_System_SetWindowSize(windowSizeX, windowSizeY), windowSizeX = 1280, windowSizeY = 720 : 0;
	//CP_Input_KeyDown(KEY_2) ? CP_System_SetWindowSize(windowSizeX, windowSizeY), windowSizeX = 1920, windowSizeY = 1080 : 0;
	CP_Input_KeyDown(KEY_Q) ? CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit) : 0;

	//yellowOrb.x[0] = randNum(), yellowOrb.y[0] = randNum();
	yellowOrb.d = 30;
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Graphics_DrawCircle(yellowOrb.x[0], yellowOrb.y[0], yellowOrb.d);

}

void Game_Level_Exit() {

}

int randNum() {
	int lower = 0, upper = 1920;
	int num = (rand() % (upper - lower + 1)) + lower;
}



