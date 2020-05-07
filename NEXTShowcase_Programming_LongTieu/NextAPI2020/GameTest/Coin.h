#pragma once

#include "enemy.h"

class Coin: public Enemy
{
public:
	Coin();
	~Coin();
	bool MoveToPlayer();
	bool CheckCollisionsCoordinates(float x1, float y1, float x2, float y2);
};