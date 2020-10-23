#include "stdafx.h"
#include "EnemyBasic.h"
#include "Spawner.h"
#include "EnemyCore.h"

EnemyBasic::EnemyBasic()
{
	spawnRadius = 100.0f;
	rotationVel = 1.0f;
	speedMultiplier = 10.0f;
	followMode = false;
}

EnemyBasic::EnemyBasic(Spawner* spawner_, int level_, Vec2& initialPos_, Vec2& initialVelocity_) : EnemyBasic()
{
	spawner = spawner_;
	position = initialPos_;
	SetVelocity(initialVelocity_);

	level = level_;
	sides = 3 + level;
	health = 1 + level;
	attack = 1 + level;
	value = 20 * level;

	if ((maxRadius = static_cast<float>(maxRadius - level)) > 8.0f) {}
	else { maxRadius = 8.0f; }
}

EnemyBasic::EnemyBasic(Spawner* spawner_, float initialPosition_, int level_) : EnemyBasic()
{
	spawner = spawner_;

	float angPosition = initialPosition_;
	position.x = sinf(angPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(angPosition * RAD) * spawnRadius + ScreenCenter.y;
	
	StatsInitializer(level_);

	LinearPath();
}

EnemyBasic::EnemyBasic(Spawner* spawner_, float initialPosition_, int level_, float speed_) : EnemyBasic(spawner_, initialPosition_, level_)
{
	speedMultiplier = speed_;
}

EnemyBasic::EnemyBasic(Spawner* spawner_, int level_, Vec2& initialPos_) : EnemyBasic()
{
	spawner = spawner_;
	position = initialPos_;
	nodePosition = &initialPos_;

	level = level_;
	sides = 3 + level;
	health = 1 + level;
	attack = 1 + level;
	value = 20 * level;

	radius = 20.0f;

	if ((maxRadius = static_cast<float>(maxRadius - level)) > 8.0f) {}
	else { maxRadius = 8.0f; }


	followMode = true;
}

EnemyBasic::~EnemyBasic()
{
	nodePosition = nullptr;
}

void EnemyBasic::StatsInitializer(int level_)
{
	level = level_;
	sides = 3 + level;
	health = 1 + level;
	attack = 1 + level;
	value = 20 * level;

	if ((maxRadius = static_cast<float>(maxRadius - level)) > 8.0f) {}
	else { maxRadius = 8.0f; }
}

void EnemyBasic::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);
	
	UpdateMotion(deltaTime_);

	LimitCheck();
}

void EnemyBasic::Render()
{
	Enemy::Render();
	
}

bool EnemyBasic::OnCollision(int damage_)
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

void EnemyBasic::LinearPath()
{
	//Move the enemy outward away from the center
	Vec2 direction = position - ScreenCenter;
	direction = Math::Normalize(direction);

	SetSpeedMult(20.0f);
	SetVelocity(direction);
}

void EnemyBasic::LimitCheck()
{
	float distSq = Math::MagSq(ScreenCenter - position);

	if (distSq > maxTravelDist) {
		spawner->DestroyEnemyNoScore(this);
	}
}

void EnemyBasic::UpdateMotion(float deltaTime_)
{
	Enemy::UpdateMotion(deltaTime_);

	//Rotation
	rotation += rotationVel;

	if (!followMode) {

		//Scale
		if (radius < maxRadius) {
			radius = fabsf(Math::Mag(position - ScreenCenter) / 10.0f);
		}
	}
	else {
		if (nodePosition != nullptr) {
			position = *nodePosition;
		}
	}
}


