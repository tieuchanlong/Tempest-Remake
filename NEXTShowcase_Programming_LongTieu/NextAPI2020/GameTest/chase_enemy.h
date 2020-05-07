#pragma once
#include "enemy.h"

class ChaseEnemy : public Enemy
{
private:
	int delay_switch_lane;
	bool rotate_counterclockwise;
	void SetCenter(float x, float y);
public:
	ChaseEnemy();
	~ChaseEnemy();

	bool MoveToPlayer();
	bool ChaseOnRing();
	bool SwitchLane(int lane, int lane_count, float spawn_x1, float spawn_y1, float dest_x1, float dest_y1, float dest_x2, float dest_y2);
};