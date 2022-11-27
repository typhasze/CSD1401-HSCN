//---------------------------------------------------------
// utils.c
// Code related to calculations, vectors and collisions
//
// authors:		 Hafiz
// contributors: Nicholas Theong
//
//
// Nicholas:
// -Circle collision with player
// -Circle collision with platform 
// 
//---------------------------------------------------------
//
// Copyright(c) 2022 DigiPen Institute of Technology
#include "cprocessing.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	if (click_x >= (area_center_x - area_width / 2) && click_x <= (area_center_x + area_width / 2)
		&& click_y >= (area_center_y - area_height / 2) && click_y <= (area_center_y + area_height / 2))
		return 1;
	return 0;
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	if (click_x >= (circle_center_x - diameter / 2) && click_x <= (circle_center_x + diameter / 2)
		&& click_y >= (circle_center_y - diameter / 2) && click_y <= (circle_center_y + diameter / 2))
		return 1;
	return 0;
}

int isRectangleClicked(float rec_x, float rec_y, float rec_w, float rec_h, float click_x, float click_y) {
	if (click_x >= rec_x && click_x <= rec_x + rec_w &&
		click_y >= rec_y && click_y <= rec_y + rec_h)
		return 1;
	return 0;
}

int circleCollision(float circle_x, float circle_y, float diameter, float rec_x, float rec_y, float rec_w, float rec_h) {
	if (circle_y + (diameter / 2) >= rec_y && circle_y - (diameter / 2) <= rec_y + rec_h
		&& circle_x + (diameter / 2) >= rec_x && circle_x - (diameter / 2) <= rec_x + rec_w)
		return 1;
	return 0;
}

int circleToPlatform(float circle_x, float circle_y, float diameter, float rec_x, float rec_y, float rec_w, float rec_h) {
	if (circle_y + diameter / 2 >= rec_y && circle_y + (diameter / 2) - 10 <= rec_y
		&& circle_x + (diameter / 2) >= rec_x && circle_x - (diameter / 2) <= rec_x + rec_w)
		return 1;
	return 0;
}

CP_Vector AngleToVector(float radian_angle)
{
	// cos A = x
	// sin A = y
	float x = (float)cos(radian_angle);
	float y = (float)sin(radian_angle);
	CP_Vector ret = CP_Vector_Set(x, y);
	ret = CP_Vector_Normalize(ret);
	return ret;
}

int random_int(int min, int max)
{
	return min + rand() % (max + 1 - min);
}

