#pragma once
#include "../Framework/AppWindow/AppWindow.h"
#include "../Framework/Config/Config.h"
#include <thread>

class AdvancedLauncher
{
private:
	// Some Data
	int m_MonitorCount;
	std::string m_restartCmd;
	std::string m_latestRestartTime = "NoData";

	// WorldData
	std::string m_latestWorldID;
	std::string m_latestInstanceID;
	std::string m_latesInstanceType;
	std::string m_latestWorldHostUser;
	std::string m_latestWorldRegion;

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