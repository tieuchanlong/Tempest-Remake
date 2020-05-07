#include "stdafx.h"
#include "game_settings.h"
#include "App/app.h"
#include <string>

GameSettings* GameSettings::instance;

GameSettings * GameSettings::GetInstance()
{
	if (instance == nullptr)
		instance = new GameSettings;
	return instance;
}

void GameSettings::Retry()
{
	if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_BACK)) {
		gameOver = false;
		level = 0;
		Score = 0;
	}
}

void GameSettings::RenderScore(float mid_horizontal, float mid_vertical)
{
	std::string score = std::to_string(Score);
	std::string levelText = "Level " + std::to_string(level);
	App::Print(94, mid_vertical*1.5 + 40, levelText.c_str());
	App::Print(95, mid_vertical*1.5 + 20, "Score");
	App::Print(100, mid_vertical*1.5, score.c_str());
	App::Print(95, mid_vertical*1.5 - 20, "Highest Score");
	App::Print(100, mid_vertical*1.5 - 45, std::to_string(HighestScore).c_str());

	// Render instructions
	App::Print(40, mid_vertical*1.5 - 400, "Press W, A, S, D to move around");
	App::Print(40, mid_vertical*1.5 - 420, "Press Space to shoot");
	App::Print(40, mid_vertical*1.5 - 440, "Press Delete to move around");
}

void GameSettings::RenderGameOver(float mid_horizontal, float mid_vertical)
{
	App::Print(mid_horizontal - 60, mid_vertical + 50, "Game over", 1.0f, 1.0f, 1.0f);
	App::Print(mid_horizontal - 100, mid_vertical + 20, "Press BackSpace to retry", 1.0f, 1.0f, 1.0f);
}
