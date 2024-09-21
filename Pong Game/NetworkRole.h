#pragma once

enum class NetworkRole
{
	NONE = 0,
	server = 1,
	client = 2
};

enum class NetworkType
{
	NONE = 0,
	TCP,
	UDP
};