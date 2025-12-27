#pragma once
#include "Type.hpp"
#include <string>
#include <vector>

// 16.f / 9.f; //common PC screen ratio
// 9.f / 16.f; //vertical smartphone screen ratio
// 3.f / 2.f;  //photo ratio
// 1.f;        //social media ratio


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
	f32 aspectRatio = 16.f / 9.f;
	s32 imageWidth = 1920;
};

const AppSettings& Settings();

void ParseArgs(s32 argc, char* argv[]);