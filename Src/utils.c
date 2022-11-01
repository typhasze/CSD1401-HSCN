#include "cprocessing.h"
#include <math.h>
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	if (click_x >= (area_center_x - area_width / 2) && click_x <= (area_center_x + area_width/2)
		&& click_y >= (area_center_y - area_height / 2) && click_y <= (area_center_y + area_height/2))
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

CP_Vector AngleToVector(float radian_angle)
{
	// cos A = x
	// sin A = y
	float x = cos(radian_angle);
	float y = sin(radian_angle);
	CP_Vector ret = CP_Vector_Set(x, y);
	ret = CP_Vector_Normalize(ret);
    return ret;
}