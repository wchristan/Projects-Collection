#pragma once

#include "Vector.h"


struct IGetter {
	virtual Vec2 GetPosition() = 0;
	virtual float GetRadius() = 0;

};