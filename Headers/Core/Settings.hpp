#pragma once
#include <chrono>

// 16.f / 9.f; //common PC screen ratio
// 9.f / 16.f; //vertical smartphone screen ratio
// 3.f / 2.f;  //photo ratio
// 1.f;        //social media ratio


struct AppSettings
{
	bool showHelp = false;
	DebugVerbosity debugVerbosity = DebugVerbosity::LITTLE_DEBUG;
	f32 aspectRatio = 16.f / 9.f;
	s32 imageWidth = 800;
	u64 seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
};

const AppSettings& Settings();

void ParseArgs(s32 argc, char* argv[]);