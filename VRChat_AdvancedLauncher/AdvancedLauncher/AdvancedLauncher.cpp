#include "AdvancedLauncher.h"
#include "../Utils/Utils.h"
#include <sstream>

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	int* count = reinterpret_cast<int*>(dwData);
	(*count)++;
	return TRUE;
}

bool AdvancedLauncher::Init()
{
	// �e��Path���擾
	m_pAppData_Config = Utils::File::GetAppDataPath(FOLDERID_LocalAppData)    + "\\VRChatAdvancedLauncher";
	m_pAppData_VRChat = Utils::File::GetAppDataPath(FOLDERID_LocalAppDataLow) + "\\VRChat\\VRChat";

	// �h���C�u�̃��[�g�̕����񂪊܂܂�Ă��Ȃ�������
	if (m_pAppData_Config.find(":\\") == std::string::npos || m_pAppData_VRChat.find(":\\") == std::string::npos) {
		// ToDo: Log or Message
		return false;
	}
	else if (!Utils::File::IsExistsDirectory(m_pAppData_Config)) {
		// �t�H���_�̍쐬��json�t�@�C���̈ړ�
		system("setup.bat");
		remove("setup.bat");
	}

	// json����VRChat�̃C���X�g�[�����ǂݎ��
	m_pVRChatInstallPath = cfg.ReadInstallPath(m_pAppData_Config);

	// ���񂩃C���X�g�[���悪�ύX���ꂽ�ꍇ
	if (!Utils::File::IsExistsDirectory(m_pVRChatInstallPath) || !Utils::File::DoesFileExistInDirectory(m_pVRChatInstallPath, "VRChat.exe"))
	{
		// VRChat���̂̃C���X�g�[������擾
		m_pVRChatInstallPath = GetVRChatInstallPath();

		if (m_pVRChatInstallPath.size() == 0)
			MessageBox(nullptr, "VRChat�̃C���X�g�[���悪������܂���ł����B�N����Ɏ蓮�Ŏw�肵�Ă��������B", "ERROR", MB_TOPMOST | MB_OK | MB_ICONERROR);
		else 
			cfg.WriteInstallPath(m_pAppData_Config, m_pVRChatInstallPath); // json�ɕۑ�
	}

	// ���j�^�[�̐����擾
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&m_MonitorCount));

	// config.json����ݒ�����[�h
	cfg.LoadSetting(m_pAppData_Config, "config.json");

	return true;
}

std::string AdvancedLauncher::GetVRChatInstallPath()
{
	std::vector<std::string> steam_dir_list;

	// Steam���C�u������T��
	for (const auto& drive_root : Utils::GetPhysicalDriveList())
	{
		std::string targetDir = "SteamLibrary";
		auto result = Utils::File::FindDirectory(drive_root, targetDir);

		if (result)
			steam_dir_list.push_back(*result);
	}

	// SteamLib���x�[�X��VRChat�̃C���X�g�[�����T��
	for (const auto& dir : steam_dir_list)
	{
		std::string common_path = dir + "\\steamapps\\common";

		if (std::filesystem::is_directory(common_path))
		{
			auto vrc_dir = std::filesystem::directory_iterator(common_path);

			for (const auto& file : vrc_dir) {
				if (file.path().string().find("VRChat") != std::string::npos)
					return file.path().string();
			}
		}
	}

	return std::string();
}

std::string AdvancedLauncher::BuildCommand()
{
	std::ostringstream vOut;

	std::string launchApp = g.g_OfflineTest ? "VRChat.exe" : "launch.exe";
	vOut << launchApp;
	vOut << " \"vrchat://launch/";

	if (g.g_DesktopMode) vOut << " --no-vr";

	vOut << " -screen-fullscreen " + std::to_string((int)g.g_FullScreen);
	vOut << " -screen-width " + std::to_string((int)GetSystemMetrics(SM_CXSCREEN));
	vOut << " -screen-height " + std::to_string((int)GetSystemMetrics(SM_CYSCREEN));

	std::string monitor_str = " -monitor " + std::to_string((int)g.g_Monitor + 1);
	vOut << monitor_str;

	if (g.g_MaxFPSEnable) {
		std::string framerate_str = " --fps=" + std::to_string((int)g.g_MaxFPS);
		vOut << framerate_str;
	}

	if (g.g_AvatarTest) vOut << " --watch-avatars";
	if (g.g_WorldTest) vOut << " --watch-world";

	std::string profile_str = " --profile=" + std::to_string((int)g.g_ProfileID);
	vOut << profile_str;

	if (g.g_CCX_Enable) {
		switch (g.g_CCX_Option)
		{
		case 0:
			vOut << " --affinity=3F";
			break;
		case 1:
			vOut << " --affinity=FF";
			break;
		case 2:
			vOut << " --affinity=FFF";
			break;
		case 3:
			vOut << " --affinity=FFFF";
			break;
		}
	}

	vOut << "\"";

	std::cout << vOut.str() << std::endl;

	return vOut.str();
}

std::string AdvancedLauncher::GetLatestVRChatLogFile()
{
	std::string latest_file;
	std::time_t latest_time = 0;
	std::regex log_pattern(R"(output_log_\d{4}-\d{2}-\d{2}_\d{2}-\d{2}-\d{2}\.txt)");

	for (const auto& entry : std::filesystem::directory_iterator(m_pAppData_VRChat))
	{
		if (entry.is_regular_file())
		{
			std::string file_name = entry.path().filename().string();

			if (std::regex_match(file_name, log_pattern))
			{
				std::tm tm = {};
				std::istringstream ss(file_name.substr(11, 19));
				ss >> std::get_time(&tm, "%Y-%m-%d_%H-%M-%S");
				std::time_t fileTime = std::mktime(&tm);

				if (fileTime > latest_time) {
					latest_time = fileTime;
					latest_file = entry.path().string();
				}
			}
		}
	}

	return latest_file;
}