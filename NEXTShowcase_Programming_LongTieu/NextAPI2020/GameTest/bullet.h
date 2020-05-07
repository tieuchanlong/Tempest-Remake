#pragma once

#if !defined(BULLET_H)
#define BULLET_H
#include <math.h>
#include "vector2.h"


class Bullet
{
private:
	float spawn_x;
	float spawn_y;
	float pos_x;
	float pos_y;
	float end_x;
	float end_y;
	float travel_speed;
	float travel_angle;
	float size;
	Vector2 dir;
public:
	int Id;
	bool Reverse = false;
	bool deleted = false;
	Bullet();
	~Bullet();
	void SetPosition(float x, float y);
	void SetDir(float x, float y);
	void ReverseDir();
	void MoveBullet();
	float GetSize();
	float GetPosX();
	float GetPosY();
};
#endif