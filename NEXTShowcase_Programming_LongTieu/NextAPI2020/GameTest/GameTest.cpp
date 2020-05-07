//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>
#include <vector>
#include <algorithm>
#include <string>
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------

//---- Include custom classes
#include "enemy.h"
#include "player.h"
#include "enemy_factory.h"
#include "game_settings.h"
#include "game_map.h"
#include "Point.h"
#include <ftpext.h>
//----

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
CSimpleSprite *testSprite;
CSimpleSprite *testSprite2;
EnemyFactory *enemyFactory;
std::vector<Player*> players;

enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	RECT rect;
	HWND hwnd = GetActiveWindow();
	GetWindowRect(hwnd, &rect);

	//
	float mid_horizontal = APP_VIRTUAL_WIDTH / 2;
	float mid_vertical = APP_VIRTUAL_HEIGHT / 2;

	testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	testSprite->SetPosition(400.0f, 400.0f);
	float speed = 1.0f / 15.0f;
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	testSprite->SetScale(2.0f);

	testSprite2 = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 12);
	testSprite2->SetPosition(400.0f, 400.0f);	
	testSprite2->SetFrame(2);
	testSprite2->SetScale(1.0f);
	//------------------------------------------------------------------------

	//-- Initiate the enemy
	enemyFactory = new EnemyFactory(mid_horizontal, mid_vertical);

	//-- Initiate the player
	Player *player = new Player(0.0f, 0.0f);
	player->original = true;
	players.push_back(player);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	
}

void CheckPlayerConditions()
{
	GameMap *gameMap = gameMap->GetInstance();
	GameSettings *gameSettings = gameSettings->GetInstance();
	players[0]->original = true;
	// Draw bullet
	for (int j = 0; j < players.size(); j++) {
		Player *player = players[j];
		for (int i = 0; i < player->bullets.size(); i++)
		{
			player->bullets[i]->MoveBullet();

			if (player->bullets[i]->deleted)
			{
				delete player->bullets[i];
				player->bullets[i] = nullptr;
				player->bullets.erase(player->bullets.begin() + i);
				i--;
				continue;
			}

			if (player->CheckCollisionsCoordinates(player->bullets[i]->GetPosX() - player->bullets[i]->GetSize(),
				player->bullets[i]->GetPosY() - player->bullets[i]->GetSize(),
				player->bullets[i]->GetPosX() + player->bullets[i]->GetSize(),
				player->bullets[i]->GetPosY() + player->bullets[i]->GetSize())
				&& player->bullets[i]->Reverse) {
				gameSettings->gameOver = true;
			}
		}
	}

	// Switch Player direction if press key
	for (int j = 0; j < players.size(); j++)
		players[j]->SwitchDirection(180 * 2 / gameMap->space_btwn_lanes);
}

void PreparePerimeterLineCoords(float mid_horizontal, float mid_vertical, std::vector<Point>& outerline, std::vector<Point>& innerline, std::vector<Point>& playerline, std::vector<Point>& centerline)
{
	GameMap *gameMap = gameMap->GetInstance();
	GameSettings *gameSettings = gameSettings->GetInstance();

	for (int i = 0; i < 180; i++)
	{
		if (i % gameMap->space_btwn_lanes != 0)
			continue;
		float sx = mid_horizontal + cos(i * PI / 180)*gameMap->GetInstance()->outer_ring_distance;
		float sy = mid_vertical + sin(i * PI / 180)*gameMap->GetInstance()->outer_ring_distance;
		float ex = mid_horizontal - cos(i * PI / 180)*gameMap->GetInstance()->outer_ring_distance;
		float ey = mid_vertical - sin(i * PI / 180)*gameMap->GetInstance()->outer_ring_distance;
		if (!gameSettings->gameOver)
			gameMap->Render(i, mid_horizontal, mid_vertical, 1.0f, 1.0f, 1.0f);

		// Save exterior lines points
		outerline.push_back(Point(sx, sy, i));
		outerline.push_back(Point(ex, ey, (i + 180) % 360));

		// Save player points
		sx = mid_horizontal + cos(i * PI / 180) * 250;
		sy = mid_vertical + sin(i * PI / 180) * 250;
		ex = mid_horizontal - cos(i * PI / 180) * 250;
		ey = mid_vertical - sin(i * PI / 180) * 250;
		playerline.push_back(Point(sx, sy, i));
		playerline.push_back(Point(ex, ey, (i + 180) % 360));

		// Save inner lines points
		sx = mid_horizontal + cos(i * PI / 180) * gameMap->GetInstance()->inner_ring_distance;
		sy = mid_vertical + sin(i * PI / 180) * gameMap->GetInstance()->inner_ring_distance;
		ex = mid_horizontal - cos(i * PI / 180) * gameMap->GetInstance()->inner_ring_distance;
		ey = mid_vertical - sin(i * PI / 180) * gameMap->GetInstance()->inner_ring_distance;
		innerline.push_back(Point(sx, sy, i));
		innerline.push_back(Point(ex, ey, (i + 180) % 360));

		// Save center lines points
		sx = mid_horizontal + cos(i * PI / 180) * gameMap->GetInstance()->center_ring_distance;
		sy = mid_vertical + sin(i * PI / 180) * gameMap->GetInstance()->center_ring_distance;
		ex = mid_horizontal - cos(i * PI / 180) * gameMap->GetInstance()->center_ring_distance;
		ey = mid_vertical - sin(i * PI / 180) * gameMap->GetInstance()->center_ring_distance;
		centerline.push_back(Point(sx, sy, i));
		centerline.push_back(Point(ex, ey, (i + 180) % 360));
	}
}

