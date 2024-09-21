#pragma once

#include "SerializableObject.h"

class GameState : SerializableObject
{
public:
	GameState(const char* name_, int scoreToWin_);

	std::string Serialize() override;
	void DeSerialize(const rapidjson::Document& doc) override;
	void Print(const char* msg = nullptr) override;

	int hostScore, clientScore, scoreToWin;
};

