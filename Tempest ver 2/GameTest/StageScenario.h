#pragma once
#include <queue>
#include "App/AppSettings.h"
#include "Custom.h"


enum class EEnemyType : short {
	Basic = 1,
	BasicRadial,
	Seeker,
	Shooter,
	Core,
	Special,
	Obstacle,
	BasicSatellite,
	ObsSatellite
};

struct SpawnQuery {
	EEnemyType enemyType;
	int enemyMinLv;
	int enemyMaxLv;
	float enemySpeed;
	int spawnInst;
	int spawnCycle;
	int cycleDiv;
	float cycleTime;
	float completeTime;
	float position;

	SpawnQuery() {
		enemyType = EEnemyType::Basic;
		enemyMinLv = 0;
		enemyMaxLv = 1;
		enemySpeed = 50.0f;
		spawnInst = 1;
		spawnCycle = 1;
		cycleDiv = 1;
		formation = 1;
		cycleTime = 1000.0f;
		completeTime = 1000.0f;
		position = 0.0f;
		positionOffset = 0.0f;
		angleGap = 0.0f;
		cycleCounter = 0;
		cycleCap = 0;
	}

	inline SpawnQuery operator = (const SpawnQuery& query_) {
		enemyType = query_.enemyType;
		enemyMinLv = query_.enemyMinLv;
		enemyMaxLv = query_.enemyMaxLv;
		enemySpeed = query_.enemySpeed;
		spawnInst = query_.spawnInst;
		spawnCycle = query_.spawnCycle;
		cycleDiv = query_.cycleDiv;
		formation = query_.formation;
		cycleTime = query_.cycleTime;
		completeTime = query_.completeTime;
		position = query_.position;
		angleGap = query_.angleGap;
		positionOffset = query_.positionOffset;

		return *this;
	}

	//0. Manual Position, 1.Random, 2.Round Equidistance, 3.Spiral, 4.Shifting Spiral, 5.Shifting Radial
	inline void SetFormation(int index_) {
		formation = index_;

		switch (formation) {
		case 0: 
			break;
		case 1:
			break;
		case 2:
			angleGap = 360.0f / spawnInst;
			break;
		case 3: 
			angleGap = 360.0f / (spawnCycle * spawnInst);
			break;
		case 4:
			angleGap = 360.0f / ((spawnCycle * spawnInst) / cycleDiv);
			cycleCap = static_cast<int>(spawnInst * (spawnCycle / cycleDiv));
			if (positionOffset == 0.0f) positionOffset = 10.0f;
			break;
		case 5:
			angleGap = 360.0f / spawnInst;
			cycleCap = spawnInst;
			break;
		}
	}

	inline void SetPosition(float positionOffset_) {
		switch (formation) {
		case 0:
		case 1:
		case 2:
		case 3:
			positionOffset = positionOffset_;
			position += positionOffset;
			break;
		case 4:
			break;
		case 5:
			positionOffset = positionOffset_;
			position += positionOffset;
			break;
		}
	}

	inline int GetLevel() {
		if (enemyMinLv == 0)
			return enemyMaxLv;
		else
			return static_cast<int>(FRAND_RANGE(enemyMinLv, enemyMaxLv + 1));
	}

	inline float GetPosition() {
		float result;

		switch (formation) {
		case 0: //Static position (have to assign manually)
			break;
		case 1: //Random position
			position = FRAND_RANGE(0.0f, 360.0f);
			break;
		case 2: //Equidistance
			position += angleGap;
			break;
		case 3: //Spiral out
			position += angleGap;
			break;
		case 4: //Shifting spiral 
			position += angleGap;
			cycleCounter++;
			if (cycleCounter == cycleCap) {
				position += positionOffset;
				cycleCounter = 0;
			}
			break;
		case 5: //Shifting Radial
			position += angleGap;
			if (cycleCounter == cycleCap) {
				position += positionOffset;
				cycleCounter = 0;
			}
			cycleCounter++;
			break;
		default:
			result = position;
			break;
		}

		return position;
	}

private:
	int formation;
	float angleGap;
	float positionOffset;
	unsigned short int cycleCounter;
	unsigned short int cycleCap;
};

struct BossQuery
{
	int level;

	BossQuery() : level(1) {}
};

