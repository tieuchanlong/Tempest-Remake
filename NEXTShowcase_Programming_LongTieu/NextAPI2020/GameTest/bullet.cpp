#include "stdafx.h"
#include "bullet.h"
#include "App/app.h"

Bullet::Bullet()
{
	travel_speed = 2;
	size = 5;
}

Bullet::~Bullet()
{
}

void Bullet::SetPosition(float x, float y)
{
	pos_x = x;
	pos_y = y;
	spawn_x = x;
	spawn_y = y;
}

/*
Set travel vector direction
*/
void Bullet::SetDir(float x, float y)
{
	dir.x = x;
	dir.y = y;
	end_x = x + pos_x;
	end_y = y + pos_y;
	dir.normalize();
}

void Bullet::ReverseDir()
{
	dir.x = -dir.x;
	dir.y = -dir.y;
	std::swap(spawn_x, end_x);
	std::swap(spawn_y, end_y);
	Reverse = true;
}

/*
Move the bullet to the center of the map
Sometimes bullets can be reversed because of reflective enemies
*/
void Bullet::MoveBullet()
{
	pos_x += dir.x * travel_speed;
	pos_y += dir.y * travel_speed;
	
	if (!Reverse)
		App::DrawSquare(pos_x, pos_y, size, 0.0f, 0.0f, 1.0f);
	else
		App::DrawSquare(pos_x, pos_y, size, 1.0f, 0.0f, 0.0f);

	if (abs(pos_x - end_x) <= 5.0f && abs(pos_y - end_y) <= 5.0f) {
		deleted = true;
	}
}

float Bullet::GetSize()
{
	return size;
}

float Bullet::GetPosX()
{
	return pos_x;
}

float Bullet::GetPosY()
{
	return pos_y;
}
