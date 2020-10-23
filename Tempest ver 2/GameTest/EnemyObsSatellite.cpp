#include "stdafx.h"
#include "EnemyObsSatellite.h"

EnemyObsSatellite::EnemyObsSatellite()
{
	health = 0;
	attack = 1;
	spawnRadius = 150.0f;
	rotationVel = 1.0f;
	radius = 2.0f;
	renderColour = defColour = Custom::Red;
	
}

EnemyObsSatellite::EnemyObsSatellite(Spawner* spawner_, float initialPosition_, int level_) : EnemyObsSatellite()
{
	spawner = spawner_;

	radialPosition = initialPosition_;
	position.x = sinf(radialPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(radialPosition * RAD) * spawnRadius + ScreenCenter.y;

	level = level_;
	radius += (float)(level * 8);
	sides = 8;

	if ((maxRadius = static_cast<float>(maxRadius - level)) > 8.0f) {}
	else { maxRadius = 8.0f; }
}

EnemyObsSatellite::~EnemyObsSatellite()
{
}

void EnemyObsSatellite::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);

	UpdateMotion(deltaTime_);
}

void EnemyObsSatellite::Render()
{
	Enemy::ColourProcess();

	for (int i = level; i > 0; i--) {
		Custom::DrawPoly(radius - ((float)(i * 5.0f)), position, rotation, sides, renderColour);
	}
}

bool EnemyObsSatellite::OnCollision(int damage_)
{
	//Do nothing
	return false;
}

void EnemyObsSatellite::UpdateMotion(float deltaTime_)
{
	Enemy::UpdateMotion(deltaTime_);

	//Rotation
	rotation += rotationVel;

	OrbitPath();
}
