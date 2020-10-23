#pragma once

#include <vector>
#include "Math.h"
#include "Bullet.h"
#include "Timer.h"
#include "GetterInterface.h"

#define FIRETIMER 0
#define DMGTIMER 1

class GameState;

//Player class, using the ship avatar
class Player : public IGetter
{
public:
	class CSimpleSprite* ship;
	std::vector<Bullet*> bullets;
	
	Vec2 screenCenter;
	Vec2 mousePosition;
	Vec2 shipPosition;
	
	Player();
	Player(GameState* gameState_);
	~Player();
	void CleanUp();

	void Update(float deltatime_);
	void Render();
	void OnCollision(int type_, int value_);

	int GetHealth();
	int GetAttack();
	Vec2 GetPosition() override;
	float GetRadius() override;
	float GetBulletSpeed();

	//Buff and Stat Bonuses
	void IncreaseHealth(int value_);
	void BuffAttack();
	void BuffFireRate();
	void BuffBulletSpeed();
	void BuffBulletRebound();
	void BuffBulletLife();
	void BuffExtraFire();

	void MoveShip();
	void RotateShip();
	void Shoot();
	void DestroyBullet(int index_);

private:
	//Player Stats
	unsigned int health;
	unsigned int attack;
	unsigned int maxAttack;
	float fireRate;
	float maxFireRate;
	float moveSpeed;
	float pathRotation;
	float colliderRadius;
	float pathRadius;
	float startLocation;

	//Stats
	float bulletInitDistance;
	float bulletSpeed;
	float maxBulletSpeed;
	float bulletLife;
	float maxBulletLife;
	unsigned int bulletRebound;
	short unsigned int bulletSpawnPoints;

	//FX Variables
	Vec3 defColour;
	Vec3 dmgColour;

	float dmgReactTime;

	Timer timer;
	GameState* gameState;

	void InputUpdate(float deltaTime_);
	Vec2 CalcBulletSpawn(Vec2& spawn_, float angleDev_);
};