void RenderPerimeterLine(float mid_horizontal, float mid_vertical, std::vector<Point>& outerline, std::vector<Point>& innerline, std::vector<Point>& playerline, std::vector<Point>& centerline)
{
	GameMap *gameMap = gameMap->GetInstance();
	//Sort outer and inner points
	std::sort(outerline.begin(), outerline.end(), Point::cmp);
	std::sort(innerline.begin(), innerline.end(), Point::cmp);
	std::sort(playerline.begin(), playerline.end(), Point::cmp);
	std::sort(centerline.begin(), centerline.end(), Point::cmp);

	// Update the enemy factory
	int sector = rand() % (180 * 2 / gameMap->space_btwn_lanes);

	// Draw exterior line
	for (int i = 0; i < outerline.size(); i++)
	{
		int next = (i + 1) % outerline.size();
		App::DrawLine(outerline[i].x, outerline[i].y, outerline[next].x, outerline[next].y, 1.0f, 1.0f, 1.0f);

		for (int j = 0; j < players.size(); j++) {
			if (players[j]->Id == i) {
				players[j]->SetPos((playerline[i].x + playerline[next].x) / 2, (playerline[i].y + playerline[next].y) / 2);
				App::DrawLine(playerline[i].x, playerline[i].y, innerline[i].x, innerline[i].y, 1.0f, 0.0f, 1.0f);
				App::DrawLine(playerline[next].x, playerline[next].y, innerline[next].x, innerline[next].y, 1.0f, 0.0f, 1.0f);
				enemyFactory->SwitchLane(players[j]->Id, 36, mid_horizontal, mid_vertical, outerline);
				break;
			}
		}

		// Update the enemies
		if (i == sector) {
			enemyFactory->SpawnEnemy(i, (outerline[i].x + outerline[next].x) / 2, (outerline[i].y + outerline[next].y) / 2);

		}
	}

	for (int j = 0; j < players.size(); j++)
		enemyFactory->CheckKillPlayer(players);

	enemyFactory->SpawnMines(players[0]->Id, players[0]->GetPosX(), players[0]->GetPosY());

	for (int j = 0; j < players.size(); j++) {
		enemyFactory->UpdateEnemyPositions(players[j]->bullets);
		players[j]->Shoot(mid_horizontal, mid_vertical);
	}


	// Draw inner line
	for (int i = 0; i < innerline.size(); i++)
	{
		int next = (i + 1) % innerline.size();
		App::DrawLine(innerline[i].x, innerline[i].y, innerline[next].x, innerline[next].y, 1.0f, 1.0f, 1.0f);
	}

	// Draw inner line
	for (int i = 0; i < centerline.size(); i++)
	{
		int next = (i + 1) % centerline.size();
		App::DrawLine(centerline[i].x, centerline[i].y, centerline[next].x, centerline[next].y, 1.0f, 1.0f, 1.0f);
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	//
	GameMap *gameMap = gameMap->GetInstance();
	gameMap->TransitNextLevel(players);
	GameSettings *gameSettings = gameSettings->GetInstance();

	//------------------------------------------------------------------------
	// Declare necessary values for rendering 
	//------------------------------------------------------------------------
	static float a = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	int step = gameMap->space_btwn_lanes;
	int sectors = 90;
	std::vector<Point> outerline;
	std::vector<Point> innerline;
	std::vector<Point> playerline;
	std::vector<Point> centerline;

	// Get the screen size
	RECT rect;
	HWND hwnd = GetActiveWindow();
	GetWindowRect(hwnd, &rect);

	// Get the mid point of the screen in vertical and horizontal
	float mid_horizontal = APP_VIRTUAL_WIDTH / 2;
	float mid_vertical = APP_VIRTUAL_HEIGHT / 2;

	// Check the player conditions from the shot bullets to check if player press swith lane
	CheckPlayerConditions();
	
	// Prepare the coordinates of the points to create perimeter for the map of each level
	PreparePerimeterLineCoords(mid_horizontal, mid_vertical, outerline, innerline, playerline, centerline);
	
	// Render the perimeter lines using the sorted coordinates
	if (!gameSettings->gameOver) 
		RenderPerimeterLine(mid_horizontal, mid_vertical, outerline, innerline, playerline, centerline);

	//------------------------------------------------------------------------
	// Render the score
	//------------------------------------------------------------------------
	gameSettings->RenderScore(mid_horizontal, mid_vertical);

	// Show game over
	if (gameSettings->gameOver)
	{
		gameSettings->RenderGameOver(mid_horizontal, mid_vertical);	
		gameSettings->Retry();
	}

}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete testSprite;
	delete testSprite2;
	delete enemyFactory;
	//------------------------------------------------------------------------
}