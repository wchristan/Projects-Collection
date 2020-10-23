#include "stdafx.h"
#include "EnemySeeker.h"
#include "Spawner.h"
#include "Player.h"

EnemySeeker::EnemySeeker()
{
	spawnRadius = 100.0f;
	rotationVel = 1.0f;
	radius = 20.0f;
	player = nullptr;
	defColour = Custom::Cyan;
}

EnemySeeker::EnemySeeker(Spawner* spawner_, int level_, Vec2& initialPos_, Player* player_) : EnemySeeker()
{
	spawner = spawner_;
	position = initialPos_;
	player = player_;

	health = level + 1;

	if (level_ <= 1) sides = 3;
	else sides = level + 2;

	value = level * 10;
}

EnemySeeker::EnemySeeker(Spawner* spawner_, float initialPos_, int level_, Player* player_) : EnemySeeker()
{
	spawner = spawner_;
	level = level_;
	player = player_;
	health = level + 1;
	
	if (level_ <= 1) sides = 3;
	else sides = level + 2;

	value = level * 10;

	float angPosition = initialPos_;

	position.x = sinf(angPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(angPosition * RAD) * spawnRadius + ScreenCenter.y;
}

EnemySeeker::~EnemySeeker()
{
	if (player != nullptr) {
		player = nullptr;
	}
}

void EnemySeeker::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);

	UpdateMotion(deltaTime_);

	Enemy::LimitCheck();
}

void EnemySeeker::Render()
{
	Enemy::Render();
}

bool EnemySeeker::OnCollision(int damage_)
{
	Enemy::OnCollision(damage_);

	sides = 2 + health;

	if (health <= 0) {
		spawner->DestroyEnemy(this);
		return true;
	}

	return false;
}

void EnemySeeker::UpdateMotion(float deltaTime_)
{
	Enemy::UpdateMotion(deltaTime_);

	float playerDistSq = Math::MagSq(player->GetPosition()- position);
	playerDistSq *= 0.001f;

	//Movement
	Vec2 newAccel;
	newAccel = Math::Normalize(player->GetPosition() - position);

	newAccel = newAccel * playerDistSq;
	SetAcceleration(newAccel);
	
	//Rotation
	rotation += rotationVel + (playerDistSq * 0.1f);
}
