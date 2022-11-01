#pragma once


int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
int isRectangleClicked(float rec_x, float rec_y, float rec_w, float rec_h, float click_x, float click_y);
union CP_Vector AngleToVector(float radian_angle);