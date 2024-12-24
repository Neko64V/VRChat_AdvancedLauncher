#pragma once
#include "../Framework/AppWindow/AppWindow.h"
#include "../Framework/Config/Config.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <thread>
#include <vector>

#include <optional>
#include <Windows.h>
#include <filesystem>
#include <TlHelp32.h>

class AdvancedLauncher
{
private:
	std::string BuildCommand();
public:
	std::string m_pAppData_Config;
	std::string m_pAppData_VRChat;
	std::string m_pVRChatInstallPath;

	// Menu
	void MainMenu();
	void LauncherMenu();
	void RestarterMenu();

	bool Init();

	std::string GetVRChatInstallPath();
	std::string GetLatestLogFile(const std::string& dir);
};