struct OrbitQuery 
{
	unsigned short int basicLv;
	unsigned short int basicAmount;
	unsigned short int obsLv;
	unsigned short int obsAmount;
	unsigned short int obsInc;
	unsigned short int turn;

	OrbitQuery() : basicLv(1), basicAmount(2), obsLv(1), obsAmount(2), obsInc(0), turn(1) {}
};

class StageScenario
{
public:
	inline StageScenario() {
		Queries = nullptr;
		cycleBuffer = 5000.0f;
		LvFunc = &StageScenario::Empty;

		bossLevel = 1;
		eSpiralChains = 16; //Max 60
		eSpiralLayers = 0; //Max 8
		pRaySections = 4; //Max 24
		pRayLayers = 0; //Max 8

		pSpiralPathSections = 2; //max 12
		pSpiralPathChains = 6; //max 24
		pSpiralPathSpeed = 35.0f; //max 80.0
		pSpiralPathDirection = false;
		eRaySections = 2;

		pSpiralPathSections2 = 8; //max 24
		pSpiralPathChains2 = 8; //max 24
		pSpiralPathSpeed2 = 80.0f; //max 200.0
		pSpiralPathDirection2 = true;

		eSpiralPathChains = 2; //min 6 max 30
		eSpiralPathLayers = 5; //min 5 max 10
		eSpiralChains2 = 0; // max 30
		eSpiralLayers2 = 1; // max 10
	}
	StageScenario(std::queue<SpawnQuery>& queries_);
	~StageScenario();

	//Non-Boss Stage set events
	void SpawnShooter(unsigned int level_, unsigned int inst_);
	void SpawnCore(unsigned int level_, unsigned int inst_);

	//Boss' stage events
	void LevelInit(unsigned int level_, unsigned int levelSelect_);
	void Level1();
	void Level2();
	void Level3();
	void Level4();

	//Obstacles patterns
	void PRay(unsigned int level_, unsigned int sections_, unsigned int length_, float position_);
	void PSpiral(unsigned int level_, unsigned int layer_);
	void PPie(unsigned int level_, unsigned int sections_, unsigned int layers_, unsigned int length_, float speed_);
	void PSpiralPath(unsigned int sections_, unsigned int length_, float speed_, bool clockwise_, float cycleTime_);
	
	//Spawn shields
	void SpawnBasicOrbit();
	void SpawnObsOrbit();

	//Shields modifiers
	void IncBOrbitLv(unsigned short int value_);
	void IncBOrbitAmount(unsigned short int value_);
	void SetOOrbitLv();

	//Spawn enemies
	void ERay(EEnemyType type_, unsigned short int level_, unsigned short int sections_, unsigned short int layers_, float positionOffset_);
	void ESpiral(EEnemyType type_, unsigned short int level_, unsigned short int length_, unsigned short int layers_);
	void ESpiralPath(EEnemyType type_, unsigned short int level_, unsigned short int sections_, unsigned short int layers_);

	inline void StageSpawn() {
		(*this.*LvFunc)();
	}

	//Runs when delegate in spawner needs to be off
	inline void Empty() {};

private:
	std::queue<SpawnQuery>* Queries;
	OrbitQuery orbitQuery;

	float cycleBuffer;
	unsigned int bossLevel;
	unsigned int eSpiralChains;
	unsigned int eSpiralLayers;
	unsigned int pRaySections;
	unsigned int pRayLayers;
	unsigned int pSpiralPathSections;
	unsigned int pSpiralPathChains;
	float pSpiralPathSpeed;
	bool pSpiralPathDirection;
	unsigned int eRaySections;
	unsigned int pSpiralPathSections2; 
	unsigned int pSpiralPathChains2; 
	float pSpiralPathSpeed2; 
	bool pSpiralPathDirection2;
	unsigned int eSpiralPathChains;
	unsigned int eSpiralPathLayers;
	unsigned int eSpiralChains2;
	unsigned int eSpiralLayers2;

	void (StageScenario::*LvFunc)();
	
	typedef void (StageScenario::*LevelFuncs)();

	LevelFuncs lvFuncs[4];

	//Utilities
	inline int RollDice(int min_, int max_) {
		return static_cast<int>(FRAND_RANGE(min_, max_ + 1.0f));
	}
};

