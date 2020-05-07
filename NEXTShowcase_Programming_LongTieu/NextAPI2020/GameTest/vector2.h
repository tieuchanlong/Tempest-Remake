#pragma once
struct Vector2
{
	float x, y;
	void normalize()
	{
		float r = sqrt(x*x + y * y) / 2;
		if (r == 0)
			return;
		x /= r;
		y /= r;
	}
};