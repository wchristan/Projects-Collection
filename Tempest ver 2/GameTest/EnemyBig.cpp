#include "stdafx.h"
#include "EnemyBig.h"
#include "Spawner.h"


EnemyBig::EnemyBig()
{

}

EnemyBig::EnemyBig(Spawner* spawner_, int level_) : EnemyBig()
{
	spawner = spawner_;
	level = level_;

	position = ScreenCenter;
	radius = 100.0f;

	sides = 3 + level_;
	attack = 1;
	health = 10 * level_;

	rotationVel = 1.0f;
}

EnemyBig::~EnemyBig()
{

}

void EnemyBig::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);
	rotation += rotationVel;
}

void EnemyBig::Render()
{
	ColourProcess();

	Custom::DrawPoly(radius - 10.0f, position, rotation, sides, renderColour);
	Custom::DrawPoly(radius, position, rotation, sides, renderColour);
}

bool EnemyBig::OnCollision(int damage_)
{
	Enemy::OnCollision(damage_);

	if (health % 10 == 0) {
		sides = health / 10 + 3;
	}

	if (health <= 0) {
		spawner->DestroyBoss(this);
		return true;
	}
	return false;
}

