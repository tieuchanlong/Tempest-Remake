#pragma once

#include <vector>
#include "player.h"

class GameMap
{
private:
	static GameMap *instance;
	GameMap() {
		outer_ring_distance = 250;
		inner_ring_distance = 50;
		center_ring_distance = 0;
		delay_next_level = 0;
		gameplay_active = true;
	}
public:
	float outer_ring_distance;
	float inner_ring_distance;
	float center_ring_distance;
	int delay_next_level;
	int space_btwn_lanes = 10;
	bool gameplay_active;
	bool add_counterclockwise = false;

	void TransitNextLevel(std::vector<Player*>& players);
	void Render(int section, float center_x, float center_y, float r, float g, float b);
	static GameMap *GetInstance();
};