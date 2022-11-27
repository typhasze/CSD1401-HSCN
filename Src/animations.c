//---------------------------------------------------------
// animations.c
// Code for animations in the game
//
// authors:		 Hafiz
// contributors: 
//
//	Hafiz:
// - Animations
// - Effects (Glowing Characters) / Lose HP Effect
// 
//---------------------------------------------------------
// 
// Copyright(c) 2022 DigiPen Institute of Technology
#include "animations.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

CP_Image glowdefault, LevelBoxesGlow;
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
	CP_Image_Draw(glowdefault, x-10, y-10, 100, 100, (int)alpha);

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
	(BobDirection == FALSE) ? CP_Image_Draw(Bob, x, y, (float)CP_Image_GetWidth(Bob), (float)CP_Image_GetHeight(Bob), 255)
		: CP_Image_Draw(BobL, x, y, (float)CP_Image_GetWidth(Bob), (float)CP_Image_GetHeight(Bob), 255);
	
}

void lostHealth(int hp, int *p) {
	static float alpha;
	static int flip = 0;
	if (hp != *p) {
		//So when u lose hp
		if (hp ==  (*p) - 1) {
			flip = 1;
			alpha = 85;
		}
		*p = hp;
	}
	if (flip) {
		CP_Settings_RectMode(CP_POSITION_CORNER);
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, (int)alpha));
		CP_Graphics_DrawRect(0, 0, 1280, 720);
		alpha -= 85 * CP_System_GetDt();
		flip = (alpha <= 0) ? FALSE : flip;
	}
}

void levelBoxesGlow(float x, float y, float w, float h) {
	float static alpha = 255;
	static bool toggle = FALSE;
	LevelBoxesGlow = CP_Image_Load("Assets/LevelBoxesGlow.png");
	CP_Image_Draw(LevelBoxesGlow, x, y, w, h, (int)alpha);
	//wrapAlphaValue(&alpha, &toggle, 125);
	if (toggle == FALSE) {
		alpha -= 40 * CP_System_GetDt();
		toggle = (alpha <= 0) ? TRUE : toggle;
	}
	else {
		alpha += 40 * CP_System_GetDt();
		toggle = (alpha >= 255) ? FALSE : toggle;
	}
}

void GlowingBob(float x, float y, float w, float h) {
	static float alpha = 255;
	static bool flip = FALSE;
	if (flip == FALSE) {
		alpha -= 127.5f * CP_System_GetDt();
		flip = (alpha <= 0) ? TRUE : flip;
	}
	else {
		alpha += 127.5f * CP_System_GetDt();
		flip = (alpha >= 255) ? FALSE : flip;
	}
	glowdefault = CP_Image_Load("Assets/iglowdefault.png");
	CP_Image_Draw(glowdefault, x, y, w, h, (int)alpha);
}

void pointsToReach(int points) {
	static int checkPoints[] = { 750, 1200, 1600 };
	static int i = 0;
	i = (points < 750) ? 0 : i;
	i = (points < 1200 && points >= 750) ? 1 : i;
	i = (points < 1600 && points >= 1200) ? 2 : i;
	char PointsToReach[15] = { 0 };
	sprintf_s(PointsToReach, _countof(PointsToReach), "%i / %i", points, checkPoints[i]);
	CP_Font_DrawText(PointsToReach, 5, 50);
	starRatings(points);

}

void starRatings(int points) {
	CP_Image stars = CP_Image_Load("Assets/IBob.png");
	static int i;
	int star = 0;
	//750, 1200, 1600
	star = (points >= 750) ? 1 : star;
	star = (points >= 1200) ? 2 : star;
	star = (points >= 1600) ? 3 : star;
	for (int x = 5, i = 1; i <= star; i++, x += 50) {
		CP_Settings_ImageMode(CP_POSITION_CORNER);
		CP_Image_Draw(stars, (float)x, 75, 40, 40, 255);
		GlowingBob((float)x - 5, 75 - 5, 50, 50);
	}
}
