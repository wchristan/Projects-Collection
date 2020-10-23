#include "stdafx.h"
#include "Timer.h"

Timer::Timer() 
{
	//Important to assign nullptr since without it the variable
	//would be uninitialize (in debug the memory is filled with 0xCC)
	//Can't check if variable is uninitialized, it'll be compile error most likely
	for (int i = 0; i < MAXNODES; i++) {
		nodes[i] = nullptr;
	}

	nodes[0] = new TimerNode();
}

Timer::~Timer()
{
	for (TimerNode* &node : nodes) {
		if (node != nullptr) {
			delete node;
			node = nullptr;
		}
	}
}

void Timer::Update(float deltaTime_)
{
	for (int i = 0; i < MAXNODES; i++) {
		if (nodes[i] == nullptr) continue;
		if (nodes[i]->timeSet <= 0.0f) continue;
		
		//Update each node's timer ticks
		nodes[i]->timeElapsed += deltaTime_;

		if (nodes[i]->timeElapsed >= nodes[i]->timeSet) {
			nodes[i]->isReady = true;
		}
		else {
			nodes[i]->isReady = false;
		}
	}
}

void Timer::SetTimer(float time_, int index_, bool loopTimer_)
{
	if (nodes[index_] != nullptr) {
		nodes[index_]->timeSet = time_;
		nodes[index_]->timeElapsed = time_;
		nodes[index_]->loopTimer = loopTimer_;
	}
	else {
		nodes[index_] = new TimerNode();
		nodes[index_]->timeSet = time_;
		nodes[index_]->timeElapsed = time_;
		nodes[index_]->loopTimer = loopTimer_;
	}
}

void Timer::SetTimer(float time_, int index_)
{
	if (nodes[index_] != nullptr) {
		nodes[index_]->timeSet = time_;
		nodes[index_]->timeElapsed = time_;
		nodes[index_]->loopTimer = false;
	}
	else {
		nodes[index_] = new TimerNode();
		nodes[index_]->timeSet = time_;
		nodes[index_]->timeElapsed = time_;
		nodes[index_]->loopTimer = false;
	}
}

void Timer::SetTime(int index_, float newTime_)
{
	if (nodes[index_] != nullptr) {
		bool tmp = nodes[index_]->loopTimer;
		SetTimer(newTime_, index_, tmp);
	}
}

void Timer::ResetTimer(int index_)
{
	nodes[index_]->isReady = false;
	nodes[index_]->timeElapsed = 0.0f;
}

void Timer::ClearTimer(int index_)
{
	delete nodes[index_];
	nodes[index_] = nullptr;
}

bool Timer::GetTimeCheck(int index_)
{
	if (nodes[index_] == nullptr) return false;

	if (nodes[index_]->loopTimer && nodes[index_]->isReady) {
		nodes[index_]->timeElapsed = 0.0f;
	}

	return nodes[index_]->isReady;
}

float Timer::GetTimeElapsed(int index_)
{
	nodes[index_];

	if (nodes[index_] != nullptr) {
		return nodes[index_]->timeElapsed;
	}
	return -1.0f;
}

float Timer::GetTimeSet(int index_)
{
	if (nodes[index_] != nullptr) {
		return nodes[index_]->timeSet;
	}
	return -1.0f;
}

void Timer::ChangeTimer(int index_, bool loopTimer)
{
}
