#include "stdafx.h"
#include "EnemyBasicSatellite.h"
#include "Spawner.h"


EnemyBasicSatellite::EnemyBasicSatellite()
{
	spawnRadius = 150.0f;
	rotationVel = 1.0f;
	radius = 40.0f;
}

EnemyBasicSatellite::EnemyBasicSatellite(Spawner* spawner_, float initialPosition_, int level_) : EnemyBasicSatellite()
{
	spawner = spawner_;

	radialPosition = initialPosition_;
	position.x = sinf(radialPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(radialPosition * RAD) * spawnRadius + ScreenCenter.y;

	level = level_;
	sides = 3 + level;
	if (sides > 16) sides = 16;
	health = 1 + level;
	attack = 1 + level;
	value = 20 * level;

	if (level > 16) radius = 10.0f;
	else if (level > 8) radius = 20.0f;
	

	if ((maxRadius = static_cast<float>(maxRadius - level)) > 8.0f) {}
	else { maxRadius = 8.0f; }
}

EnemyBasicSatellite::~EnemyBasicSatellite()
{

}

void EnemyBasicSatellite::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);

	UpdateMotion(deltaTime_);
}

void EnemyBasicSatellite::Render()
{
	Enemy::Render();
}

bool EnemyBasicSatellite::OnCollision(int damage_)
{
	Enemy::OnCollision(damage_);
	 
	if (health > 18) {
		sides = 20;
		attack = 20;
	}
	else {
		sides = 2 + health;
		attack = health;
	}

	if (health <= 0) {
		spawner->DestroyEnemy(this);
		return true;
	}
	return false;
}

void EnemyBasicSatellite::UpdateMotion(float deltaTime_)
{
	Enemy::UpdateMotion(deltaTime_);

	//Rotation
	rotation += rotationVel;

	OrbitPath();
}
