#include "stdafx.h"
#include "GameState.h"
#include "App/app.h"

GameState::GameState()
{
	screenXMax = APP_VIRTUAL_WIDTH;
	screenYMax = APP_VIRTUAL_HEIGHT;

	stageLevel = 1;
	scoreHealth = 100;
	scorePoint = 0;
	enemiesCount = 0;

	attack = 0;
	rebound = 0;
	fireRate = 0;
	bulletSpeed = 0;
	bulletLife = 0;
	turretNum = 0;
}

GameState::~GameState()
{
}

void GameState::Initialize(int initHealth_)
{
	scoreHealth = initHealth_;
}

void GameState::Update(float deltaTime_)
{
}

void GameState::Render()
{
	ShowScore();
	ShowHealth();
	ShowLevel();
	ShowInfo();
	ShowBuffs();
}

void GameState::ShowScore()
{
	App::PrintLabel(screenXMax - 180, screenYMax - 50, 120, "Score:: ", scorePoint);
}

void GameState::ShowHealth()
{
	App::PrintLabel(50, screenYMax - 50, 100, "Health:: ", scoreHealth);
}

void GameState::ShowLevel()
{
	App::PrintLabel(screenXMax / 2 - 50, screenYMax - 50, 100, "Level:: ", stageLevel);
}

void GameState::ShowInfo()
{
	App::PrintLabel(50, 170, 300, "Press [A] or [D] to Move", 1);
	App::PrintLabel(50, 140, 300, "Hold [Shift] to Move Faster", 2);
	App::PrintLabel(50, 110, 300, "Press LMB to Shoot", 3);
	App::PrintLabel(50, 80, 300, "Press [ESC] to Exit Game", 4);
}

void GameState::ShowFinalScore()
{
	App::PrintLabel(screenXMax / 2 - 100, screenYMax /2, 200, "FINAL SCORE:: ", scorePoint);
}

void GameState::ShowBuffs()
{
	App::PrintLabel(1000, 120, 200, "Attack", attack);
	App::PrintLabel(1000, 100, 200, "Rebound", rebound);
	App::PrintLabel(1000, 80, 200, "Fire Rate", fireRate);
	App::PrintLabel(1000, 60, 200, "Bullet Speed", bulletSpeed);
	App::PrintLabel(1000, 40, 200, "Bullet Life", bulletLife);
	App::PrintLabel(1000, 20, 200, "Turret", turretNum);
}

void GameState::UpdateScore(int score_)
{
	scorePoint += score_;
}

void GameState::UpdateHealth(int value_)
{
	scoreHealth = value_;

	if (scoreHealth < 0) {
		scoreHealth = 0;
	}
}

void GameState::UpdateEnemiesCount(int value_)
{
	enemiesCount += value_;
}

void GameState::UpdateLevel(int value_)
{
	stageLevel = value_;
}

void GameState::UpdateBuffs(EBuffType buffType_, int value_)
{
	switch (buffType_) {
	case EBuffType::Attack:
		attack = value_;
		break;
	case EBuffType::Rebound:
		rebound = value_;
		break;
	case EBuffType::FireRate:
		fireRate = value_;
		break;
	case EBuffType::BulletSpeed:
		bulletSpeed = value_;
		break;
	case EBuffType::BulletLife:
		bulletLife++;
		break;
	case EBuffType::Turret:
		turretNum = value_;
		break;
	}
}

int GameState::GetLevel()
{
	return stageLevel;
}
