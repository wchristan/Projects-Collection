#pragma once

#include "GameScene.h"
#include "Player.h"
#include "Spawner.h"
#include "CollisionCheck.h"
#include "GameState.h"
#include "Background.h"

//Scene class for the main game
class GameScene01 : public GameScene
{
public:
	GameScene01();
	~GameScene01();
	
	void Initialize();

	void Update(float deltaTime_);
	void Render();

	void CleanUp();

private:
	Player* player;
	Spawner* spawner;
	CollisionCheck* colCheck;
	GameState* state;
	Background* background;
};