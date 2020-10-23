#pragma once

#include "Vector.h"
#include "GetterInterface.h"


//Base projectile class
class Bullet : public IGetter
{
public:
	//Variables
	Vec2 moveSpeed;
	Vec2 position;
	unsigned int rebound;
	float rotation;
	float lifeSpan;
	float radius;
	int sides;
	Vec3 colour;

	//Functions
	Bullet();
	Bullet(Vec2& initPosition_, Vec2& initVelocity_, float initRotation_, unsigned int rebound_, float lifeSpan_);
	Bullet(Vec2& initPosition_, Vec2& initVelocity_, float initRotation_, int initSides_, unsigned int rebound_, float lifeSpan_, Vec3& initColour_);
	~Bullet();

	virtual void Initialize();
	virtual void Update(float deltaTime_);
	virtual void Render();

	virtual void SetReflection(Vec2& hitNormal_);

	virtual Vec2 GetPosition() override;
	virtual float GetRadius() override;
	void MoveBullet(float enemyRadius_);

protected:

	//Functions
	virtual void Move(float deltaTime_);
	virtual void DrawShape();

};

