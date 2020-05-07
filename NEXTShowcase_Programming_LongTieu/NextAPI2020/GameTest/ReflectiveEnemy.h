#pragma once

#include "enemy.h"

class ReflectiveEnemy : public Enemy
{
public:
	ReflectiveEnemy();
	~ReflectiveEnemy();
	bool MoveToPlayer();
	bool CheckCollisions(std::vector<Bullet*>& bullets);
	
};