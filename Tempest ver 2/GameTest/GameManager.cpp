#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager()
{
	scene = new GameScene01();
}

GameManager::~GameManager()
{
}

void GameManager::Initialize()
{
	scene->Initialize();
}

void GameManager::Update(float deltaTime_)
{
	if (scene == nullptr) {
		return;
	}

	scene->Update(deltaTime_);
}

void GameManager::Render()
{
	if (scene == nullptr) {
		return;
	}

	scene->Render();
}

void GameManager::LoadScene(GameScene* scene_)
{
	scene->CleanUp();
	delete scene;

	scene = scene_;
	Initialize();
}

void GameManager::CleanUp()
{
	scene->CleanUp();
	delete scene;
	scene = nullptr;
}
