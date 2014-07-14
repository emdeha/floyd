#include "Reporting.h"

#include <fstream>
#include <iostream>
#include <sstream>


void Logger::Log(const std::string &message, Priority prio)
{
	std::ofstream logFile("log", std::ios::app);

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
		Report::UnexpectedError("Cannot create logger file", __LINE__, __FILE__);
	}

	logFile.close();
}


void Report::UnexpectedError(const std::string &message, int line, const std::string &file)
{
	std::stringstream errorText;
	errorText << "Unexpected Error: \n"
			  << "\"" << message << "\"\n at"
			  << line << " in " << file << '\n';
	DoReport(errorText.str(), HIGH);
}

void Report::Error(const std::string &message, int line, const std::string &file)
{
	std::stringstream errorText;
	errorText << "Error: \n"
			  << "\"" << message << "\"\n at"
			  << line << " in " << file << '\n';
	DoReport(errorText.str(), HIGH);
}

void Report::Warning(const std::string &message, int line, const std::string &file)
{
	std::stringstream warningText;
	warningText << "Warning: \n"
				<< "\"" << message << "\"\n at"
				<< line << " in " << file << '\n';
	DoReport(warningText.str(), MEDIUM);
}

void Report::DoReport(const std::string &message, Priority prio)
{
	Logger::Log(message, HIGH);

#ifdef _DEBUG
	std::cerr << message;
#endif

	if (prio == HIGH)
	{
		_CrtDbgBreak();
	}
}