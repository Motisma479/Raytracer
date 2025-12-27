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

		if (arg == "-r" || arg == "--ratio")
		{
			if (i + 1 == argc)
				throw std::runtime_error(arg + " : <float> needed. do -h/--help for more information.");
		

			std::string ip1 = argv[i + 1];
			s32 pos = ip1.find('/');
			if (pos != std::string::npos)
			{
				f32 a = std::stof(ip1.substr(0, pos));
				f32 b = std::stof(ip1.substr(pos+1));
				settings.aspectRatio = a/b;
			}
			else
			{
				settings.aspectRatio = std::stof(ip1);
			}

		}

		if (arg == "-w" || arg == "--width")
		{
			if (i + 1 == argc)
				throw std::runtime_error(arg + " : <int> needed. do -h/--help for more information.");

			settings.imageWidth = std::stoi(argv[i + 1]);
			if (settings.imageWidth < 1)
				throw std::runtime_error(arg + " : invalid width. do -h/--help for more information.");
		}
	}
}