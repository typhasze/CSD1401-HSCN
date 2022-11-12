#include "animations.h"

CP_Image glowdefault;
CP_Image Bob, BobL;

void glowingBob(float x, float y, bool BobImmune) {
	static float alpha = 255;
	static bool flip = FALSE;
	if (flip == FALSE) {
		alpha -= 255 * CP_System_GetDt();
		flip = (alpha <= 0) ? TRUE : flip;
	}
	else {
		alpha += 255 * CP_System_GetDt();
		flip = (alpha >= 255 ) ? FALSE : flip;
	}
	glowdefault = (!BobImmune) ? CP_Image_Load("Assets/glowdefault.png") : CP_Image_Load("Assets/iglowdefault.png");
	CP_Image_Draw(glowdefault, x-10, y-10, 100, 100, alpha);

}

void drawBob(float x, float y, bool BobDirection, bool BobImmune) {
	if (BobImmune) {
		Bob = CP_Image_Load("Assets/IBob.png");
		BobL = CP_Image_Load("Assets/IBobL.png");

	} else {
		Bob = CP_Image_Load("Assets/Bob.png");
		BobL = CP_Image_Load("Assets/BobL.png");
	}
	glowingBob(x, y, BobImmune);
	(BobDirection == FALSE) ? CP_Image_Draw(Bob, x, y, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255)
		: CP_Image_Draw(BobL, x, y, CP_Image_GetWidth(Bob), CP_Image_GetHeight(Bob), 255);
	
}


void blastRadius(float x, float y) {
	static float old_x, old_y;
	if (y > 0) {
		old_y = y;
		old_x = x;
	}
	static float radius = 255;
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Graphics_DrawCircle(old_x, old_y, radius);

}

void lostHealth(int hp, int *p) {
	static float alpha;
	static bool flip = FALSE;
	if (hp != *p) {
		//So when u lose hp
		if (hp = *p - 1) {
			flip = TRUE;
			alpha = 85;
		}
		*p = hp;
	}
	if (flip) {
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, alpha));
		CP_Graphics_DrawRect(0, 0, 1280, 720);
		alpha -= 127.5 * CP_System_GetDt();
		flip = (alpha <= 0) ? FALSE : flip;
	}
}
