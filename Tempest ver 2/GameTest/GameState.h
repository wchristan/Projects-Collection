#pragma once

#include "Spawner.h"

//Class to keep track of scoring system and stage level
//Also helps with rendering UI onto the screen
class GameState
{
public:
	GameState();
	~GameState();

	void Initialize(int initHealth_);

	void Update(float deltaTime_);
	void Render();

	//UI functions
	void ShowScore();
	void ShowHealth();
	void ShowLevel();
	void ShowInfo();
	void ShowFinalScore();
	void ShowBuffs();

	void UpdateScore(int score_);
	void UpdateHealth(int value_);
	void UpdateEnemiesCount(int value_);
	void UpdateLevel(int value_);
	void UpdateBuffs(EBuffType buffType_, int value_);

	int GetLevel();

private:
	unsigned int scoreHealth;
	unsigned int scorePoint;
	unsigned int enemiesCount;
	unsigned int stageLevel;
	unsigned short int attack;
	unsigned short int rebound;
	unsigned short int fireRate;
	unsigned short int bulletSpeed;
	unsigned short int bulletLife;
	unsigned short int turretNum;

	int screenXMax;
	int screenYMax;

};