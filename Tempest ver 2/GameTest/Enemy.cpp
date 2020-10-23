#include "stdafx.h"
#include "Enemy.h"
#include "Spawner.h"



Enemy::Enemy()
{
	ScreenCenter = Vec2(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2);
	ScreenMax = Vec2(APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);

	spawner = nullptr;

	level = 1;
	health = 1;
	attack = 1;
	value = 1;
	maxTravelDist = 640000.0f; //800.0f ^ 2

	position = ScreenCenter;
	velocity = Vec2();
	acceleration = Vec2();
	radialPosition = 0.0f;
	spawnRadius = 0.0f;
	speedMultiplier = 1.0f;
	mass = 1.0f;

	rotation = 0.0f;
	rotationVel = 0.0f;
	radius = 8.0f;
	maxRadius = 50.0f;
	sides = 4;
	
	revOrbitSpeed = 0.5f;
	revOutwardSpeed = 0.2f;
	
	renderColour = Vec3();
	defColour = Custom::White;
	dmgColour = Vec3(1.0f, 0.0f, 0.0f);

	//FX 
	colDown = 1000.0f;
	isReactToDmg = false;

	timer.SetTimer(colDown, 0, false);
}

Enemy::Enemy(Spawner* spawner_): Enemy()
{
	spawner = spawner_;
}

Enemy::Enemy(Spawner* spawner_, Vec2& initialPos_) : Enemy()
{
	spawner = spawner_;
	position = initialPos_;
}

Enemy::~Enemy()
{
	spawner = nullptr;
}


void Enemy::Update(float deltaTime_)
{
	//FX
	timer.Update(deltaTime_);
	
	if (timer.GetTimeCheck(0)) isReactToDmg = false;
}

void Enemy::UpdateMotion(float deltaTime_)
{
	float deltaTime = deltaTime_ / 1000.0f;

	velocity += acceleration * deltaTime;
	position += velocity * deltaTime * speedMultiplier;
}

void Enemy::Render()
{
	ColourProcess();

	Custom::DrawPoly(radius, position, rotation, sides, renderColour);
}

void Enemy::ColourProcess()
{
	//Change colour when damaged
	if (!isReactToDmg) {
		renderColour = defColour;
	}
	else {
		Custom::RandomColour(dmgColour);
		renderColour = dmgColour;
	}
}

void Enemy::AddForce(Vec2& newForce_)
{
	acceleration += newForce_ / mass;
}

void Enemy::SetForce(Vec2& newForce_)
{
	acceleration = newForce_ / mass;
}

void Enemy::SetAcceleration(Vec2& newAcceleration_)
{
	acceleration = newAcceleration_;
}

void Enemy::SetVelocity(Vec2& newVelocity_)
{
	velocity = newVelocity_	;
}

void Enemy::SetSpeedMult(float value_)
{
	speedMultiplier = value_;
}

int Enemy::GetHealth()
{
	return health;
}

int Enemy::GetAttack()
{
	return attack;
}

int Enemy::GetValue()
{
	return value;
}

float Enemy::GetRadius()
{
	return radius;
}

Vec2 Enemy::GetPosition()
{
	return position;
}

void Enemy::LimitCheck()
{
	float distSq = Math::MagSq(ScreenCenter - position);

	if (distSq > maxTravelDist) {
		spawner->DestroyEnemyNoScore(this);
	}
}

void Enemy::LinearPath()
{
	//Move the enemy outward away from the center
	Vec2 direction = position - ScreenCenter;
	direction = Math::Normalize(direction);

	SetVelocity(direction);
}

void Enemy::RadialPath()
{
	//Spiral outward
	radialPosition += revOrbitSpeed;
	spawnRadius += revOutwardSpeed;
	position.x = sinf(radialPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(radialPosition * RAD) * spawnRadius + ScreenCenter.y;
}

void Enemy::OrbitPath()
{
	radialPosition += 0.5f;
	position.x = sinf(radialPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(radialPosition * RAD) * spawnRadius + ScreenCenter.y;
}

bool Enemy::OnCollision(int damage_)
{
	isReactToDmg = true;
	timer.ResetTimer(0);

	health -= damage_;
	
	return false;
}

