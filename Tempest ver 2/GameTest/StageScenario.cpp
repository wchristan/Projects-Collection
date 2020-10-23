#include "stdafx.h"
#include "StageScenario.h"


StageScenario::StageScenario(std::queue<SpawnQuery>& queries_) : StageScenario()
{
	Queries = &queries_;

	lvFuncs[0] = &StageScenario::Level1;
	lvFuncs[1] = &StageScenario::Level2;
	lvFuncs[2] = &StageScenario::Level3;
	lvFuncs[3] = &StageScenario::Level4;
}

StageScenario::~StageScenario()
{
	Queries = nullptr;
}

void StageScenario::SpawnShooter(unsigned int level_, unsigned int inst_)
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = EEnemyType::Shooter;
	query.enemyMaxLv = level_;
	query.enemySpeed = 50.0f;
	query.spawnInst = inst_;
	query.cycleDiv = 1;
	query.spawnCycle = 1;
	query.SetPosition(0.0f);
	query.SetFormation(2);
	query.cycleTime = 500.0f;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	Queries->push(query);
}

void StageScenario::SpawnCore(unsigned int level_, unsigned int inst_)
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = EEnemyType::Core;
	query.enemyMaxLv = level_;
	query.enemySpeed = 50.0f;
	query.spawnInst = inst_;
	query.cycleDiv = 1;
	query.spawnCycle = 1;
	query.SetPosition(0.0f);
	query.SetFormation(2);
	query.cycleTime = 500.0f;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	Queries->push(query);
}

void StageScenario::LevelInit(unsigned int level_, unsigned int levelSelect_)
{
	if (levelSelect_ > 4) return;

	bossLevel = level_;
	LvFunc = lvFuncs[levelSelect_ - 1];

	switch (levelSelect_) {
	case 1:
		if ((eSpiralChains += 4) > 60) eSpiralChains = 60; //Max 60
		if (RollDice(0, 1)) {
			if ((eSpiralLayers += 1) > 8) eSpiralLayers = 8; //Max 8
		}
		if ((pRaySections += 4) > 24) pRaySections = 24; //Max 24
		if (RollDice(0, 1)) {
			if ((pRayLayers += 1) > 8) pRayLayers = 8; //Max 8
		}
		break;
	case 2:
		if ((pSpiralPathSections += 1) > 12) pSpiralPathSections = 12; 
		if ((pSpiralPathChains += 2) > 24) pSpiralPathChains = 24; 
		if ((pSpiralPathSpeed += 5.0f) > 80.0f) pSpiralPathSpeed = 80.0f;
		pSpiralPathDirection = RollDice(0, 1);
		if ((eRaySections += 2) > 24) eRaySections = 24;
		break;
	case 3:
		if ((pSpiralPathSections2 += 2) > 24) pSpiralPathSections = 24;
		if ((pSpiralPathChains2 += 2) > 24) pSpiralPathChains = 24;
		if ((pSpiralPathSpeed2 += 20.0f) > 200.0f) pSpiralPathSpeed = 200.0f;
		break;
	case 4:
		if ((eSpiralPathChains += 4) > 30) eSpiralPathChains = 30;
		if ((eSpiralChains2 += 4) > 30) eSpiralChains2 = 30;
		if (bossLevel % 3 == 0) {
			if ((eSpiralPathLayers++) > 10) eSpiralPathLayers = 10;
			if ((eSpiralLayers2++) > 10) eSpiralPathLayers = 10;
		}
		break;
	}
}

void StageScenario::Level1()
{
	ESpiral(EEnemyType::Basic, bossLevel, eSpiralChains, eSpiralLayers);
	
	for (int i = pRayLayers; i > 0; i--) {
		PRay(1, pRaySections, 2, 0.0f);
		PRay(1, pRaySections, 2, 180.0f / (float)pRaySections);
	}
}

