#pragma once
#include "../Framework/AppWindow/AppWindow.h"
#include "../Framework/Config/Config.h"
#include <thread>

class AdvancedLauncher
{
private:
	int m_MonitorCount;
	bool m_processStarted;
	void ProcessThread();
	std::string BuildCommand();
	std::string GetVRChatInstallPath();
	std::string GetLatestVRChatLogFile();
public:
	std::string m_pAppData_Config;
	std::string m_pAppData_VRChat;
	std::string m_pVRChatInstallPath;
	std::string m_fLatestLogFile;

	bool Init();
	void RestarterFunc();

	// Menu
	void MainMenu();
	void LauncherMenu();
	void RestarterMenu();
};