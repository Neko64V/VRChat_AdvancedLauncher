#include "AdvancedLauncher.h"
#include "../Utils/Utils.h"

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	int* count = reinterpret_cast<int*>(dwData);
	(*count)++;
	return TRUE;
}

bool AdvancedLauncher::Init()
{
	// AppData\Local(Low)からのパス
	static std::string ConfigPath	= "\\VRChatAdvancedLauncher";
	static std::string VRC_LogPath	= "\\VRChat\\VRChat";

	// 各種Pathを取得
	m_pAppData_Config = Utils::File::GetAppDataPath(FOLDERID_LocalAppData) + ConfigPath;
	m_pAppData_VRChat = Utils::File::GetAppDataPath(FOLDERID_LocalAppDataLow) + VRC_LogPath;

	// ドライブのルートの文字列が含まれていなかったら
	if (m_pAppData_Config.find(":\\") == std::string::npos || m_pAppData_VRChat.find(":\\") == std::string::npos) {
		// ToDo: Log or Message
		return false;
	}
	else if (!Utils::File::IsExistsDirectory(m_pAppData_Config)) {
		// フォルダの作成とjsonファイルの移動
		system("setup.bat");
		remove("setup.bat");
	}

	// jsonからVRChatのインストール先を読み取る
	m_pVRChatInstallPath = cfg.ReadInstallPath(m_pAppData_Config);

	// 初回かインストール先が変更された場合
	if (!Utils::File::IsExistsDirectory(m_pVRChatInstallPath) || !Utils::File::DoesFileExistInDirectory(m_pVRChatInstallPath, "VRChat.exe"))
	{
		// VRChat自体のインストール先を取得
		m_pVRChatInstallPath = FindVRChatInstallationPath();

		if (m_pVRChatInstallPath.size() == 0) {
			MessageBox(nullptr, "VRChatのインストール先が見つかりませんでした。起動後に手動で指定してください。", "ERROR", MB_TOPMOST | MB_OK | MB_ICONERROR);
		}	
		else {
			cfg.WriteInstallPath(m_pAppData_Config, m_pVRChatInstallPath); // jsonに保存
		}
	}

	// モニターの数を取得
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&m_MonitorCount));

	// config.jsonから設定をロード
	cfg.LoadSetting(m_pAppData_Config, "config.json");

	return true;
}

void AdvancedLauncher::ProcessThread()
{
	cfg.SaveSetting(m_pAppData_Config, "config.json");

	std::string run_cmd = m_pVRChatInstallPath + "\\" + BuildCommand();
	Utils::Process::StartProcess(run_cmd);

	while (!Utils::Process::IsProcessRunning("VRChat.exe"))
		std::this_thread::sleep_for(std::chrono::seconds(5));
}

std::string AdvancedLauncher::FindVRChatInstallationPath()
{
	static std::string targetDir = "SteamLibrary";
	std::vector<std::string> steam_dir_list;

	// Steamライブラリを探す
	for (const auto& drive_root : Utils::GetPhysicalDriveList()) 
	{
		auto result = Utils::File::FindDirectory(drive_root, targetDir);

		if (result)
			steam_dir_list.push_back(*result);
	}

	// SteamLibをベースにVRChatのインストール先を探す
	for (const auto& dir : steam_dir_list)
	{
		std::string common_path = dir + "\\steamapps\\common";

		if (std::filesystem::is_directory(common_path)) {
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

	return vOut.str();
}