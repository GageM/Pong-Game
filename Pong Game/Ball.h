#pragma once

#include "SerializableObject.h"
#include "Renderable.h"
#include "Vector.h"
#include "Color.h"
#include "Collider.h"

class Ball : SerializableObject, Renderable, Collider
{
public:
	Ball(const char* name_, int screenWidth, int screenHeight);

	std::string Serialize() override;
	void DeSerialize(const rapidjson::Document& doc) override;
	void Print(const char* msg = nullptr) override;
	void Render(SDL_Renderer* renderer) const override;
	void Update() override;
	bool IsColliding(const Vector2& otherPos, const Vector2 otherDimensions) override;

	int IsOOB();

	void Reset();

	inline Vector2 GetPosition() const { return position; }
	inline void SetPosition(const Vector2& newPos) { position = newPos; }

	inline Vector2 GetVelocity() const { return velocity; }
	inline void SetVelocity(const Vector2& newVel) { velocity = newVel; }

	inline float GetRadius() const { return radius; }

private:
	Vector2 position;
	Vector2 velocity;
	float radius;
	Color color;

	Vector2 screenSize;
};

