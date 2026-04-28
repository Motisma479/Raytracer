#pragma once

#ifdef _WIN32
	#define NOMINMAX
	#include <Windows.h>
#endif 

#include <LibMath/Maths.hpp>
#include <string>
#include <vector>

#include "Core/Type.hpp"//used by every project file

#define LOG_USE_CLOG
#include "Core/Logger.hpp"
#include "Core/LogLevels.hpp"