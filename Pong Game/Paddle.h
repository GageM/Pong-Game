#pragma once

#include "SerializableObject.h"
#include "Renderable.h"
#include "Vector.h"
#include "Color.h"
#include "NetworkRole.h"
#include "Collider.h"

class Paddle : SerializableObject, Renderable, Collider
{
public:
	Paddle(const char* name_, NetworkRole role, int screenWidth, int screenHeight);

	std::string Serialize() override;
	void DeSerialize(const rapidjson::Document& doc) override;
	void Print(const char* msg = nullptr) override;
	void Render(SDL_Renderer* renderer) const override;
	void Update() override;
	bool IsColliding(const Vector2& otherPos, const Vector2 otherDimensions) override;

	inline Vector2 GetPosition() const { return position; }
	inline void SetPosition(const Vector2& newPos) { position = newPos; }

	inline Vector2 GetDimensions() const { return dimensions; }

	inline float GetMoveSpeed() const { return moveSpeed; }

private:
	Vector2 position;
	Vector2 dimensions;
	Color color;
	float moveSpeed;

	Vector2 screenSize;
};

