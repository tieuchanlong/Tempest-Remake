#include "stdafx.h"
#include "enemy_factory.h"
#include "game_map.h"
#include "vector2.h"
#include "Point.h"
#include "game_settings.h"
#include "App/app.h"

/*
This class will be the factory that produces enemy, coins, mines and other items
*/
EnemyFactory::EnemyFactory(float x, float y)
{
	spawn_delay = 0;
	spawn_x = x;
	spawn_y = y;
}

EnemyFactory::~EnemyFactory()
{
}

/*
Randomly spawn coins and enemies
*/
void EnemyFactory::SpawnEnemy(int id, float dest_x, float dest_y)
{
	GameMap *gameMap = gameMap->GetInstance();
	GameSettings *gameSettings = gameSettings->GetInstance();

	if (!gameMap->gameplay_active || gameSettings->gameOver) {
		Wipe();
		return;
	}

	if (spawn_delay == 0) {
		spawn_delay++;

		for (int i = 0; i < gameSettings->level / 2 + 1; i++) {
			int check = rand() % 4;

			if (check == 0) {
				Enemy *enemy = new Enemy();
				enemy->Id = id;
				enemy->SetPosition(spawn_x, spawn_y);
				enemy->SetDir(dest_x - spawn_x, dest_y - spawn_y);
				enemies.push_back(enemy);
			}
			else if (check == 1 && gameSettings->level > 1 && chase_enemies.size() <= 1)
			{
				ChaseEnemy *enemy = new ChaseEnemy();
				enemy->Id = id;
				enemy->SetPosition(spawn_x, spawn_y);
				enemy->SetDir(dest_x - spawn_x, dest_y - spawn_y);
				chase_enemies.push_back(enemy);
			}
			else if (check == 2 && gameSettings->level > 2) {
				ReflectiveEnemy *enemy = new ReflectiveEnemy();
				enemy->Id = id;
				enemy->SetPosition(spawn_x, spawn_y);
				enemy->SetDir(dest_x - spawn_x, dest_y - spawn_y);
				reflective_enemies .push_back(enemy);
			}
			else if (check == 3) {
				Coin *coin = new Coin();
				coin->Id = id;
				coin->SetPosition(spawn_x, spawn_y);
				coin->SetDir(dest_x - spawn_x, dest_y - spawn_y);
				coins.push_back(coin);
			}
		}
	}

	// Spawn mine when control
}

/*
Update the positions of the enemies and coins
*/
void EnemyFactory::UpdateEnemyPositions(std::vector<Bullet*>& bullets)
{
	if (spawn_delay > 0)
	{
		spawn_delay++;
		if (spawn_delay >= 60)
			spawn_delay = 0;
	}

	//
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->MoveToPlayer())
		{
			delete enemies[i];
			enemies[i] = nullptr;
			enemies.erase(enemies.begin() + i);
			i--;
			continue;
		}

		if (enemies[i]->CheckCollisions(bullets)) {
			delete enemies[i];
			enemies[i] = nullptr;
			enemies.erase(enemies.begin() + i);
			i--;
			continue;
		}
	}

	//
	for (int i = 0; i < chase_enemies.size(); i++) {
		if (chase_enemies[i]->MoveToPlayer())
		{
			delete chase_enemies[i];
			chase_enemies[i] = nullptr;
			chase_enemies.erase(chase_enemies.begin() + i);
			i--;
			continue;
		}

		if (chase_enemies[i]->ChaseOnRing())
		{
			delete chase_enemies[i];
			chase_enemies[i] = nullptr;
			chase_enemies.erase(chase_enemies.begin() + i);
			i--;
			continue;
		}

		if (chase_enemies[i]->CheckCollisions(bullets)) {
			delete chase_enemies[i];
			chase_enemies[i] = nullptr;
			chase_enemies.erase(chase_enemies.begin() + i);
			i--;
			continue;
		}
	}

	//
	for (int i = 0; i < reflective_enemies.size(); i++) {
		if (reflective_enemies[i]->MoveToPlayer())
		{
			delete reflective_enemies[i];
			reflective_enemies[i] = nullptr;
			reflective_enemies.erase(reflective_enemies.begin() + i);
			i--;
			continue;
		}

		if (reflective_enemies[i]->CheckCollisions(bullets)) {
			delete reflective_enemies[i];
			reflective_enemies[i] = nullptr;
			reflective_enemies.erase(reflective_enemies.begin() + i);
			i--;
			continue;
		}
	}

	//
	for (int i = 0; i < coins.size(); i++) {
		if (coins[i]->MoveToPlayer())
		{
			delete coins[i];
			coins[i] = nullptr;
			coins.erase(coins.begin() + i);
			i--;
			continue;
		}
	}

	//
	for (int i = 0; i < mines.size(); i++) {
		if (mines[i]->MoveToPlayer())
		{
			delete mines[i];
			mines[i] = nullptr;
			mines.erase(mines.begin() + i);
			i--;
			continue;
		}
	}
}

