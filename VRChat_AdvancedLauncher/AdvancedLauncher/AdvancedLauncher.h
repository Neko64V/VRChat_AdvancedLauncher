#pragma once
#include "../Framework/AppWindow/AppWindow.h"
#include "../Framework/Config/Config.h"

class AdvancedLauncher
{
private:
	int m_MonitorCount;
	bool m_processStarted;
	void ProcessThread();
	std::string BuildCommand();
	std::string GetVRChatInstallPath();
	std::string GetLatestLogFile(const std::string& dir);
public:
	std::string m_pAppData_Config;
	std::string m_pAppData_VRChat;
	std::string m_pVRChatInstallPath;

	bool Init();

	// Menu
	void MainMenu();
	void LauncherMenu();
	void RestarterMenu();
};