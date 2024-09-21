#pragma once

#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class SerializableObject
{
public:
	SerializableObject(const char* name) : name(name) {}

	virtual std::string Serialize() = 0;
	virtual void DeSerialize(const rapidjson::Document& doc) = 0;
	virtual void Print(const char* msg = nullptr) = 0;

	const char* name;
};