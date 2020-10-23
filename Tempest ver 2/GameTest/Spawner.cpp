#include "stdafx.h"
#include "Spawner.h"
#include "App/app.h"
#include "GameState.h"
#include "Player.h"
#include "Math.h"
#include <typeinfo>

#define DQ DefaultQuery

Spawner::Spawner(GameState* gameState_, Player* player_) : Spawner()
{
	gameState = gameState_;
	playerRef = player_;
}

Spawner::~Spawner()
{
	CleanUp();
}

void Spawner::CleanUp()
{
	ClearVector(obstacles);
	ClearVector(enemies);
	ClearVector(bossEnemies);

	if (gameState != nullptr) {
		gameState = nullptr;
	}
	if (playerRef != nullptr) {
		playerRef = nullptr;
	}
}

void Spawner::Update(float deltaTime_)
{
	timer.Update(deltaTime_);

	if (timer.GetTimeCheck(0)) {
		//The actual spawner func
		SpawnQueries();
		SpawnBoss();
	}
	if (timer.GetTimeCheck(2)) {
		if (Queries.size() > 0) return;
		//Scripted-continuous mobs
		(stageS.*ScFunc)();
		
	}

	//Update enemies
	if (obstacles.size() > 0) {
		for (int i = 0; i < obstacles.size(); i++) {
			obstacles[i]->Update(deltaTime_);
		}
	}

	if (enemies.size() > 0) {
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->Update(deltaTime_);
		}
	}

	//Update bosses
	if (bossEnemies.size() > 0) {
		for (int i = 0; i < bossEnemies.size(); i++) {
			bossEnemies[i]->Update(deltaTime_);
		}
	}
}

void Spawner::Render()
{
	if (obstacles.size() > 0) {
		for (int i = 0; i < obstacles.size(); i++) {
			obstacles[i]->Render();
		}
	}

	//Render enemies
	if (enemies.size() > 0) {
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->Render();
		}
	}

	//Render bosses
	if (bossEnemies.size() > 0) {
		for (int i = 0; i < bossEnemies.size(); i++) {
			bossEnemies[i]->Render();
		}
	}
}

void Spawner::SpawnLogic()
{
	//For quick testing
	if (lvTracker % 2 == 0) {
		//TESTS
	}

	//First round of 5 set
	if (lvTracker % 5 == 1) {
		//Increase default enemy lv
		DQ.enemyMaxLv ++;

		//Buff spawn every 3/5 sets
		int dice = static_cast<int>(FRAND_RANGE(minBuffSpawn, maxBuffSpawn));
		SpawnSpecial(dice, false, false);
	}
	//Special buff chance every stage that's not 1/5
	else {
		int dice = static_cast<int>(FRAND_RANGE(1, 20));
		if (dice > 19) {
			SpawnSpecial(1, false, false);
		}
	}
	
	//Second round
	if (lvTracker % 5 == 2) {
		stageS.SpawnShooter(shooterLv, shooterInst);
		shooterLv++;
	}

	//Third round
	if (lvTracker % 5 == 3) {
		stageS.SpawnCore(coreLv, coreInst);
		coreLv++;
	}

	//Fourth round //Bubbled buffs
	if (lvTracker % 5 == 4) {
		SpawnSpecial(maxBuffSpawn, false, true);
	}

	//Boss Stages at 5th round of 5 set
	if (lvTracker % 5 == 0) {
		BossQuery query;
		query.level = DefaultBossQuery.level;
		BossQueries.push(query);

		stageS.LevelInit(DefaultBossQuery.level, stageLvTracker);
		ScFunc = &StageScenario::StageSpawn;
		stageLvTracker++;
		if (stageLvTracker > 4) stageLvTracker = 1;

		if (lvTracker % 10 == 0) {
			//Spawn destructable shield
			stageS.SpawnBasicOrbit();
			stageS.IncBOrbitLv(1);
		}
		else if (lvTracker % 15 == 0) {
			//Spawn indestructable shield
			stageS.SetOOrbitLv();
			stageS.SpawnObsOrbit();
		}
		if (lvTracker % 20 == 0) {
			//Inc the basic shield number
			stageS.IncBOrbitAmount(2);
		}

		//Prevents regular enemy score to inc the stage level
		isBossStage = true;

		//Boost the lv of next boss
		DefaultBossQuery.level++;

		timer.SetTime(2, 10000.0f);
		timer.ResetTimer(1);
	}

	if (lvTracker % 10 == 0) {
		DQ.enemyMinLv++;
		DQ.spawnInst++;

		shooterInst++;
		coreInst++;
	}
	if (lvTracker % 25 == 0) {
		maxBuffSpawn++;
	}
	if (lvTracker % 30 == 0) {
		minBuffSpawn++;
	}

	//Stops default enemy spawn if Boss Stage is active
	if (isBossStage) SpawnerFPtr = &Spawner::EmptyFunction;
	else SpawnerFPtr = &Spawner::SpawnDefaultEnemy;
}

