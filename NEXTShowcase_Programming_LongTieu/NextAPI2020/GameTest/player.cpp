#include "stdafx.h"
#include "player.h"
#include "App/app.h"
#include "bullet.h"
#include "game_settings.h"
#include "game_map.h"
#include <iostream>

using namespace std;
Player::Player(float x, float y)
{
	delay_shoot_time = 0;
	delay_shoot_time = 0;
	size = 20;
	button_up = true;
	button_dir_up = true;
}

Player::~Player()
{
}

void Player::Shoot(float dest_x, float dest_y)
{
	GameSettings *gameSettings = gameSettings->GetInstance();

	if (gameSettings->gameOver)
		return;

	int c = 0;

	if (delay_shoot_time > 10)
		delay_shoot_time = 0;
	
	if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_ALT_A) && delay_shoot_time == 0) {
		Bullet *bullet = new Bullet();
		bullet->Id = Id;
		bullet->SetPosition(pos_x, pos_y);
		bullet->SetDir(dest_x - pos_x, dest_y - pos_y);
		bullets.push_back(bullet);
		button_dir_up = false;
		delay_shoot_time++;
		if (original)
			PlaySoundW(TEXT("Test.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	else if (App::IsKeyReleased(APP_PAD_EMUL_BUTTON_ALT_A) && !button_dir_up) {
		button_dir_up = true;
		//delay_shoot_time = 0;
	}

	if (delay_shoot_time > 0)
		delay_shoot_time++;
}

void Player::SetPos(float x, float y)
{
	pos_x = x;
	pos_y = y;

	// Draw the player
	App::DrawSquare(pos_x, pos_y, size, 0.0f, 1.0f, 0.0f);
}

void Player::SwitchDirection(int sectors)
{
	GameSettings *gameSettings = gameSettings->GetInstance();

	if (gameSettings->gameOver)
		return;

	if (delay_switch > 5)
		delay_switch = 0;

	if (App::IsKeyPressed(APP_PAD_EMUL_LEFT_THUMB_RIGHT) && button_up && delay_switch == 0) {
		Id = (Id + 1) % sectors;
		button_up = false;
		delay_shoot_time++;
		delay_shoot_time %= 5; 
		delay_switch++;
	}

	if (App::IsKeyPressed(APP_PAD_EMUL_LEFT_THUMB_LEFT) && button_up && delay_switch == 0) {
		Id = (Id - 1);
		if (Id < 0)
			Id = sectors - 1;
		Id = Id % sectors;
		button_up = false;
		delay_shoot_time %= 5;
		delay_switch++;
	}

	if ((App::IsKeyReleased(APP_PAD_EMUL_LEFT_THUMB_RIGHT) || App::IsKeyReleased(APP_PAD_EMUL_LEFT_THUMB_LEFT)) && !button_up)
		button_up = true;

	if (delay_switch > 0)
		delay_switch++;
}

float Player::GetPosX()
{
	return pos_x;
}

float Player::GetPosY()
{
	return pos_y;
}

float Player::GetSize()
{
	return size;
}

bool Player::CheckCollisionsCoordinates(float x1, float y1, float x2, float y2)
{
	if (abs(pos_x - (x2 - x1) / 2) <= 5.0f && abs(pos_y - (y2 - y1) / 2) <= 5.0f)
		return true;

	if (pos_x - size <= x1 && pos_y - size <= y1 && x1 <= pos_x + size && y1 <= pos_y + size)
		return true;

	if (pos_x - size <= x2 && pos_y - size <= y2 && x2 <= pos_x + size && y2 <= pos_y + size)
		return true;

	if (x1 <= pos_x - size && y1 <= pos_y - size && pos_x - size <= x2 && pos_y - size <= y2)
		return true;

	if (x1 <= pos_x + size && y1 <= pos_y + size && pos_x + size <= x2 && pos_y + size <= y2)
		return true;

	return false;
}
