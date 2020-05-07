#pragma once

#if !defined(ENEMY_H)
#define ENEMY_H
#include "stdafx.h"
#include <iostream>
#include "bullet.h"
#include <vector>
#include "vector2.h"
#include <functional>

class Enemy
{
protected:
	// Spawn locations
	float spawn_x;
	float spawn_y;
	//Destinations to travel to
	float end_x;
	float end_y;
	//Current positions
	float travel_x;
	float travel_y;
	float travel_angle;
	float travel_speed;
	float size;
	// TIme travelling
	int travel_time;
	// Score worth
	int score_worth;
	// HP mostly applies for the reflective enemy
	int HP = 1;
	// Travel vector direction
	Vector2 dir;
public:
	// Unique Id that shows the lane the enemy is on
	int Id;
	// Check if the object should be deleted after collides with bullets or go out of map perimeter
	bool deleted = false;
	bool Init = false;
	Enemy();
	void SetPosition(float location_x, float location_y);
	void SetDir(float x, float y);
	~Enemy();
	virtual bool MoveToPlayer();
	virtual bool CheckCollisions(std::vector<Bullet*>& bullets);
	bool CheckCollisionsCoordinates(float x1, float y1, float x2, float y2);
	float GetPosX();
	float GetPosY();
	float GetSize();
	int GetScore();
};
#endif