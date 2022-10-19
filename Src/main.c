#include "cprocessing.h"
#include "splashscreen.h"


int main(void)
{
	CP_Engine_SetNextGameState(splash_screen_init, splash_screen_update, splash_screen_exit);
	CP_Engine_Run();
	return 0;
}
