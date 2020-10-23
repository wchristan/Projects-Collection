#pragma once

#include "GameScene.h"
#include "GameScene01.h"

//Helps with scene transition
class GameManager
{
public:
	GameScene* scene;

	GameManager();
	~GameManager();

	void Initialize();

	void Update(float deltaTime_);
	void Render();

	void LoadScene(GameScene* scene_);

	void CleanUp();

};

