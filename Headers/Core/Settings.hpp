#pragma once
#include "Type.hpp"
#include <string>
#include <vector>

struct Option
{
	std::string shortVerion;
	std::string longVersion;
	std::string valueType;  //optional if not used put ""
	std::string description;
};


enum class DebugLevel
{
	NO_DEBUG,
	LITTLE_DEBUG,
	FULL_DEBUG
};

struct AppSettings
{
	bool showHelp = false;
	DebugLevel debugLevel = DebugLevel::LITTLE_DEBUG;
};

const AppSettings& Settings();

void ParseArgs(s32 argc, char* argv[]);