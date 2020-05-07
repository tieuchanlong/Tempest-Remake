#pragma once

#ifndef ENEMY_FACTORY_H
#define ENEMY_FACTORY_H

#include "enemy.h"
#include "chase_enemy.h"
#include <vector>
#include "Point.h"
#include "player.h"
#include "Coin.h"
#include "Mine.h"
#include "ReflectiveEnemy.h"

class EnemyFactory
{
private:
	float spawn_x;
	float spawn_y;
	float spawn_delay;
	std::vector<Enemy*> enemies;
	std::vector<ChaseEnemy*> chase_enemies;
	std::vector<ReflectiveEnemy*> reflective_enemies;
	std::vector<Coin*> coins;
	std::vector<Mine*> mines;

	void Wipe();
public:
	EnemyFactory(float x, float y);
	~EnemyFactory();
	void SpawnEnemy(int id, float dest_x, float dest_y);
	void UpdateEnemyPositions(std::vector<Bullet*>& bullets);
	void SwitchLane(int lane, int lane_count, float spawn_x1, float spawn_y1, std::vector<Point> outerline);
	void CheckKillPlayer(std::vector<Player*>& players);
	void SpawnMines(int id, float center_x, float center_y);
};

#endif // !ENEMY_FACTORY_H