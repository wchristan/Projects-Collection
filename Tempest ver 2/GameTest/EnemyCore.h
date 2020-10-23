#pragma once
#include "EnemyShooter.h"
#include "StageScenario.h"

class Player;

class EnemyCore : public EnemyShooter
{
public:
	EnemyCore();
	EnemyCore(Spawner* spawner_, float initialPosition_, int level_, Player* player_);
	virtual ~EnemyCore();

	virtual void Update(float deltaTime_);
	virtual void Render();
	virtual bool OnCollision(int damage_);

protected:
	Player* player;
	bool foundPlayer;
	float searchRadius;
	float bulletSpeed;
	float fireRate;
	float nodeRevTracker;
	float nodeRevSpeed;
	int maxNodes;
	EEnemyType ammoType;

	std::vector<Vec2*> nodes;
	std::vector<void*> nodesAddr;

	virtual void UpdateMotion(float deltaTime_) override;
	virtual void MoveCheck();
};

