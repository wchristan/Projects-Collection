#include "stdafx.h"
#include "Bullet.h"
#include "Custom.h"
#include "Math.h"

Bullet::Bullet()
{
	Initialize();
}

Bullet::Bullet(Vec2& initPosition_, Vec2& initVelocity_, float initRotation_, unsigned int rebound_, float lifeSpan_) : Bullet()
{
	position = initPosition_;
	moveSpeed = initVelocity_;
	rotation = initRotation_;
	sides = 3;
	rebound = rebound_;
	lifeSpan = lifeSpan_;
}

Bullet::Bullet(Vec2& initPosition_, Vec2& initVelocity_, float initRotation_, int initSides_, unsigned int rebound_, float lifeSpan_, Vec3& initColour_) : Bullet()
{
	position = initPosition_;
	moveSpeed = initVelocity_;
	rotation = initRotation_;
	sides = initSides_;
	colour = initColour_;
	rebound = rebound_;
	lifeSpan = lifeSpan_;
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	//Set initial stats
	lifeSpan = 2000.0f;
	radius = 5.0f;
	rebound = 0;
	position = Vec2();
	moveSpeed = Vec2();
	rotation = 0.0f;
	sides = 3;
	colour = Vec3(0.5f, 0.7f, 1.0f);
}

void Bullet::Update(float deltaTime_)
{
	Move(deltaTime_);
}

void Bullet::Render()
{
	DrawShape();
}

void Bullet::SetReflection(Vec2& hitNormal_)
{
	//moveSpeed = newVelocity_;

	Vec2 N = Math::Normalize(hitNormal_);
	Vec2 V1 = moveSpeed;

	float dot = 2.0f * (Math::Dot(V1, N));
	moveSpeed = V1 - (N * dot);
}

Vec2 Bullet::GetPosition()
{
	return position;
}

float Bullet::GetRadius()
{
	return radius;
}

void Bullet::MoveBullet(float enemyRadius_)
{
	position += Math::Normalize(moveSpeed) * ((radius * 2.0f) + (enemyRadius_ * 2.0f));
}

void Bullet::Move(float deltaTime_)
{
	position += moveSpeed * (deltaTime_/1000.0f);
	lifeSpan -= deltaTime_;
}

void Bullet::DrawShape()
{
	Custom::DrawPoly(radius, position, rotation, sides, colour);
}
