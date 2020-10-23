#pragma once

#include "Enemy.h"

class Player;


class EnemySpecial : public Enemy
{
public:
	EnemySpecial();
	EnemySpecial(Spawner* spawner_, Vec2& initialPos_, EBuffType buffIndex_);
	EnemySpecial(Spawner* spawner_, Player& playerRef_, float initialPosition_, EBuffType buffIndex_, bool shieldOn_);
	virtual ~EnemySpecial() override;

	virtual void Update(float deltaTime_) override;
	virtual void Render() override;
	virtual bool OnCollision(int damage_) override;
	virtual float GetRadius() override;

protected:
	Player* playerRef;
	EBuffType buffIndex;

	unsigned short int uniqueSides;
	unsigned short int shieldSides;
	int shieldHealth;
	float shieldRadius;
	bool shieldOn;
	bool isMovingLinear;

	virtual void UpdateMotion(float deltaTime_) override;
	virtual void ColourProcess() override;
	
	void LevelInitializer(int level_);

};