void StageScenario::Level2()
{
	if (bossLevel > 10) {
		pSpiralPathDirection = RollDice(0, 1);
	}

	//Slow spiral pathing
	PSpiralPath(pSpiralPathSections, pSpiralPathChains, pSpiralPathSpeed, pSpiralPathDirection, 400.0f);

	ERay(EEnemyType::Basic, bossLevel, eRaySections, 4, (float)eRaySections / 180.0f);
}

void StageScenario::Level3()
{
	if (bossLevel > 20) {
		pSpiralPathDirection2 = RollDice(0, 1);
	}

	PSpiralPath(pSpiralPathSections2, pSpiralPathChains2, pSpiralPathSpeed2, pSpiralPathDirection2, 500.0f);

	ESpiral(EEnemyType::Basic, bossLevel, eSpiralChains, eSpiralLayers);
}

void StageScenario::Level4()
{
	EEnemyType type;

	if (bossLevel > 20) {
		if (RollDice(0, 1)) type = EEnemyType::Basic;
		else type = EEnemyType::BasicRadial;
	}
	else {
		type = EEnemyType::Basic;
	}

	ESpiralPath(EEnemyType::Basic, bossLevel, eSpiralPathChains, eSpiralPathLayers);

	if (bossLevel > 10) {
		if (RollDice(0, 1)) type = EEnemyType::Seeker;
		else type = EEnemyType::BasicRadial;
	}
	else {
		type = EEnemyType::BasicRadial;
	}

	ESpiral(type, bossLevel, eSpiralChains2, eSpiralLayers2);
}

void StageScenario::PRay(unsigned int level_, unsigned int sections_, unsigned int length_, float position_)
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = EEnemyType::Obstacle;
	query.enemyMaxLv = level_;
	query.enemySpeed = 50.0f;
	query.spawnInst = sections_;
	query.spawnCycle = length_;
	query.SetFormation(2);
	query.SetPosition(position_);
	query.cycleTime = 1500.0f;
	query.completeTime = query.cycleTime * query.spawnCycle; //+cycleBuffer

	Queries->push(query);
}

void StageScenario::PSpiral(unsigned int level_, unsigned int layer_)
{
	if (Queries == nullptr) return;
	if (!layer_) layer_ = 4;

	SpawnQuery query;
	query.enemyType = EEnemyType::Obstacle;
	query.enemyMaxLv = level_;
	query.enemySpeed = 40.0f;
	query.spawnInst = 1;
	query.cycleDiv = layer_;
	query.spawnCycle = 16 * query.cycleDiv;
	query.SetPosition(10.0f);
	query.SetFormation(4);
	query.cycleTime = 120.0f;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	Queries->push(query);
}

void StageScenario::PPie(unsigned int level_, unsigned int sections_, unsigned int layers_, unsigned int length_, float speed_)
{
	if (Queries == nullptr) return;

	int sections; 
	if ((sections = sections_) == 0) {
		sections = (int)FRAND_RANGE(2.0f, 8.0f);
	}

	SpawnQuery query;
	query.enemyType = EEnemyType::Obstacle;
	query.enemyMaxLv = level_;
	query.enemySpeed = speed_;
	query.spawnInst = sections;
	query.cycleDiv = 1;
	query.spawnCycle = length_; 
	query.SetPosition(10.0f);
	query.SetFormation(5);
	query.cycleTime = 500.0f + ((float)sections * 20.0f); //Adjust for evade path
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	for (int i = layers_; i > 0; i--) {
		Queries->push(query);
	}
}

void StageScenario::PSpiralPath(unsigned int sections_, unsigned int  length_, float speed_, bool clockwise_, float cycleTime_)
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = EEnemyType::Obstacle;
	query.enemyMaxLv = 1;
	query.enemySpeed = speed_;
	query.spawnInst = sections_;
	query.cycleDiv = 1;
	query.spawnCycle = length_;
	(clockwise_)? query.SetPosition(10.0f) : query.SetPosition(-10.0f);
	query.SetFormation(5);
	query.cycleTime = cycleTime_;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	Queries->push(query);
}

