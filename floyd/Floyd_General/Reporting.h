#ifndef FLOYD_REPORTING_H
#define FLOYD_REPORTING_H


#include <string>

enum Priority
{
	LOW,
	MEDIUM,
	HIGH,
};

class Logger
{
public:
	///
	/// @brief Clears the contents of the previous logs.
	///
	static void Init();
	///
	/// @brief Logs a message with its priority.
	///
	static void Log(const std::string &message, Priority prio);
};

class Report
{
public:
	static void UnexpectedError(const std::string &message, int line, const std::string &file);
	static void Error(const std::string &message, int line, const std::string &file);
	static void Warning(const std::string &message, int line, const std::string &file); 

private:
	///
	/// @brief Logs the error.
	///
	static void DoReport(const std::string &message, Priority prio);
};


#endif