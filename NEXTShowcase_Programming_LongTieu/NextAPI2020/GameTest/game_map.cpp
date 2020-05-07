#include "stdafx.h"
#include "game_map.h"
#include "game_settings.h"
#include "App/app.h"
#include <math.h>

GameMap* GameMap::instance;
void GameMap::TransitNextLevel(std::vector<Player*>& players)
{
	GameSettings *gameSettings = gameSettings->GetInstance();

	if (gameSettings->gameOver) {
		delay_next_level = 0;
		return;
	}

	if (delay_next_level < 1000)
	delay_next_level++;

	if (delay_next_level == 1000) {
		gameplay_active = false;
		inner_ring_distance+=1.0f;
		outer_ring_distance+=1.0f;
		center_ring_distance += 50.0f/(250.0f-50.0f);

		// DUring the transition scene add sound
		if ((int)inner_ring_distance % 30 == 1)
			App::PlaySound("Test2.wav");

		// Make sure when the inner ring is the same as outer ring, start a new level
		if (inner_ring_distance >= 250.0f)
		{
			// Create new data for new level
			inner_ring_distance = 50.0f;
			outer_ring_distance = 250.0f;
			center_ring_distance = 0.0f;
			delay_next_level = 1;
			gameSettings->level++;
			space_btwn_lanes = rand() % 10 + 3;

			// Increase enemy speed
			gameSettings->speed += 0.05f;
			// Make game active again
			gameplay_active = true;

			// Limit the number of players in total, after adding 1 more side of the player after each level, is 5
			if (players.size() >= 5)
				return;
			int id = 0;

			// Add a new side to the player, make the player resist one more turn of damage
			// THis will help us replace the health feature in the original Tempest 1981 game
			if (players.size() == 1)
				id = players[0]->Id;
			else
				id = players[players.size() - 2]->Id;

			int lanes_count = 180 / space_btwn_lanes * 2;

			// Add another player support
			if (add_counterclockwise)
				id = (id + 1) % lanes_count;
			else
				id = (id - 1 + lanes_count) % lanes_count;

			add_counterclockwise = !add_counterclockwise;	//-- Initiate the player
			Player *player;
			player = new Player(0.0f, 0.0f);
			player->Id = id;
			players.push_back(player);
		}
	}
}

/*
Render the line crossing the screen, not the circle line for the map 
*/
void GameMap::Render(int section, float mid_horizontal, float mid_vertical, float r, float g, float b)
{
	float sx = mid_horizontal + cos(section * PI / 180)*GetInstance()->outer_ring_distance;
	float sy = mid_vertical + sin(section * PI / 180)*GetInstance()->outer_ring_distance;
	float ex = mid_horizontal - cos(section * PI / 180)*GetInstance()->outer_ring_distance;
	float ey = mid_vertical - sin(section * PI / 180)*GetInstance()->outer_ring_distance;

	App::DrawLine(sx, sy, ex, ey, r, g, b);
}

/*
Using Singleton Design Pattern, since we only need 1 copy of this class
*/
GameMap * GameMap::GetInstance()
{
	if (instance == nullptr)
		instance = new GameMap;
	return instance;
}
