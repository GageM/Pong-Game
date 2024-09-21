#pragma once

#include "Vector.h"

class Collider
{
	virtual bool IsColliding(const Vector2& otherPos, const Vector2 otherDimensions) = 0;
};