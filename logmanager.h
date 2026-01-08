// COMP710 GP Framework 2025
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <vector>
#include <stdlib.h>
#include <string>

class LogManager
{
	// Member methods:
public:
	static LogManager& GetInstance();
	static void DestroyInstance();

	void Log(const char* pcMessage);

	void DebugDraw();
protected:

private:
	LogManager();
	~LogManager();
	LogManager(const LogManager& logManager);
	LogManager& operator=(const LogManager& logManager);

	// Member data:
public:

protected:
	static LogManager* sm_pInstance; // Singleton Pattern: Only one LogManager object can ever be created

	std::vector<std::string> m_logHistory;
	bool m_bShowLogMessages;

private:
};

#endif // LOGMANAGER_H