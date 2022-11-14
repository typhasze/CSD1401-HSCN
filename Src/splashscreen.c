#include "cprocessing.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include <stdio.h>
#include <stdlib.h>

CP_Image logo;
CP_Color red;
int windowSizeX = 1280, windowSizeY = 720;

void splash_screen_init(void)
{
	//Set to 60fps
	CP_System_SetFrameRate(60.0f);
	logo = CP_Image_Load("Assets/DigiPen_BLACK.png");
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_REPEAT);
	CP_System_SetWindowSize(windowSizeX, windowSizeY);
}

void splash_screen_update(void)
{
	/*
	Takes 2 Seconds(system runtime) to reach from 0 to 255
	Need a multiplier to reach 255 with system runtime = 255/2.00s = 127.5
	*/
	double logoW = CP_Image_GetWidth(logo) * windowSizeX / 1920
		, logoH = CP_Image_GetHeight(logo) * windowSizeY / 1080;
	float currentElapsedTime = CP_System_GetDt() * 127.5;
	static float totalElapsedTime = 0;
	totalElapsedTime += currentElapsedTime;
	//totalElapsedTime = (int) totalElapsedTime % 255;
	//Display Image
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Image_Draw(logo, windowSizeX / 2, windowSizeY / 2, logoW, logoH, totalElapsedTime);

	CP_Input_KeyDown(KEY_1) ? splash_screen_init(), windowSizeX = 1280, windowSizeY = 720 : NULL;
	CP_Input_KeyDown(KEY_2) ? splash_screen_init(), windowSizeX = 1920, windowSizeY = 1080 : NULL;

	red = CP_Color_Create(255, 0, 0, 255);

	//This function sets the color of all subsequently drawn shapes or text.
	//CP_Settings_Fill(red);
	//Draw a circle at the current mouse pointer
	//CP_Graphics_DrawCircle(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 20.0f);

	if (totalElapsedTime > 255) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}

}

void splash_screen_exit(void)
{
	CP_Image_Free(&logo);
}