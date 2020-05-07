#include "stdafx.h"
#include "Mine.h"
#include "App/app.h"

Mine::Mine()
{
}


Mine::~Mine()
{
}

bool Mine::MoveToPlayer()
{
	travel_x += dir.x;
	travel_y += dir.y;
	App::DrawTriangle(travel_x, travel_y, size, 1.0f, 0.75f, 0.8f);


	return false;
}
