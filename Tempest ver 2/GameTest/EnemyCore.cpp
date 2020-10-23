#include "stdafx.h"
#include "EnemyCore.h"
#include "Spawner.h"
#include "Player.h"

EnemyCore::EnemyCore() : EnemyShooter()
{
	spawnRadius = 50.0f;
	rotationVel = 1.5f;
	radius = 50.0f;
	speedMultiplier = 30.0f;
	searchRadius = 400.0f;
	searchRadius *= searchRadius; //I'm using MagSq
	bulletSpeed = 10.0f; //max 25.0f
	fireRate = 4000.0f; //max 1000.0f
	maxNodes = 4;

	nodeRevTracker = 0.0f;
	nodeRevSpeed = 0.05f;
	revOrbitSpeed = 0.5f;
	revOutwardSpeed = 0.4f;
	foundPlayer = false;

	defColour = Custom::Pink;

	timer.SetTimer(fireRate, 1, true);
	player = nullptr;

	ammoType = EEnemyType::Basic;
}

EnemyCore::EnemyCore(Spawner* spawner_, float initialPosition_, int level_, Player* player_) : EnemyCore()
{
	spawner = spawner_;
	player = player_;
	radialPosition = initialPosition_;

	float angPosition = initialPosition_;
	position.x = sinf(angPosition * RAD) * spawnRadius + ScreenCenter.x;
	position.y = cosf(angPosition * RAD) * spawnRadius + ScreenCenter.y;

	level = level_;
	sides = 3 + level;
	health = 2 + (level * 2);
	attack = 1 + level;
	value = 100 * level;

	int nodeScaling = (int)(level / 2);
	maxNodes = 4 + nodeScaling;
	if (maxNodes > 32) maxNodes = 32;
	
	nodes.reserve(maxNodes);

	if (level > 10) {
		ammoType = EEnemyType::Obstacle;
		fireRate = 2000.0f;
		bulletSpeed = 20.0f;
	}
	else if (level > 4) {
		ammoType = EEnemyType::Seeker;
		fireRate = 3000.0f;
		bulletSpeed = 15.0f;
	}
	else {
		ammoType = EEnemyType::Basic;
	}

	if ((maxRadius = static_cast<float>(maxRadius - level)) > 8.0f) {}
	else { maxRadius = 8.0f; }

	float angleGap = 360.0f / (float)maxNodes;
	
	for (int i = maxNodes; i > 0; i--) {
		Vec2* nodePosition = new Vec2();
		
		nodePosition->x = sinf(angleGap * (float)i * RAD) * 20.0f + position.x;						   
		nodePosition->y = cosf(angleGap * (float)i * RAD) * 20.0f + position.y;
		nodes.push_back(nodePosition);
		
		nodesAddr.push_back(spawner->SpawnParts(level, *nodes.back()));
	}
}

EnemyCore::~EnemyCore()
{
	for (Vec2*& node : nodes) {
		delete node;
		node = nullptr;
	}
}

void EnemyCore::Update(float deltaTime_)
{
	Enemy::Update(deltaTime_);

	MoveCheck();

	UpdateMotion(deltaTime_);

	Enemy::LimitCheck();
}

void EnemyCore::Render()
{
	ColourProcess();

	Custom::DrawPoly(radius, position, rotation, sides, renderColour);
	Custom::DrawPoly(radius - 10.0f, position, rotation, sides, renderColour);
}

bool EnemyCore::OnCollision(int damage_)
{
	Enemy::OnCollision(damage_);

	if (health > 18) {
		sides = 20;
		attack = 20;
	}
	else {
		sides = 3 + (int)(health / 2); 
		attack = health;
	}

	if (health <= 0) {
		for (void* address : nodesAddr) {
			spawner->DestroyEnemyNoScore(address);
			address = nullptr;
		}

		spawner->DestroyEnemy(this);
		return true;
	}

	return false;
}

void EnemyCore::UpdateMotion(float deltaTime_)
{
	Enemy::UpdateMotion(deltaTime_);

	nodeRevTracker += nodeRevSpeed;

	if (nodes.size() > 0) {
		//Update nodes' positions
		float angleGap = 360.0f / (float)maxNodes;

		for (int i = 0; i < maxNodes; i++) {
			Vec2 nodePosition;

			nodes[i]->x = sinf(angleGap * (float)i * RAD + nodeRevTracker) * 60.0f + position.x;
			nodes[i]->y = cosf(angleGap * (float)i * RAD + nodeRevTracker) * 60.0f + position.y;
		}
	}
}

void EnemyCore::MoveCheck()
{
	Vec2 playerDir = player->GetPosition() - position;
	float playerDist = Math::MagSq(playerDir);

	if (playerDist < searchRadius) {
		if (!timer.GetTimeCheck(1)) {
			SetVelocity(Vec2());
			return;
		}

		Vec2 velocity = Math::Normalize(playerDir) * bulletSpeed;

		if (ammoType == EEnemyType::Obstacle) {
			spawner->SpawnSingleEnemy(ammoType, 3, position, velocity);
		}
		else {
			spawner->SpawnSingleEnemy(ammoType, level, position, velocity);
		}
	}
	else {
		RadialPath();
	}
}
