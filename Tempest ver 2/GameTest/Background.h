#pragma once

#include "Vector.h"

class Background
{
public:
	Vec2 screenCenter;	//Center of the screen
	Vec3 colour;		//Colour of BG
	int ringsNum;		//Number of circles to show on BG

	//Variables that affects the appearance and animation of lines on BG
	int lineNum;		
	float lineAngle;	
	float lineRotVel;
	float lineMin;
	float lineMax;
	Vec2 lineStartPos;
	Vec2 lineFinalPos;

	Background();
	~Background();

	void Update(float deltaTime_);
	void Render();

	void DrawBackground();

};

