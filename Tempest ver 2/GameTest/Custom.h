#pragma once

#include "Vector.h"
#include <string>


enum class EBuffType : unsigned short int {
	Attack = 0, Rebound, FireRate, BulletSpeed, BulletLife, Turret, Health
};

struct Block{
	bool isActive;
	bool isVector;
	std::string message;
	float value;
	float value2;

	Block() {
		isActive = false;
		isVector = false;
		message = "";
		value = 0.0f;
		value2 = 0.0f;
	}
};

//Class to help with Drawing lines into polygons
class Custom
{
public:
	static Vec2 ScreenCenter;
	static Vec2 ScreenMax;

	static Vec3 Black;
	static Vec3 White;
	static Vec3 Grey;
	static Vec3 Red;
	static Vec3 Green;
	static Vec3 Blue;
	static Vec3 Yellow;
	static Vec3 Violet;
	static Vec3 Purple;
	static Vec3 Cyan;
	static Vec3 Pink;
	static Vec3 Orange;

	Custom();
	~Custom();

	static void Render();

	//Draw various types of polygons from App::DrawLine
	static void DrawPoly(float radius_, Vec2& pos_, float rot_, int sides_, Vec3& colour_);
	static void DrawLine(Vec2& startPos_, Vec2& finalPos_, Vec3& colour_);
	static void DrawTriangle(float radius_, Vec2& pos_, float rot_);
	static void DrawSquare(float radius_, Vec2& pos_, float rot_);
	static void DrawSquare(float radius_, Vec2& pos_, float rot_, Vec3& colour_);

	static Vec3 RandomColour(Vec3 &colour_);
	static Vec3 AnimColour(Vec3 &start_, Vec3& end_);
	
	static void Debug(int index_, std::string message_, float value_);
	static void Debug(int index_, float value_);
	static void Debug(int index_, const char* message_);
	static void Debug(int index_, std::string& message_, Vec2& vec2_);
	static void Debug(int index_, Vec2& vec2_);

private:
#define MAXBLOCKS 30

	static Block debugBlocks[MAXBLOCKS];

};