/*
Check switch lane especially for the chasing enemy who has this functionality
*/
void EnemyFactory::SwitchLane(int lane, int lane_count, float spawn_x1, float spawn_y1, std::vector<Point> outerline)
{
	for (int i = 0; i < chase_enemies.size(); i++) {
		if (chase_enemies[i]->SwitchLane(lane, lane_count, spawn_x1, spawn_y1,outerline[(chase_enemies[i]->Id-1 + lane_count) % lane_count].x, outerline[(chase_enemies[i]->Id - 1 + lane_count) % lane_count].y, outerline[(chase_enemies[i]->Id + 1 + lane_count) % lane_count].x, outerline[(chase_enemies[i]->Id + 1 + lane_count) % lane_count].y))
		{
			delete chase_enemies[i];
			chase_enemies[i] = nullptr;
			chase_enemies.erase(chase_enemies.begin() + i);
			i--;
			continue;
		}

		if (chase_enemies[i]->deleted)
		{
			delete chase_enemies[i];
			chase_enemies[i] = nullptr;
			chase_enemies.erase(chase_enemies.begin() + i);
			i--;
			continue;
		}
	}
}

/*
Check if the enemy collides with player or the coins collide with the player
If the enemy collide with the player, reduce carrying spaceships or if only 1, game over
If coin collides with player, add points
*/
void EnemyFactory::CheckKillPlayer(std::vector<Player*>& players)
{
	GameSettings *gameSettings = gameSettings->GetInstance();

	for (int i = 0; i < enemies.size(); i++) {
		for (int j = 0; j < players.size(); j++) {
			if (enemies[i]->CheckCollisionsCoordinates(players[j]->GetPosX() - players[j]->GetSize() / 2, players[j]->GetPosY() - players[j]->GetSize() / 2, players[j]->GetPosX() + players[j]->GetSize() / 2, players[j]->GetPosY() + players[j]->GetSize() / 2))
			{
				if (players.size() == 1) {
					gameSettings->gameOver = true;
					gameSettings->HighestScore = max(gameSettings->HighestScore, gameSettings->Score);
					Wipe();
					return;
				}

				if (j == 0)
					players[j]->original = true;

				// Remove the players;
				delete players[j];
				players[j] = nullptr;
				players.erase(players.begin() + j);
				j--;
				continue;
			}
		}

		for (int j = 0; j < mines.size(); j++)
		{
			if (enemies[i]->CheckCollisionsCoordinates(mines[j]->GetPosX() - mines[j]->GetSize(), mines[j]->GetPosY(), mines[j]->GetPosX() + mines[j]->GetSize() - mines[j]->GetSize(), mines[j]->GetPosY() + mines[j]->GetSize()))
			{
				enemies[i]->deleted = true;

				gameSettings->Score += enemies[i]->GetScore();

				delete mines[j];
				mines[j] = nullptr;
				mines.erase(mines.begin() + j);
				j--;
				break;
			}
		}

		if (enemies[i]->deleted)
		{
			delete enemies[i];
			enemies[i] = nullptr;
			enemies.erase(enemies.begin() + i);
			i--;
			continue;
		}
	}

	
	for (int i = 0; i < chase_enemies.size(); i++) {
		for (int j = 0; j < players.size(); j++) {
			Player *player = players[j];
			if (chase_enemies[i]->CheckCollisionsCoordinates(player->GetPosX() - player->GetSize() / 2, player->GetPosY() - player->GetSize() / 2, player->GetPosX() + player->GetSize() / 2, player->GetPosY() + player->GetSize() / 2))
			{
				if (players.size() == 1) {
					gameSettings->gameOver = true;
					gameSettings->HighestScore = max(gameSettings->HighestScore, gameSettings->Score);
					Wipe();
					return;
				}

				if (j == 0)
					players[j]->original = true;

				// Remove the players;
				delete players[j];
				players[j] = nullptr;
				players.erase(players.begin() + j);
				j--;
				continue;
			}
		}

		for (int j = 0; j < mines.size(); j++)
		{
			if (chase_enemies[i]->CheckCollisionsCoordinates(mines[j]->GetPosX() - mines[j]->GetSize(), mines[j]->GetPosY(), mines[j]->GetPosX() + mines[j]->GetSize() - mines[j]->GetSize(), mines[j]->GetPosY() + mines[j]->GetSize()))
			{
				chase_enemies[i]->deleted = true;

				gameSettings->Score += chase_enemies[i]->GetScore();

				delete mines[j];
				mines[j] = nullptr;
				mines.erase(mines.begin() + j);
				j--;
				break;
			}
		}

		if (chase_enemies[i]->deleted)
		{
			delete chase_enemies[i];
			chase_enemies[i] = nullptr;
			chase_enemies.erase(chase_enemies.begin() + i);
			i--;
			continue;
		}	
	}

	for (int i = 0; i < coins.size(); i++) {
		for (int j = 0; j < players.size(); j++) {
			Player *player = players[j];
			if (coins[i]->CheckCollisionsCoordinates(player->GetPosX() - player->GetSize() / 2, player->GetPosY() - player->GetSize() / 2, player->GetPosX() + player->GetSize() / 2, player->GetPosY() + player->GetSize() / 2))
			{
				PlaySoundW(TEXT("Pickup_Coin.wav"), NULL, SND_FILENAME | SND_ASYNC);
				gameSettings->Score++;
				coins[i]->deleted = true;
				break;
			}
		}

		if (coins[i]->deleted)
		{
			delete coins[i];
			coins[i] = nullptr;
			coins.erase(coins.begin() + i);
			i--;
			continue;
		}
	}
}

