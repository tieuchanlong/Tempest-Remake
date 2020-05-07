#pragma once

#include "enemy.h"

class Mine : public Enemy
{
public:
	Mine();
	~Mine();
	bool MoveToPlayer();
};