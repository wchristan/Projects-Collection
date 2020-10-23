#pragma once

struct TimerNode{
	bool isReady = false;
	bool loopTimer = false;
	float timeSet = 0.0f;
	float timeElapsed = 0.0f;
};

class Timer
{
private:
#define MAXNODES 4

public:
	TimerNode* nodes[MAXNODES];

	Timer();
	~Timer();

	//Needs to be called every update
	void Update(float deltaTime_);

	//Index is between 0 - 3
	void SetTimer(float time_, int index_, bool loopTimer_);
	void SetTimer(float time_, int index_);

	//Set the timer's SetTime
	void SetTime(int index_, float newTime);

	//Reset the node's elapsed time and its boolean check
	void ResetTimer(int index_);

	//Clear and delete the timer at index
	void ClearTimer(int index_);

	//Return true/false if time set has been reached
	bool GetTimeCheck(int index_);
	
	float GetTimeElapsed(int index_);

	float GetTimeSet(int index_);

	void ChangeTimer(int index_, bool loopTimer);

};

