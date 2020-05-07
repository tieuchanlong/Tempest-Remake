#pragma once

#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

class GameSettings
{
private:
	static GameSettings *instance;
	GameSettings() {
		Score = 0;
		HighestScore = 0; 
		level = 1;
		speed = 1.0f;
		gameOver = false;
	}
public:
	int level;
	int Score;
	int HighestScore;
	float speed;
	bool gameOver;
	static GameSettings *GetInstance();
	void Retry();
	void RenderScore(float mid_horizontal, float mid_vertical);
	void RenderGameOver(float mid_horizontal, float mid_vertical);
};

#endif // !GAME_SETTINGS_H