#include "stdafx.h"
#include "ReflectiveEnemy.h"
#include "game_settings.h"
#include "App/app.h"	

ReflectiveEnemy::ReflectiveEnemy()
{
	HP = 10;
	score_worth = 20;
}

ReflectiveEnemy::~ReflectiveEnemy()
{
}

/*
Similar to Enemy class MoveToPlayer method with different rendering
*/
bool ReflectiveEnemy::MoveToPlayer()
{
	GameSettings *gameSettings = gameSettings->GetInstance();

	travel_x += dir.x/3 * gameSettings->speed;
	travel_y += dir.y/3 * gameSettings->speed;
	App::DrawSquare(travel_x, travel_y, size + 3, 0.0f, 0.0f, 1.0f);
	App::DrawSquare(travel_x, travel_y, size, 1.0f, 0.0f, 0.0f);
	

	if (abs(travel_x - end_x) <= 1.0f && abs(travel_y - end_y) <= 1.0f) {
		deleted = true;
		return true;
	}
	return false;
}

/*
Returns true if collide with the bullet and dies
SOmetimes enemy can reflect bullet to try and kill player
*/
bool ReflectiveEnemy::CheckCollisions(std::vector<Bullet*>& bullets)
{
	GameSettings *gameSettings = gameSettings->GetInstance();
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->Id != Id)
			continue;
		if (CheckCollisionsCoordinates(bullets[i]->GetPosX() - bullets[i]->GetSize() / 2, bullets[i]->GetPosY() - bullets[i]->GetSize() / 2, bullets[i]->GetPosX() + bullets[i]->GetSize() / 2, bullets[i]->GetPosY() + bullets[i]->GetSize() / 2))
		{
			gameSettings->Score += score_worth;
			bullets[i]->ReverseDir();

			HP--;
			if (HP <= 0) {
				deleted = true;
				return true;
			}
		}
	}

	return false;
}