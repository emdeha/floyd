#include "stdafx.h"
#include "Reporting.h"

#include "Dirs.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>


void Logger::Init()
{
	std::ofstream logFile(FILE_LOG, std::ios::trunc);

	logFile.close();
}

void Logger::Log(const std::string &message, Priority prio)
{
	std::ofstream logFile(FILE_LOG, std::ios::app);

	if (logFile.is_open())
	{
		switch (prio)
		{
		case LOW:
			logFile << "LOW: ";
			break;
		case MEDIUM:
			logFile << "--MEDIUM: ";
			break;
		case HIGH:
			logFile << "(!)HIGH: ";
			break;
		}

		logFile << message << '\n';
	}
	else
	{
		assert("Cannot open log file!");
	}

	logFile.close();
}


void Report::UnexpectedError(const std::string &message, int line, const std::string &file)
{
	std::stringstream errorText;
	errorText << "Unexpected Error: \n"
			  << "\"" << message << "\"\n at line "
			  << line << " in " << file << '\n';
	DoReport(errorText.str(), HIGH);
}

void Report::Error(const std::string &message, int line, const std::string &file)
{
	std::stringstream errorText;
	errorText << "Error: \n"
			  << "\"" << message << "\"\n at line "
			  << line << " in " << file << '\n';
	DoReport(errorText.str(), HIGH);
}

void Report::Warning(const std::string &message, int line, const std::string &file)
{
	std::stringstream warningText;
	warningText << "Warning: \n"
				<< "\"" << message << "\"\n at line "
				<< line << " in " << file << '\n';
	DoReport(warningText.str(), MEDIUM);
}

void Report::DoReport(const std::string &message, Priority prio)
{
	Logger::Log(message, prio);

#ifdef _DEBUG
	std::cerr << message;
#endif

	if (prio == HIGH)
	{
		_CrtDbgBreak();
	}
}