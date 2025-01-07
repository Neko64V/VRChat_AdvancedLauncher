#pragma once
#include "../Framework/AppWindow/AppWindow.h"
#include "../Framework/Config/Config.h"
#include <thread>

class AdvancedLauncher
{
private:
	// Some Data
	ImFont* font;
	int m_MonitorCount;
	std::string m_restartCmd;
	std::string m_latestWorldName;
	std::string m_latestRestartTime;
	std::string m_ConfigFileName = "config.json";

	void ProcessThread();
	std::string BuildCommand();
	std::string FindVRChatInstallationPath();
public:
	std::string m_pAppData_Config;
	std::string m_pAppData_VRChat;
	std::string m_pVRChatInstallPath;

	bool Init();
	void RestarterFunc();

	// Menu
	void MiscMenu();
	void LauncherMenu();
};