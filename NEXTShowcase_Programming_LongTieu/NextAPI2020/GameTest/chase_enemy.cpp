#include "stdafx.h"
#include "chase_enemy.h"
#include "App/app.h"
#include "game_settings.h"

ChaseEnemy::ChaseEnemy()
{
	ChaseEnemy::Enemy();
	delay_switch_lane = 0;
	score_worth = 8;
	dir.x = -1;
	dir.y = -1;
	size = 10;

	int r = rand() % 2;
	rotate_counterclockwise = (r == 0);
}

ChaseEnemy::~ChaseEnemy()
{
	
}

/*
Move to the outer perimeter ring
Returns true if not escape out of perimeter ring

*/
bool ChaseEnemy::MoveToPlayer()
{
	if (dir.x == 0 && dir.y == 0)
		return false;

	GameSettings *gameSettings = gameSettings->GetInstance();
	travel_x += dir.x * gameSettings->speed;
	travel_y += dir.y * gameSettings->speed;
	travel_time++;
	App::DrawSquare(travel_x, travel_y, size, 0.3f, 0.4f, 0.6f);

	if (abs(travel_x - end_x) <= 5.0f && abs(travel_y - end_y) <= 5.0f) {
		//
		//return true;
		dir.x = 0;
		dir.y = 0;
	}
	return false;
}

/*
Chase the player in the perimeter circle after travel through the center
*/
bool ChaseEnemy::ChaseOnRing()
{
	if (dir.x != 0 || dir.y != 0)
		return false;

	float angle = atan2f(travel_y - spawn_y, travel_x - spawn_x);
	float dist = sqrt((travel_y - spawn_y) * (travel_y - spawn_y) + (travel_x - spawn_x) * (travel_x - spawn_x));

	angle = angle * 180 / PI;

	if (rotate_counterclockwise)
		angle += 0.5f;
	else
		angle = angle - 0.5f + 360;

	if (angle > 360)
		angle -= 360;

	travel_x = spawn_x + dist * cosf(angle * PI/180);
	travel_y = spawn_y + dist * sinf(angle * PI/180);

	App::DrawSquare(travel_x, travel_y, size, 0.3f, 0.4f, 0.6f);

	return false;
}

/*
Set the initial spawn location for the enemy
*/
void ChaseEnemy::SetCenter(float x, float y)
{
	spawn_x = x;
	spawn_y = y;
}

/*
Switch lane to pursue player and chooses the shortest path
*/
bool ChaseEnemy::SwitchLane(int lane, int lane_count, float spawn_x1, float spawn_y1, float dest_x1, float dest_y1, float dest_x2, float dest_y2)
{
	if (dir.x == 0 && dir.y == 0)
		return false;
	if (delay_switch_lane > 100)
		delay_switch_lane = 0;
	if (delay_switch_lane == 0) {
		if (Id == lane)
			return false;

		int dist = abs(Id - lane);
		bool switch_counterclockwise = true;

		if (dist <= lane_count - dist)
			Id--;
		else {
			Id++;
			switch_counterclockwise = false;
		}

		if (Id < 0)
			Id = lane_count - 1;
		
		Id %= lane_count;
		
		if (switch_counterclockwise) {
			SetPosition(spawn_x1, spawn_y1);
			if (dir.x != 0 || dir.y != 0)
				SetDir(dest_x1 - spawn_x1, dest_y1 - spawn_y1);
			SetPosition(spawn_x1 + dir.x * travel_time, spawn_y1 + dir.y * travel_time);
		}
		else {
			SetPosition(spawn_x1, spawn_y1);
			if (dir.x != 0 || dir.y != 0)
				SetDir(dest_x2 - spawn_x1, dest_y2 - spawn_y1);
			SetPosition(spawn_x1 + dir.x * travel_time, spawn_y1 + dir.y * travel_time);
		}
		SetCenter(spawn_x1, spawn_y1);
	}
	delay_switch_lane++;

	return false;
}
