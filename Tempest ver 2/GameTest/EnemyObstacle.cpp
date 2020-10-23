#include "stdafx.h"
#include "EnemyObstacle.h"
#include "Spawner.h"

EnemyObstacle::EnemyObstacle()
{
	health = 0;
	spawnRadius = 100.0f;
	rotationVel = 1.0f;
	radius = 2.0f;
	speedMultiplier = 10.0f; 
	defColour = Custom::Red;
}

EnemyObstacle::EnemyObstacle(Spawner* spawner_, int level_, Vec2& initialPos_, Vec2& initialVelocity_) : EnemyObstacle()
{
	spawner = spawner_;
	position = initialPos_;
	SetVelocity(initialVelocity_);

	level = level_;
	radius += level * 5;

	sides = 6 + (level * 2);
	attack = level;
}

EnemyObstacle::EnemyObstacle(Spawner* spawner_, float initialPosition_, int level_) : EnemyObstacle()
{
	spawner = spawner_;

	float angPosition = initialPosition_;
	position.x = sinf(angPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(angPosition * RAD) * spawnRadius + ScreenCenter.y;

	level = level_;
	radius += level * 5;

	sides = 6 + (level * 2);
	attack = level;

	LinearPath();
}

EnemyObstacle::EnemyObstacle(Spawner* spawner_, float initialPosition_, int level_, float speed_) : EnemyObstacle()
{
	spawner = spawner_;

	float angPosition = initialPosition_;
	position.x = sinf(angPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(angPosition * RAD) * spawnRadius + ScreenCenter.y;

	level = level_;
	radius += level * 5;
	sides = 6 + (level * 2);
	attack = level;

	speedMultiplier = speed_;
	
	LinearPath();
}

EnemyObstacle::~EnemyObstacle()
{
}

void EnemyObstacle::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);

	UpdateMotion(deltaTime_);

	LimitCheck();
}

void EnemyObstacle::Render()
{
	Enemy::Render();

}

bool EnemyObstacle::OnCollision(int damage_)
{
	//Do Nothing
	return false;
}

void EnemyObstacle::UpdateMotion(float deltaTime_)
{
	Enemy::UpdateMotion(deltaTime_);
}

void EnemyObstacle::ColourProcess()
{
	Enemy::ColourProcess();
}

void EnemyObstacle::LimitCheck()
{
	float distSq = Math::MagSq(ScreenCenter - position);

	if (distSq > maxTravelDist) {
		spawner->DestroyObstacle(this);
	}
}
