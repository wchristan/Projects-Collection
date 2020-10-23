#pragma once
#include "Enemy.h"

class EnemyObsSatellite : public Enemy
{
public:
	EnemyObsSatellite();
	EnemyObsSatellite(Spawner* spawner_, float initialPosition_, int level_);
	virtual ~EnemyObsSatellite();

	virtual void Update(float deltaTime_) override;
	virtual void Render() override;
	virtual bool OnCollision(int damage_) override;

protected:
	virtual void UpdateMotion(float deltaTime_) override;
};

