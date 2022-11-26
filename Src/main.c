//---------------------------------------------------------
// main.c
// To run the game
//
// authors:		 Hafiz
// contributors:
// 
//---------------------------------------------------------
// 
// Copyright(c) 2022 DigiPen Institute of Technology
#include "cprocessing.h"
#include "splashscreen.h"
#include "gamelevel.h"
#include "mainmenu.h"
#include "utils.h"

int main(void)
{
	CP_Engine_SetNextGameState(splash_screen_init, splash_screen_update, splash_screen_exit);
	CP_Engine_Run();
	return 0;
}
