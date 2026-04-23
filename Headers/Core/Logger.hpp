#pragma once
#include <cstdint>
#include <string>
#include <streambuf>
#include <filesystem>
#include <fstream>
#include <unordered_map>

#ifndef LOG_FILE_LOCATION
#define LOG_FILE_LOCATION "Generated/Logs/"
#endif
//if no file is wanted use #define NO_FILE_GENERATION
//to log in the terminal use #define LOG_USE_CLOG
#ifndef SHOW_TO_LEVEL
#define SHOW_TO_LEVEL 0 //this will show only up level n set in the macro by default
#endif

enum class Severity : uint8_t
{
	NORMAL,
	WARNING,
	ERROR
};
inline std::ostream& operator<<(std::ostream& os, Severity s)
{
	switch (s)
	{
	case Severity::NORMAL:  return os << "NORMAL";
	case Severity::WARNING: return os << "WARNING";
	case Severity::ERROR:   return os << "ERROR";
	}
	return os << "UNKNOWN";
}

struct LogData
{
	std::chrono::system_clock::time_point time;
	std::string timeAsString;
	Severity severity;
	std::string category;
	std::string message;
	unsigned int level;
};

//all log will be saved in a file like HHMM_ddmm.log
class Logger
{
public:


	static Logger& Get()
	{
		static Logger instance;
		return instance;
	}

	//add a string to a list of category; it can be used for reflection system
	inline void RegisterCategory(const std::string& _str)
	{
		category.push_back(_str);
	}
	inline void RegisterCategory(const char* _cstr)
	{
		RegisterCategory(std::string(_cstr));
	}

	//Add a log to the list of logs
	inline void Log(Severity _severity, const std::string& _category, const std::string& _str, unsigned int _level)
	{
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

		std::time_t t = std::chrono::system_clock::to_time_t(now);

		std::tm tm{};
#ifdef _WIN32
		localtime_s(&tm, &t);
#else
		localtime_r(&t, &tm);
#endif

		std::string message = _str;
		if (message.find('\n') == std::string::npos)
		{
			message.push_back('\n');
		}

		LogData logData = {
			now,
			std::format("{:02}:{:02}:{:02}",
			tm.tm_hour,
			tm.tm_min,
			tm.tm_sec),
			_severity,
			_category,
			message,
			_level
		};

		data.push_back(logData);

#ifndef NO_FILE_GENERATION
		outputFile << "[" << logData.timeAsString << "][" << logData.category << "][" << logData.severity << "]: " << logData.message;
#endif
#ifdef LOG_USE_CLOG
		if (logData.level > showToLevel) return;
		std::clog << (logData.severity == Severity::WARNING ? "\033[38;2;229;194;0m" : logData.severity == Severity::ERROR ? "\033[38;2;237;38;36m" : "")
			<< "[" << logData.timeAsString << "][" << logData.category << "][" << logData.severity << "]: " << logData.message
			<< "\033[0m";
#endif
	}
	inline void Log(Severity _severity, const char* _category, const char* _cstr, unsigned int _level)
	{
		Log(_severity, std::string(_category), std::string(_cstr), _level);
	}

	inline const std::vector<LogData>& GetLogs() const
	{
		return data;
	}

	inline void SetShowToLevel(unsigned int _newLevel)
	{
		showToLevel = _newLevel;
	}

private:
	Logger()
	{
#ifndef NO_FILE_GENERATION
		std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		std::tm tm{};

#ifdef _WIN32
		localtime_s(&tm, &t);
#else
		localtime_r(&t, &tm);
#endif


		std::filesystem::path outputFileDir = LOG_FILE_LOCATION;
		outputFileDir /= std::format("{:02}{:02}_{:02}{:02}.log",
			tm.tm_hour,
			tm.tm_min,
			tm.tm_mday,
			tm.tm_mon + 1);

		std::filesystem::create_directories(outputFileDir.parent_path());
		outputFile.open(outputFileDir);
#endif

		showToLevel = SHOW_TO_LEVEL;
	}

	~Logger()
	{
#ifndef NO_FILE_GENERATION
		outputFile.close();
#endif
	}

	std::vector<LogData> data;
	std::vector<std::string> category;

	unsigned int showToLevel;

#ifndef NO_FILE_GENERATION
	std::ofstream outputFile;
#endif
};

#define MESSAGE_LOG(message) Logger::Get().Log(Severity::NORMAL, "Engine", message, 0)
#define MESSAGE_LOG_EX(message, category, level) Logger::Get().Log(Severity::NORMAL, category, message, (unsigned int)level)

#define WARNING_LOG(message) Logger::Get().Log(Severity::WARNING, "Engine", message, 0)
#define WARNING_LOG_EX(message, category, level) Logger::Get().Log(Severity::WARNING, category, message, (unsigned int)level)

#define ERROR_LOG(message) Logger::Get().Log(Severity::ERROR, "Engine", message, 0)
#define ERROR_LOG_EX(message, category, level) Logger::Get().Log(Severity::ERROR, category, message, (unsigned int)level)

#define SET_SHOW_TO_LEVEL(newLevel) Logger::Get().SetShowToLevel((unsigned int)newLevel)