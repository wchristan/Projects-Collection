#include "stdafx.h"
#include "EnemyShooter.h"
#include "Spawner.h"
#include "Player.h"

EnemyShooter::EnemyShooter() : Enemy()
{
	spawnRadius = 100.0f;
	rotationVel = 1.0f;
	radius = 50.0f;
	speedMultiplier = 30.0f;
	searchRadius = 400.0f;
	searchRadius *= searchRadius; //I'm using MagSq
	bulletSpeed = 10.0f; //max 25.0f
	fireRate = 4000.0f; //max 1000.0f
	
	revOrbitSpeed = 0.5f;
	revOutwardSpeed = 0.4f;
	foundPlayer = false;

	defColour = Custom::Orange;

	timer.SetTimer(fireRate, 1, true);
	player = nullptr;

	ammoType = EEnemyType::Basic;
}

EnemyShooter::EnemyShooter(Spawner* spawner_, float initialPosition_, int level_, Player* player_) : EnemyShooter()
{
	spawner = spawner_;
	player = player_;
	
	radialPosition = initialPosition_;
	
	float angPosition = initialPosition_;
	position.x = sinf(angPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(angPosition * RAD) * spawnRadius + ScreenCenter.y;

	level = level_;
	sides = 3 + level;
	health = 2 + (level * 2);
	attack = 1 + level;
	value = 60 * level;

	if (level > 10) {
		ammoType = EEnemyType::Obstacle;
		fireRate = 2000.0f;
		bulletSpeed = 20.0f;
	}

	else if (level > 4) {
		ammoType = EEnemyType::Seeker;
		fireRate = 3000.0f;
		bulletSpeed = 15.0f;
	}
	else {
		ammoType = EEnemyType::Basic;
	}

	if ((maxRadius = static_cast<float>(maxRadius - level)) > 8.0f) {}
	else { maxRadius = 8.0f; }
}

EnemyShooter::~EnemyShooter()
{

}

void EnemyShooter::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);
	
	MoveCheck();

	UpdateMotion(deltaTime_);

	Enemy::LimitCheck();

}

void EnemyShooter::Render()
{
	ColourProcess();

	Custom::DrawPoly(radius, position, rotation, sides, renderColour);
	Custom::DrawPoly(radius - 20.0f, position, rotation, sides, renderColour);
}

bool EnemyShooter::OnCollision(int damage_)
{
	Enemy::OnCollision(damage_);

	if (health > 18) {
		sides = 20;
		attack = 20;
	}
	else {
		sides = 3 + (int)(health / 2);
		attack = health;
	}

	if (health <= 0) {
		spawner->DestroyEnemy(this);
		return true;
	}

	return false;
}

void EnemyShooter::UpdateMotion(float deltaTime_)
{
	Enemy::UpdateMotion(deltaTime_);

	//Rotation
	rotation += rotationVel;
}

void EnemyShooter::MoveCheck()
{
	Vec2 playerDir = player->GetPosition() - position;
	float playerDist = Math::MagSq(playerDir);

	if (playerDist < searchRadius) {
		if (!timer.GetTimeCheck(1)) {
			SetVelocity(Vec2());
			return;
		}
		
		Vec2 velocity = Math::Normalize(playerDir) * bulletSpeed;
		
		if (ammoType == EEnemyType::Obstacle) {
			spawner->SpawnSingleEnemy(ammoType, 3, position, velocity);
		}
		else {
			spawner->SpawnSingleEnemy(ammoType, level, position, velocity);
		}
	}
	else {
		RadialPath();
	}
}

