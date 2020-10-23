#include "stdafx.h"
#include "GameScene01.h"


GameScene01::GameScene01()
{
	background = nullptr;
	state = nullptr;
	player = nullptr;
	spawner = nullptr;
	colCheck = nullptr;
}

GameScene01::~GameScene01()
{
}

void GameScene01::Initialize()
{
	background = new Background();
	state = new GameState();
	player = new Player(state);
	spawner = new Spawner(state, player);
	colCheck = new CollisionCheck(player, spawner);

}

void GameScene01::Update(float deltaTime_)
{
	if (player->GetHealth() <= 0) {
		return;
	}
	
	background->Update(deltaTime_);
	player->Update(deltaTime_);
	spawner->Update(deltaTime_);
	colCheck->Update(deltaTime_);
	state->Update(deltaTime_);
}

void GameScene01::Render()
{
	background->Render();
	
	if (player->GetHealth() <= 0) {
		state->ShowFinalScore();
	}

	player->Render();
	spawner->Render();
	state->Render();

	Custom::Render();
}

void GameScene01::CleanUp()
{
	delete player;
	player = nullptr;

	delete spawner;
	spawner = nullptr;

	delete colCheck;
	colCheck = nullptr;

	delete state;
	state = nullptr;

	delete background;
	background = nullptr;
}

