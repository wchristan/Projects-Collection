#pragma once
//#include "Enemy.h"
#include "EnemyBasic.h"

class EnemyBasicRadial : public EnemyBasic
{
public:
	EnemyBasicRadial();
	EnemyBasicRadial(Spawner* spawner_, float initialPosition_, int level_);
	EnemyBasicRadial(Spawner* spawner_, float initialPosition_, int level_, float speed_);
	virtual ~EnemyBasicRadial() override;

	virtual void Update(float deltaTime_) override;
	virtual void Render() override;
	virtual bool OnCollision(int damage_) override;

protected:
	virtual void UpdateMotion(float deltaTime_) override;


};

