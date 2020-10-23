#pragma once

#include "Enemy.h"


class EnemyBasicSatellite : public Enemy
{
public:
	EnemyBasicSatellite();
	EnemyBasicSatellite(Spawner* spawner_, float initialPosition_, int level_);
	virtual ~EnemyBasicSatellite();

	virtual void Update(float deltaTime_) override;
	virtual void Render() override;
	virtual bool OnCollision(int damage_) override;

protected:
	virtual void UpdateMotion(float deltaTime_) override;

};

