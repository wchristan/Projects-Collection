#pragma once
#include "Enemy.h"
#include "StageScenario.h"

class Player;

class EnemyShooter : public Enemy
{
public:
	EnemyShooter();
	EnemyShooter(Spawner* spawner_, float initialPosition_, int level_, Player* player_);
	virtual ~EnemyShooter();
	virtual void Update(float deltaTime_);
	virtual void Render();
	virtual bool OnCollision(int damage_);

protected:
	Player* player;
	bool foundPlayer;
	float searchRadius;
	float bulletSpeed;
	float fireRate;
	EEnemyType ammoType;

	virtual void UpdateMotion(float deltaTime_) override;
	virtual void MoveCheck();
};

