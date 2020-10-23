#pragma once

#include "Enemy.h"

class Player;

class EnemySeeker : public Enemy
{
public:
	EnemySeeker();
	EnemySeeker(Spawner* spawner_, int level_, Vec2& initialPos_, Player* player_);
	EnemySeeker(Spawner* spawner_, float initialPos_, int level_, Player* player_);
	virtual ~EnemySeeker() override;

	virtual void Update(float deltaTime_) override;
	virtual void Render() override;
	virtual bool OnCollision(int damage_) override;

protected:
	Player* player;

	virtual void UpdateMotion(float deltaTime_) override;

};