void Spawner::ScoreCall(bool isBossPoint_)
{
	//If boss is defeated, move to next stage
	if (isBossPoint_) {

	}
	//If enough enemies defeated move to next stage
	else {
		smallEnemyScores = 0;
	}

	lvTracker++;
	gameState->UpdateLevel(lvTracker);

	//Choose the next scenario
	SpawnLogic();

	//Break time
	timer.ResetTimer(0);
}

void Spawner::EmptyFunction()
{
}

void Spawner::SpawnSingleEnemy(EEnemyType type_, int level_, Vec2& position_, Vec2& velocity_)
{
	using E = EEnemyType;
	
	Enemy* enemy;

	switch (type_) {
	case E::Seeker:
		enemy = new EnemySeeker(this, level_, position_, playerRef);
		enemies.push_back(enemy);
		break;
	case E::Obstacle:
		enemy = new EnemyObstacle(this, level_, position_, velocity_);
		obstacles.push_back(enemy);
		break;
	case E::Basic:
		enemy = new EnemyBasic(this, level_, position_, velocity_);
		enemies.push_back(enemy);
	default:
		break;
	}

}

void* Spawner::SpawnParts(int level_, Vec2& position_)
{
	Enemy* enemy = nullptr;
	enemy = new EnemyBasic(this, level_, position_);
	enemies.push_back(enemy);

	return &enemies.back();
}

void Spawner::SpawnDefaultEnemy()
{
	if (timer.GetTimeSet(1) != DQ.cycleTime) {
		timer.SetTime(1, DQ.cycleTime);
	}

	if (!timer.GetTimeCheck(1)) return;
	
	//Spawn basic enemies from the center of the screen
	for (int i = 0; i < DQ.spawnInst; i++) {
		int level = DQ.GetLevel();
		float position = DQ.GetPosition();

		Enemy* enemy = nullptr;

		switch (DQ.enemyType) {
		case EEnemyType::BasicRadial:
			enemy = new EnemyBasicRadial(this, position, level);
			enemies.push_back(enemy);
			break;
		case EEnemyType::Seeker:
			enemy = new EnemySeeker(this, position, level, playerRef);
			enemies.push_back(enemy);
			break;
		case EEnemyType::Obstacle:
			enemy = new EnemyObstacle(this, position, DQ.GetLevel(), DQ.enemySpeed);
			obstacles.push_back(enemy);
			break;
		case EEnemyType::Basic:
		default:
			enemy = new EnemyBasic(this, position, level, 20.0f);
			enemies.push_back(enemy);
			break;
		}
	}
}

void Spawner::SpawnSpecial(int spawnInst_, bool randomPosition_, bool hasShield_)
{
	float position = 0.0f;
	EBuffType buffType = EBuffType::FireRate;

	for (int i = 0; i < spawnInst_; i++) {
		int dice = static_cast<int>(FRAND_RANGE(0, 101));

		//Buff chances
		if (dice > 90)
			buffType = EBuffType::Health;
		else if (dice <= 90 && dice > 65)
			buffType = EBuffType::FireRate;
		else if (dice <= 65 && dice > 40)
			buffType = EBuffType::BulletSpeed;
		else if (dice <= 40 && dice > 30)
			buffType = EBuffType::BulletLife;
		else if (dice <= 30 && dice > 7)
			buffType = EBuffType::Rebound;
		else if (dice <= 7 && dice > 2)
			buffType = EBuffType::Turret;
		else if (dice <= 2 && dice >= 1)
			buffType = EBuffType::Attack;

		if (randomPosition_)
			position = FRAND_RANGE(0.0f, 360.0f);
		else
			position += 360.0f / spawnInst_;

		Enemy* enemy = new EnemySpecial(this, *playerRef, position, buffType, hasShield_);
		enemies.push_back(enemy);
	}
}

