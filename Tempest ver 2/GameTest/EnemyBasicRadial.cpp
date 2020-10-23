#include "stdafx.h"
#include "EnemyBasicRadial.h"
#include "Spawner.h"

EnemyBasicRadial::EnemyBasicRadial()
{
	spawnRadius = 100.0f;
	rotationVel = 1.0f;
}

EnemyBasicRadial::EnemyBasicRadial(Spawner* spawner_, float initialPosition_, int level_) : EnemyBasicRadial()
{
	spawner = spawner_;

	radialPosition = initialPosition_;
	position.x = sinf(radialPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(radialPosition * RAD) * spawnRadius + ScreenCenter.y;

	StatsInitializer(level_);
}

EnemyBasicRadial::EnemyBasicRadial(Spawner* spawner_, float initialPosition_, int level_, float speed_) : EnemyBasicRadial(spawner_, initialPosition_, level_)
{
	speedMultiplier = speed_;
}

EnemyBasicRadial::~EnemyBasicRadial()
{
}

void EnemyBasicRadial::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);

	UpdateMotion(deltaTime_);

	Enemy::LimitCheck();
}

void EnemyBasicRadial::Render()
{
	Enemy::Render();

}

bool EnemyBasicRadial::OnCollision(int damage_)
{
	return EnemyBasic::OnCollision(damage_);
}

void EnemyBasicRadial::UpdateMotion(float deltaTime_)
{
	Enemy::UpdateMotion(deltaTime_);

	//Rotation
	rotation += rotationVel;

	//Scale
	if (radius < maxRadius) {
		radius = fabsf(Math::Mag(position - ScreenCenter) / 10.0f);
	}

	RadialPath();
}
