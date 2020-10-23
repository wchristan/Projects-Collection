#pragma once

#include "Vector.h"
#include "Math.h"
#include "App/AppSettings.h"
#include "Custom.h"
#include "Timer.h"
#include "GetterInterface.h"


class Spawner;

//Base enemy class
class Enemy : public IGetter
{
public:
	Vec2 ScreenCenter;
	Vec2 ScreenMax;
	
	Enemy();
	Enemy(Spawner* spawner_);
	Enemy(Spawner* spawner_, Vec2& initialPos_);
	virtual ~Enemy();
	
	virtual void Update(float deltaTime_);
	virtual void Render();
	virtual bool OnCollision(int damage_);	

	int GetHealth();
	int GetAttack();
	int GetValue();
	virtual float GetRadius() override;
	virtual Vec2 GetPosition() override;

protected:
	//Basic Variables
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	float radialPosition;
	float spawnRadius;
	float speedMultiplier;
	float mass;
	float rotation;
	float rotationVel;
	float radius;
	float maxRadius;
	int sides;

	//FX Variables
	Vec3 renderColour;
	Vec3 defColour;
	Vec3 dmgColour;

	//Stats Variables
	int level;
	int health;
	int attack;
	int value;
	float maxTravelDist;
	float revOrbitSpeed;
	float revOutwardSpeed;


	//FX Variables
	bool isReactToDmg;
	float colDown;

	virtual void UpdateMotion(float deltaTime_);
	virtual void ColourProcess();
	virtual void LimitCheck();
	virtual void LinearPath();
	virtual void RadialPath();
	virtual void OrbitPath();

	void AddForce(Vec2& newForce_);
	void SetForce(Vec2& newForce_);
	void SetAcceleration(Vec2& newAcceleration_);
	void SetSpeedMult(float value_);
	void SetVelocity(Vec2& newVelocity_);

	Timer timer;
	Spawner* spawner;
};

