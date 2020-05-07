#include "stdafx.h"
#include "Coin.h"
#include "App/app.h"

Coin::Coin()
{
	score_worth = 1;
}

/*
Check if move to the outer ring
Return true and destroy the coin if reached the outer ring
*/
bool Coin::MoveToPlayer()
{
	travel_x += dir.x;
	travel_y += dir.y;
	App::DrawTriangle(travel_x, travel_y, size, 1.0f, 1.0f, 0.0f);

	if (abs(travel_x - end_x) <= 1.0f && abs(travel_y - end_y) <= 1.0f) {
		deleted = true;
		return true;
	}
	return false;
}

/// Check collision on 3 tips of the triangles
bool Coin::CheckCollisionsCoordinates(float x1, float y1, float x2, float y2)
{
	float endpoint_x = travel_x + size * cos(30 * PI / 180);
	float endpoint_y = travel_y - size * sin(30 * PI / 180);
	if (abs(endpoint_x - (x2 - x1) / 2) <= 5.0f && abs(endpoint_y - (y2 - y1) / 2) <= 5.0f)
		return true;

	if (endpoint_x - size <= x1 && endpoint_y - size <= y1 && x1 <= endpoint_x + size && y1 <= endpoint_y + size)
		return true;

	if (endpoint_x - size <= x2 && endpoint_y - size <= y2 && x2 <= endpoint_x + size && y2 <= endpoint_y + size)
		return true;

	if (x1 <= endpoint_x - size && y1 <= endpoint_y - size && endpoint_x - size <= x2 && endpoint_y - size <= y2)
		return true;

	if (x1 <= endpoint_x + size && y1 <= endpoint_y + size && endpoint_x + size <= x2 && endpoint_y + size <= y2)
		return true;


	endpoint_x = travel_x;
	endpoint_y = travel_y + size * sin(90 * PI / 180);
	if (abs(endpoint_x - (x2 - x1) / 2) <= 5.0f && abs(endpoint_y - (y2 - y1) / 2) <= 5.0f)
		return true;

	if (endpoint_x - size <= x1 && endpoint_y - size <= y1 && x1 <= endpoint_x + size && y1 <= endpoint_y + size)
		return true;

	if (endpoint_x - size <= x2 && endpoint_y - size <= y2 && x2 <= endpoint_x + size && y2 <= endpoint_y + size)
		return true;

	if (x1 <= endpoint_x - size && y1 <= endpoint_y - size && endpoint_x - size <= x2 && endpoint_y - size <= y2)
		return true;

	if (x1 <= endpoint_x + size && y1 <= endpoint_y + size && endpoint_x + size <= x2 && endpoint_y + size <= y2)
		return true;

	endpoint_x = travel_x - size * cos(30 * PI / 180);
	endpoint_y = travel_y - size * sin(30 * PI / 180);
	if (abs(endpoint_x - (x2 - x1) / 2) <= 5.0f && abs(endpoint_y - (y2 - y1) / 2) <= 5.0f)
		return true;

	if (endpoint_x - size <= x1 && endpoint_y - size <= y1 && x1 <= endpoint_x + size && y1 <= endpoint_y + size)
		return true;

	if (endpoint_x - size <= x2 && endpoint_y - size <= y2 && x2 <= endpoint_x + size && y2 <= endpoint_y + size)
		return true;

	if (x1 <= endpoint_x - size && y1 <= endpoint_y - size && endpoint_x - size <= x2 && endpoint_y - size <= y2)
		return true;

	if (x1 <= endpoint_x + size && y1 <= endpoint_y + size && endpoint_x + size <= x2 && endpoint_y + size <= y2)
		return true;


	return false;
}

Coin::~Coin()
{
}
