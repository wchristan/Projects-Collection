#pragma once

#include "Enemy.h"
#include "EnemyBasic.h"
#include "EnemyBasicRadial.h"
#include "EnemySeeker.h"
#include "EnemyShooter.h"
#include "EnemyCore.h"
#include "EnemySpecial.h"
#include "EnemyObstacle.h"
#include "EnemyBasicSatellite.h"
#include "EnemyObsSatellite.h"
#include "EnemyBig.h"
#include "Timer.h"
#include "StageScenario.h"

class GameState;
class Player;

//Class that spawns the enemies and keep track of them
class Spawner
{
public:
	std::vector<Enemy*> enemies;
	std::vector<Enemy*> obstacles;
	std::vector<Enemy*> bossEnemies;
	
	inline Spawner() {
		isBossStage = false;
		spawnRate = 1000.0f;
		stageBreakTime = 5000.0f;
		lvTracker = 1;
		scoreGoal = 100; //This is temporary for quicker stage progression// It's a multiplier for score target
		smallEnemyScores = 0;
		bigEnemyScores = 0;
		minBuffSpawn = 1;
		maxBuffSpawn = 3;
		stageLvTracker = 1;

		shooterLv = 1;
		shooterInst = 1;
		coreLv = 1;
		coreInst = 1;

		gameState = nullptr;
		playerRef = nullptr;
		timer.SetTimer(stageBreakTime, 0, false);
		timer.SetTimer(spawnRate, 1, true);
		timer.SetTimer(stageBreakTime, 2, true);
		timer.ResetTimer(2);

		enemies.reserve(20);
		obstacles.reserve(20);

		stageS = StageScenario(Queries);
		SpawnerFPtr = &Spawner::SpawnDefaultEnemy;
		ScFunc = &StageScenario::Empty;
	}

	Spawner(GameState* gameState_, Player* player_);
	~Spawner();

	void CleanUp();
	void Update(float deltaTime_);
	void Render();
	void DestroyEnemy(int index_);
	void DestroyEnemy(void* addr_);
	void DestroyObstacle(int index_);
	void DestroyObstacle(void* addr_);
	void DestroyEnemyNoScore(void* addr_);
	void DestroyBoss(void* addr_);

	void SpawnSingleEnemy(EEnemyType type_, int level_, Vec2& position_, Vec2& velocity_);
	void* SpawnParts(int level_, Vec2& position_);

private:
	bool isBossStage;
	float spawnRate;
	float stageBreakTime;
	unsigned int lvTracker;
	unsigned int scoreGoal;
	unsigned short int minBuffSpawn;
	unsigned short int maxBuffSpawn;
	int smallEnemyScores;
	int bigEnemyScores;
	unsigned short int stageLvTracker;
	unsigned int shooterLv;
	unsigned int shooterInst;
	unsigned int coreLv;
	unsigned int coreInst;

	GameState* gameState;
	Player* playerRef;
	Timer timer;
	SpawnQuery DefaultQuery;
	BossQuery DefaultBossQuery;
	std::queue<SpawnQuery> Queries;
	std::queue<BossQuery> BossQueries;
	StageScenario stageS;

	void SpawnLogic();
	void ScoreCall(bool isBossPoint_);
	void EmptyFunction();
	void SpawnDefaultEnemy();
	void SpawnSpecial(int spawnInst_, bool randomPosition_, bool hasShield_);
	void SpawnQueries();
	void SpawnObstacles();
	void SpawnBoss();
	void SpawnEnemySpiral();

	//Function pointer used as function delegate
	void (Spawner::*SpawnerFPtr)();
	void (StageScenario::*ScFunc)();

	template<class Type>
	inline void RunFunction(Type* obj, void (Type::*f)()) {
		(obj->*f)();
	}

	template<typename Type>
	inline void ClearVector(std::vector<Type>& container_) {
		while (container_.size() > 0) {
			if (container_.back() != nullptr) {
				delete container_.back();
				container_.back() = nullptr;
			}
			container_.pop_back();
		}
	}
};

