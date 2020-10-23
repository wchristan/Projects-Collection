#include "stdafx.h"
#include "EnemySpecial.h"
#include "Spawner.h"
#include "Player.h"


EnemySpecial::EnemySpecial()
{
	spawnRadius = 100.0f;
	rotationVel = 1.0f;
	radialPosition = 0.0f;
	radius = 15.0f;
	shieldRadius = 40.0f;
	shieldHealth = 6;
	shieldSides = 8;
	speedMultiplier = 40.0f;
	buffIndex = EBuffType::FireRate;

	playerRef = nullptr;

	level = 1;
	sides = 6;
	uniqueSides = 3;
	health = 5;
	attack = 0;
	value = 50;

	shieldOn = false;
}

EnemySpecial::EnemySpecial(Spawner* spawner_, Vec2& initialPos_, EBuffType buffIndex_) : EnemySpecial()
{
	spawner = spawner_;
	position = initialPos_;
	
	buffIndex = buffIndex_;
}

EnemySpecial::EnemySpecial(Spawner* spawner_, Player& playerRef_, float initialPosition_, EBuffType buffIndex_, bool shieldOn_) : EnemySpecial()
{
	spawner = spawner_;
	playerRef = &playerRef_;

	radialPosition = initialPosition_;
	position.x = sinf(radialPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(radialPosition * RAD) * spawnRadius + ScreenCenter.y;

	shieldOn = shieldOn_;
	if (shieldOn) {
		value = 50;
		LinearPath();
		isMovingLinear = true;
	}

	buffIndex = buffIndex_;

	switch (buffIndex) {
	case EBuffType::FireRate:
		uniqueSides = 3;
		defColour = Vec3(0.9f, 0.9f, 0.1f); // Yellow
		break;
	case EBuffType::BulletSpeed:
		uniqueSides = 3;
		defColour = Vec3(0.6f, 0.4f, 0.0f); // Brown
		break;
	case EBuffType::BulletLife:
		uniqueSides = 3;
		defColour = Vec3(0.2f, 0.6f, 0.6f); // Dark Cyan
		break;
	case EBuffType::Attack:
		uniqueSides = 4;
		defColour = Vec3(0.8f, 0.2f, 0.0f); // Red
		break;
	case EBuffType::Rebound:
		uniqueSides = 4;
		defColour = Vec3(0.0f, 0.9f, 0.3f); // Green
		break;
	case EBuffType::Turret:
		uniqueSides = 5;
		defColour = Vec3(0.4f, 0.0f, 0.6f); // Purple
		break;
	case EBuffType::Health:
		uniqueSides = 6;
		defColour = Vec3(0.1f, 0.9f, 0.9f); // Cyan
		break;
	default:
		defColour = Vec3(0.9f, 0.9f, 0.1f); // Orange
		break;
	}
}

void EnemySpecial::LevelInitializer(int level_)
{
	level = level_;

	sides = 6 * level;
	health = 1 + level;
	attack = level;
	value = 10 * level;
}

EnemySpecial::~EnemySpecial()
{
	playerRef = nullptr;
}

void EnemySpecial::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);

	UpdateMotion(deltaTime_);

	Enemy::LimitCheck();
}

void EnemySpecial::Render()
{
	ColourProcess();

	//Draw Core
	Custom::DrawPoly(radius - 6.0f, position, rotation, uniqueSides, renderColour);
	Custom::DrawPoly(radius, position, rotation, sides, renderColour);
	
	//Draw Shield
	if (shieldOn) {
		Custom::DrawPoly(shieldRadius, position, -rotation, shieldSides, renderColour);
		Custom::DrawPoly(shieldRadius - 5.0f, position, -rotation, shieldSides, renderColour);
	}
}

bool EnemySpecial::OnCollision(int damage_)
{
	if (shieldOn) {
		shieldHealth -= damage_;
		shieldSides = shieldHealth + 2;

		if (shieldHealth <= 0) {
			shieldOn = false;
		}
		return false;
	}

	Enemy::OnCollision(damage_);

	sides = 2 + health;

	if (health <= 0) {
		//Give player buff before death
		if (playerRef != nullptr) {
			switch (buffIndex) {
			case EBuffType::Health:
				playerRef->IncreaseHealth(5);
				break;
			case EBuffType::Attack:
				playerRef->BuffAttack();
				break;
			case EBuffType::Rebound:
				playerRef->BuffBulletRebound();
				break;
			case EBuffType::FireRate:
				playerRef->BuffFireRate();
				break;
			case EBuffType::BulletSpeed:
				playerRef->BuffBulletSpeed();
				break;
			case EBuffType::BulletLife:
				playerRef->BuffBulletLife();
				break;
			case EBuffType::Turret:
				playerRef->BuffExtraFire();
				break;
			default:
				break;
			}
		}

		//Dies
		spawner->DestroyEnemy(this);
		return true;
	}

	return false;
}

float EnemySpecial::GetRadius()
{
	if (shieldOn) return shieldRadius;
	return radius;
}

void EnemySpecial::UpdateMotion(float deltaTime_)
{
	if (!isMovingLinear) RadialPath();
	
	rotation += rotationVel;

	Enemy::UpdateMotion(deltaTime_);
}

void EnemySpecial::ColourProcess()
{
	Enemy::ColourProcess();

}
