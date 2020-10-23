#include "stdafx.h"
#include "Custom.h"

#include "App/app.h"

Block Custom::debugBlocks[MAXBLOCKS];
Vec2 Custom::ScreenCenter = Vec2(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2);
Vec2 Custom::ScreenMax = Vec2(APP_VIRTUAL_WIDTH, APP_VIRTUAL_HEIGHT);

//Colour Library
Vec3 Custom::Black = Vec3();
Vec3 Custom::White = Vec3(1.0f, 1.0f, 1.0f);
Vec3 Custom::Grey = Vec3(0.5f, 0.5f, 0.5f);
Vec3 Custom::Red = Vec3(1.0f, 0.0f, 0.0f);
Vec3 Custom::Green = Vec3(0.0f, 1.0f, 0.0f);
Vec3 Custom::Blue = Vec3(0.0f, 0.0f, 1.0f);
Vec3 Custom::Yellow = Vec3(1.0f, 1.0f, 0.0f);
Vec3 Custom::Violet = Vec3(0.7f, 0.0f, 1.0f);
Vec3 Custom::Purple = Vec3(0.3f, 0.0f, 7.0f);
Vec3 Custom::Cyan = Vec3(0.0f, 1.0f, 1.0f);
Vec3 Custom::Pink = Vec3(1.0f, 0.0f, 7.0f);
Vec3 Custom::Orange = Vec3(1.0f, 0.5f, 0.0f);


Custom::Custom()
{

}

Custom::~Custom()
{
	
}

void Custom::Render()
{
	for (int i = 0; i < MAXBLOCKS; i++) {
		if (debugBlocks[i].isActive) {
			
			if (!debugBlocks[i].isVector) {
				App::Debug(i, debugBlocks[i].message.c_str(), debugBlocks[i].value);
			}
			else {
				App::Debug(i, debugBlocks[i].message.c_str(), debugBlocks[i].value, debugBlocks[i].value2);
			}
		}
	}
}

void Custom::DrawPoly(float rad_, Vec2& pos_, float rot_, int sides_, Vec3& colour_)
{
	float radius = rad_;
	Vec2 position = pos_;
	float rotation = rot_ * RAD;
	int sides = sides_;

	//Find the angle deviation for each segments/sides of the poly
	float angle = (DOUBLE_PI) / (float)sides;

	float r = colour_.x;
	float g = colour_.y;
	float b = colour_.z;

	//Loop to draw an enclosed poly, anything from square to circle
	for (int i = 0; i < sides; i++)
	{
		float sx = position.x + sinf(rotation + angle * i) * radius;
		float sy = position.y + cosf(rotation + angle * i) * radius;
		float ex = position.x + sinf(rotation + angle * (i + 1)) * radius;
		float ey = position.y + cosf(rotation + angle * (i + 1)) * radius;

		App::DrawLine(sx, sy, ex, ey, r, g, b);
	}

}

void Custom::DrawLine(Vec2& startPos_, Vec2& finalPos_, Vec3& colour_)
{
	App::DrawLine(startPos_.x, startPos_.y, finalPos_.x, finalPos_.y, colour_.x, colour_.y, colour_.z);
}

void Custom::DrawTriangle(float radius_, Vec2& pos_, float rot_)
{
	DrawPoly(radius_, pos_, rot_, 3, Vec3(1.0f, 1.0f, 1.0f));
}

void Custom::DrawSquare(float radius_, Vec2& pos_, float rot_)
{
	DrawPoly(radius_, pos_, rot_, 4, Vec3(1.0f, 1.0f, 1.0f));
}

void Custom::DrawSquare(float radius_, Vec2& pos_, float rot_, Vec3& colour_)
{
	DrawPoly(radius_, pos_, rot_, 4, colour_);
}

Vec3 Custom::RandomColour(Vec3& colour_) 
{
	float x = FRAND_RANGE(0.0f, 1.0f);
	float y = FRAND_RANGE(0.0f, 1.0f);
	float z = FRAND_RANGE(0.0f, 1.0f);

	colour_ = Vec3(x, y, z);

	return Vec3(x, y, z);
}

Vec3 Custom::AnimColour(Vec3 &start_, Vec3& end_)
{
	if (start_.x >= start_.x) return end_;

	start_ += end_ / 100.0f;

	return start_;
}

void Custom::Debug(int index_, std::string message_, float value_)
{
	if (index_ < 0 || index_ > 30) return;

	debugBlocks[index_].message = message_;
	debugBlocks[index_].value = value_;
	debugBlocks[index_].isActive = true;
}

void Custom::Debug(int index_, float value_)
{
	Debug(index_, "Debug " + std::to_string(index_), value_);
}

void Custom::Debug(int index_, const char* message_)
{
	Debug(index_, message_, 0.0f);
}

void Custom::Debug(int index_, std::string& message_, Vec2& vec2_)
{
	if (index_ < 0 || index_ > 30) return;

	debugBlocks[index_].message = message_;
	debugBlocks[index_].value = vec2_.x;
	debugBlocks[index_].value2 = vec2_.y;
	debugBlocks[index_].isActive = true;
	debugBlocks[index_].isVector = true;
}

void Custom::Debug(int index_, Vec2& vec2_)
{
	Debug(index_, "Debug " + std::to_string(index_), vec2_);
}

