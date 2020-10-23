#include "stdafx.h"
#include "Background.h"
#include "Custom.h"
#include "App/AppSettings.h"
#include "Math.h"


Background::Background()
{
	screenCenter = Vec2(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2);
	colour = Vec3(0.1f, 0.1f, 0.5f);
	ringsNum = 17;

	lineNum = 8;
	lineAngle = 0.0f;
	lineRotVel = 1.0f;
	lineMin = 100.0f;
	lineMax = 600.0f;
	lineStartPos = screenCenter;
	lineFinalPos = Vec2();
}

Background::~Background()
{
}

void Background::Update(float deltaTime_)
{
	//Animate the lines moving
	lineRotVel += deltaTime_ / 20000.0f;
}

void Background::Render()
{
	DrawBackground();
}

void Background::DrawBackground()
{
	//Draw circles with increasing diameter
	for (int i = 5; i < ringsNum; i++) {
		Custom::DrawPoly(powf(1.5f, (float)i), screenCenter, 0, 4 * i, colour);
	}

	lineAngle = (2.0f * PI / (float)lineNum);

	//Drawing lines from the center and outward to form tunnel rays
	for (int i = 0; i < lineNum; i++) {

		lineStartPos.x = screenCenter.x + sinf(lineAngle * (float)i + lineRotVel) * lineMin;
		lineStartPos.y = screenCenter.y + cosf(lineAngle * (float)i + lineRotVel) * lineMin;

		lineFinalPos.x = screenCenter.x + sinf(lineAngle * (float)i + lineRotVel) * lineMax;
		lineFinalPos.y = screenCenter.y + cosf(lineAngle * (float)i + lineRotVel) * lineMax;

		Custom::DrawLine(lineStartPos, lineFinalPos, colour);
	}

}

