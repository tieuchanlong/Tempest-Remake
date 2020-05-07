#pragma once
#include "bullet.h"
#include <vector>

class Player
{
private:
	float pos_x;
	float pos_y;
	float size;
	int delay_shoot_time;
	int delay_switch;
	bool button_up;
	bool button_dir_up;
public:
	int Id;
	bool original = false; // Check if the current player is the orginal player or the added player for every level, this will help prevent stuck shooting sound
	std::vector<Bullet*> bullets;
	Player(float x, float y);
	~Player();
	void Shoot(float dest_x, float dest_y);
	void SetPos(float x, float y);
	void SwitchDirection(int sectors);
	float GetPosX();
	float GetPosY();
	float GetSize();
	bool CheckCollisionsCoordinates(float x1, float y1, float x2, float y2);
};