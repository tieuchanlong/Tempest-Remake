#include "stdafx.h"
#include "enemy.h"
#include "App/app.h"
#include "game_settings.h"

Enemy::Enemy()
{
	size = 10;
	score_worth = 2;
	travel_time = 0;
}

void Enemy::SetPosition(float location_x, float location_y)
{
	spawn_x = location_x;
	spawn_y = location_y;
	//travel_angle = angle;
	//travel_speed = speed;
	travel_x = location_x;
	travel_y = location_y;
}

void Enemy::SetDir(float x, float y)
{
	dir.x = x;
	dir.y = y;

	end_x = spawn_x + x;
	end_y = spawn_y + y;

	dir.normalize();
}

Enemy::~Enemy()
{
}

bool Enemy::MoveToPlayer()
{
	GameSettings *gameSettings = gameSettings->GetInstance();

	travel_x += dir.x * gameSettings->speed;
	travel_y += dir.y * gameSettings->speed;
	App::DrawSquare(travel_x, travel_y, size, 1.0f, 0.0f, 0.0f);

	if (abs(travel_x - end_x) <= 1.0f && abs(travel_y - end_y) <= 1.0f) {
		deleted = true;
		return true;
	}
	return false;
}

bool Enemy::CheckCollisions(std::vector<Bullet*>& bullets)
{
	GameSettings *gameSettings = gameSettings->GetInstance();
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->Id != Id)
			continue;
		if (CheckCollisionsCoordinates(bullets[i]->GetPosX() - bullets[i]->GetSize() / 2, bullets[i]->GetPosY() - bullets[i]->GetSize() / 2, bullets[i]->GetPosX() + bullets[i]->GetSize() / 2, bullets[i]->GetPosY() + bullets[i]->GetSize() / 2))
		{
			gameSettings->Score += score_worth;
			delete bullets[i];
			bullets[i] = nullptr;
			bullets.erase(bullets.begin() + i);
			deleted = true;
			PlaySoundW(TEXT("Hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
			i--;
			return true;
		}

		if (bullets[i]->deleted) {
			delete bullets[i];
			bullets[i] = nullptr;
			bullets.erase(bullets.begin() + i);
			i--;
			continue;
		}
	}

	return false;
}

bool Enemy::CheckCollisionsCoordinates(float x1, float y1, float x2, float y2)
{
	if (abs(travel_x - (x2 - x1) / 2) <= 5.0f && abs(travel_y - (y2 - y1) / 2) <= 5.0f)
		return true;

	if (travel_x - size <= x1 && travel_y - size <= y1 && x1 <= travel_x + size && y1 <= travel_y + size)
		return true;

	if (travel_x - size <= x2 && travel_y - size <= y2 && x2 <= travel_x + size && y2 <= travel_y + size)
		return true;

	if (x1 <= travel_x - size && y1 <= travel_y - size && travel_x - size <= x2 && travel_y - size <= y2)
		return true;

	if (x1 <= travel_x + size && y1 <= travel_y + size && travel_x + size <= x2 && travel_y + size <= y2)
		return true;

	return false;
}

float Enemy::GetPosX()
{
	return travel_x;
}

float Enemy::GetPosY()
{
	return travel_y;
}

float Enemy::GetSize()
{
	return size;
}

int Enemy::GetScore()
{
	return score_worth;
}

