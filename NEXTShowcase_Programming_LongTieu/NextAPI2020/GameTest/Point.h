#pragma once

class Point 
{
public:
	float x, y, angle;
	Point(float x1, float y1, float angle1);
	static bool cmp(Point a, Point b) {
		return a.angle < b.angle;
	}
};