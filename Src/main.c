#include "cprocessing.h"
#include "splashscreen.h"
#include "gamelevel.h"
#include "mainmenu.h"
#include "utils.h"

int main(void)
{
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit, NULL);
	//CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);
	CP_Engine_Run();
	return 0;
}
