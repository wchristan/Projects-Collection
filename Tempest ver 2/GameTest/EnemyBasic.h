#pragma once

#include "Enemy.h"

class EnemyCore;

//Basic/Common type of enemy inherited from Enemy 
//This enemy will move outward from the center 
class EnemyBasic : public Enemy
{
public:
	EnemyBasic();
	EnemyBasic(Spawner* spawner_, int level_, Vec2& initialPos_, Vec2& initialVelocity_);
	EnemyBasic(Spawner* spawner_, float initialPosition_, int level_);
	EnemyBasic(Spawner* spawner_, float initialPosition_, int level_, float speed_);
	EnemyBasic(Spawner* spawner_, int level_, Vec2& initialPos_);
	virtual ~EnemyBasic() override;

	virtual void StatsInitializer(int level_);
	virtual void Update(float deltaTime_) override;
	virtual void Render() override;
	virtual bool OnCollision(int damage_) override;

protected:
	Vec2* nodePosition;
	bool followMode;

	virtual void UpdateMotion(float deltaTime_) override;
	virtual void LinearPath() override;
	virtual void LimitCheck() override;
};