void StageScenario::SpawnBasicOrbit()
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = EEnemyType::BasicSatellite;
	query.enemyMaxLv = orbitQuery.basicLv;

	if (orbitQuery.basicLv <= 8 && orbitQuery.basicAmount > 10) {
		query.spawnInst = 10;
	}
	else if (orbitQuery.basicLv <= 16 && orbitQuery.basicAmount > 20) {
		query.spawnInst = 20;
	}
	else if (orbitQuery.basicAmount > 40) {
		query.spawnInst = 40;
	}
	else {
		query.spawnInst = orbitQuery.basicAmount;
	}

	query.spawnCycle = 1;
	query.SetFormation(2);
	query.cycleTime = 1000.0f;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	Queries->push(query);
}

void StageScenario::SpawnObsOrbit()
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = EEnemyType::ObsSatellite;
	query.enemyMaxLv = orbitQuery.obsLv; //Max 3
	query.spawnInst = orbitQuery.obsAmount; //lv 1 = max 20, 2 = 16, 3 = 14
	query.spawnCycle = 1;
	query.SetFormation(2);
	query.cycleTime = 1000.0f;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	Queries->push(query);
}

void StageScenario::IncBOrbitLv(unsigned short int value_)
{
	orbitQuery.basicLv += value_;
}

void StageScenario::IncBOrbitAmount(unsigned short int value_)
{
	orbitQuery.basicAmount += value_;
}

void StageScenario::SetOOrbitLv()
{
#define INC orbitQuery.obsInc
	
	switch (orbitQuery.turn) {
	case 1:
		orbitQuery.obsLv = 2;
		orbitQuery.obsAmount = 8 + (INC * 4); //max 16
		break;
	case 2:
		orbitQuery.obsLv = 10;
		orbitQuery.obsAmount = 2 + INC; //max 4
		break;
	case 3:
		orbitQuery.obsLv = 5;
		orbitQuery.obsAmount = 4 + (INC * 2); //max 8
		break;
	default:
		orbitQuery.obsLv = 4;
		orbitQuery.obsAmount = 10;
		break;
	}

	orbitQuery.turn++;
	if (orbitQuery.turn > 3) {
		orbitQuery.turn = 1;
		
		if (INC++ > 2) INC = 2;
	}
#undef INC
}

void StageScenario::ERay(EEnemyType type_, unsigned short int level_, unsigned short int sections_, unsigned short int layers_, float positionOffset_)
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = type_;
	//query.enemyMinLv = 1;
	query.enemyMaxLv = level_;
	query.enemySpeed = 100.0f;
	query.spawnInst = sections_;
	query.spawnCycle = layers_;
	//query.cycleDiv = 1;
	query.SetFormation(2);
	query.SetPosition(positionOffset_);
	query.cycleTime = 2000.0f;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	Queries->push(query);
}

void StageScenario::ESpiral(EEnemyType type_, unsigned short int level_, unsigned short int length_, unsigned short int layers_)
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = type_;
	//query.enemyMinLv = 1;
	query.enemyMaxLv = level_;
	query.enemySpeed = 150.0f;
	query.spawnInst = 1;
	query.spawnCycle = length_; //Basic max = 30
	//query.cycleDiv = 1;
	query.SetFormation(3);
	//query.SetPosition(10.0f);
	query.cycleTime = 100.0f;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	for (int i = layers_; i > 0; i--) {
		Queries->push(query);
	}
}

void StageScenario::ESpiralPath(EEnemyType type_, unsigned short int level_, unsigned short int sections_, unsigned short int layers_)
{
	if (Queries == nullptr) return;

	SpawnQuery query;
	query.enemyType = type_;
	//query.enemyMinLv = 1;
	query.enemyMaxLv = level_;
	query.enemySpeed = 100.0f;
	query.spawnInst = sections_;
	query.spawnCycle = layers_;
	//query.cycleDiv = 1;
	query.SetFormation(5);
	query.SetPosition(10.0f);
	query.cycleTime = 600.0f;
	query.completeTime = query.cycleTime * query.spawnCycle + cycleBuffer;

	Queries->push(query);
}

