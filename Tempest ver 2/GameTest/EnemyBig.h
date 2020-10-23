#pragma once

#include "Enemy.h"

//Big/Boss type enemy inherited from Enemy
//Stationary and has more health
class EnemyBig : public Enemy
{
public:

	EnemyBig();
	EnemyBig(Spawner* spawner_, int level_);
	virtual ~EnemyBig() override;

	virtual void Update(float deltaTime_);
	virtual void Render();
	virtual bool OnCollision(int damage_) override;

protected:

};

