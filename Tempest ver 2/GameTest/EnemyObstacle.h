#pragma once

#include "Enemy.h"

class EnemyObstacle : public Enemy
{
public:
	EnemyObstacle();
	EnemyObstacle(Spawner* spawner_, int level_, Vec2& initialPos_, Vec2& initialVelocity_);
	EnemyObstacle(Spawner* spawner_, float initialPosition_, int level_);
	EnemyObstacle(Spawner* spawner_, float initialPosition_, int level_, float speed_);
	virtual ~EnemyObstacle() override;

	virtual void Update(float deltaTime_) override;
	virtual void Render() override;
	virtual bool OnCollision(int damage_) override;

protected:
	float spawnRadius;

	virtual void UpdateMotion(float deltaTime_) override;
	virtual void ColourProcess() override;
	virtual void LimitCheck() override;
};