/*
Spawn mines when the player press Delete button
*/
void EnemyFactory::SpawnMines(int id, float center_x, float center_y)
{
	if (mines.size() >= 2)
		return;
	if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_A))
	{
		// Check if existing mine with same location exists
		for (int i = 0; i < mines.size(); i++)
			if (mines[i]->Id == id)
				return;

		Mine *mine = new Mine();
		mine->Id = id;
		mine->SetPosition(center_x, center_y);
		mine->SetDir(0, 0);
		mines.push_back(mine);
	}
}

/*
Wipe and reset the factory data of enemies and coins
Happens after every level change or every game over restart
*/
void EnemyFactory::Wipe()
{
	for (int i = 0; i < enemies.size(); i++) {
		delete enemies[i];
		enemies[i] = nullptr;
	}

	enemies.clear();

	for (int i = 0; i < chase_enemies.size(); i++) {
		delete chase_enemies[i];
		chase_enemies[i] = nullptr;
	}

	chase_enemies.clear();

	for (int i = 0; i < reflective_enemies.size(); i++) {
		delete reflective_enemies[i];
		reflective_enemies[i] = nullptr;
	}

	reflective_enemies.clear();

	for (int i = 0; i < coins.size(); i++) {
		delete coins[i];
		coins[i] = nullptr;
	}

	coins.clear();

	for (int i = 0; i < mines.size(); i++) {
		delete mines[i];
		mines[i] = nullptr;
	}

	mines.clear();
}
