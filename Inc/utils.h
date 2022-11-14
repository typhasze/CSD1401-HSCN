#pragma once


int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
int isRectangleClicked(float rec_x, float rec_y, float rec_w, float rec_h, float click_x, float click_y);
int circleCollision(float circle_x, float circle_y, float diameter, float rec_x, float rec_y, float rec_w, float rec_h);
int circleToPlatform(float circle_x, float circle_y, float diameter, float rec_x, float rec_y, float rec_w, float rec_h);
union CP_Vector AngleToVector(float radian_angle);
int random_int(int min, int max);
