#include "cprocessing.h"
#include "splashscreen.h"
#include "gamelevel.h"


int main(void)
{
	CP_Engine_SetNextGameState(Game_Level_Init, Game_Level_Update, Game_Level_Exit);
	CP_Engine_Run();
	return 0;
}