void Spawner::SpawnQueries()
{
	if (Queries.size() == 0) {
		//If there's not queries then spawn default enemies
		//Default mobs
		(*this.*SpawnerFPtr)();
		return;
	}
	
	//Temporary variable
	SpawnQuery* query = &Queries.front();
	float position = 0.0f;
	int level = 0;

	if (timer.GetTimeSet(2) != query->completeTime) {
		timer.SetTime(2, query->completeTime);
	}

	if (timer.GetTimeSet(1) != query->cycleTime) {
		timer.SetTime(1, query->cycleTime);
	}
	
	if (!timer.GetTimeCheck(1)) return;
	

	//Spawn basic enemies from the center of the screen
	for (int i = 0; i < query->spawnInst; i++) {

		level = query->GetLevel();
		position = query->GetPosition();

		Enemy* enemy = nullptr;

		//Spawning sequence
		switch (query->enemyType) {
		case EEnemyType::BasicRadial:
			enemy = new EnemyBasicRadial(this, position, level);
			enemies.push_back(enemy);
			break;
		case EEnemyType::Seeker:
			enemy = new EnemySeeker(this, position, level, playerRef);
			enemies.push_back(enemy);
			break;
		case EEnemyType::Shooter:
			enemy = new EnemyShooter(this, position, level, playerRef);
			enemies.push_back(enemy);
			break;
		case EEnemyType::Core:
			enemy = new EnemyCore(this, position, level, playerRef);
			enemies.push_back(enemy);
			break;
		case EEnemyType::Obstacle:
			enemy = new EnemyObstacle(this, position, level, query->enemySpeed);
			obstacles.push_back(enemy);
			break;
		case EEnemyType::BasicSatellite:
			enemy = new EnemyBasicSatellite(this, position, level);
			enemies.push_back(enemy);
			break;
		case EEnemyType::ObsSatellite:
			enemy = new EnemyObsSatellite(this, position, level);
			obstacles.push_back(enemy);
			break;
		case EEnemyType::Basic:
		default:
			enemy = new EnemyBasic(this, position, level);
			enemies.push_back(enemy);
			break;
		}
	}

	//Clear completed query
	query->spawnCycle--;
	if (query->spawnCycle == 0) {
		Queries.pop();
	}
}

void Spawner::SpawnObstacles()
{
	//Temporary variable
	SpawnQuery query = DefaultQuery;
	float position = 0.0f;
	int level = 0;

	if (timer.GetTimeSet(1) != query.cycleTime) {
		timer.SetTime(1, query.cycleTime);
	}

	//Spawn basic enemies from the center of the screen
	for (int i = 0; i < query.spawnInst; i++) {

		level = DQ.GetLevel();
		position = DQ.GetPosition();

		//Spawning sequence
		Enemy* enemy = nullptr;

		enemy = new EnemyObstacle(this, position, level);
	}
}

void Spawner::SpawnBoss()
{
	while (BossQueries.size() > 0) {
		BossQuery* boss = &BossQueries.front();

		//Manage boss and its scenario & minions here
		Enemy* enemy = new EnemyBig(this, boss->level);
		bossEnemies.push_back(enemy);

		BossQueries.pop();
	}
}

void Spawner::SpawnEnemySpiral()
{
	//Spawn enemies in a circle
	//Used for when big enemies appear
	int spawnNum = 40;
	float angle = 360.0f / (float)spawnNum;

	for (int i = 0; i < spawnNum; i++) {

		Enemy* enemy = new EnemyBasic(this, angle * (float)i , 1);
		enemies.push_back(enemy);
	}
}

void Spawner::DestroyEnemy(int index_) 
{
	delete enemies[index_];
	enemies[index_] = nullptr;
	enemies.erase(enemies.begin() + index_);
}

void Spawner::DestroyEnemy(void* addr_)
{
	for (int i = 0; i < enemies.size(); i++) {
		if (&*enemies[i] == addr_) {
			//Update Score
			int score = enemies[i]->GetValue();
			gameState->UpdateScore(score);
			smallEnemyScores += score;

			if (smallEnemyScores >= (lvTracker * scoreGoal) && !isBossStage)
			ScoreCall(false);

			delete enemies[i];
			enemies[i]  = nullptr;
			enemies.erase(enemies.begin() + i);

			break;
		}
	}
}

void Spawner::DestroyObstacle(int index_)
{
	delete obstacles[index_];
	obstacles[index_] = nullptr;
	obstacles.erase(obstacles.begin() + index_);
}

void Spawner::DestroyObstacle(void* addr_)
{
	for (int i = 0; i < obstacles.size(); i++) {
		if (&*obstacles[i] == addr_) {
			delete obstacles[i];
			obstacles[i] = nullptr;
			obstacles.erase(obstacles.begin() + i);

			break;
		}
	}
}

void Spawner::DestroyEnemyNoScore(void* addr_)
{
	for (int i = 0; i < enemies.size(); i++) {
		if (&*enemies[i] == addr_) {

			delete enemies[i];
			enemies[i] = nullptr;
			enemies.erase(enemies.begin() + i);

			break;
		}
	}
}

void Spawner::DestroyBoss(void* addr_)
{
	for (int i = 0; i < bossEnemies.size(); i++) {
		if (&*bossEnemies[i] == addr_) {
			//Update Score
			int score = bossEnemies[i]->GetValue();
			gameState->UpdateScore(score);
			bigEnemyScores += score;

			if (bossEnemies.size() == 1) {
				isBossStage = false;
				ScoreCall(true);
			}

			delete bossEnemies[i];
			bossEnemies[i] = nullptr;
			bossEnemies.erase(bossEnemies.begin() + i);
		}
	}

	//Stop auto spawn obstacles/small enemies
	ScFunc = &StageScenario::Empty;
	
	//Clear leftover queries
	while (Queries.size() > 0) {
		Queries.pop();
	}

	//Clear obstacles
	ClearVector(obstacles);
}

