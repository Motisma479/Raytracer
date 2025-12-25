#include "Core/Settings.hpp"
#include <iostream>
AppSettings settings;

const AppSettings& Settings()
{
	return settings;
}

void ParseArgs(s32 argc, char* argv[])
{
	for (s32 i = 0; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "-h" || arg == "--help")
		{
			settings.showHelp = true;
		}

		if (arg == "-d" || arg == "--debug")
		{
			if (i + 1 == argc)
				throw std::runtime_error(arg + " : <int> needed. do -h/--help for more information.");
			s32 level = std::stoi(argv[i + 1]);
			if(level < 0 || level > 2)
				throw std::runtime_error(arg + " : invalid debug level. do -h/--help for more information.");

			settings.debugLevel = static_cast<DebugLevel>(level);
		}
	}
}