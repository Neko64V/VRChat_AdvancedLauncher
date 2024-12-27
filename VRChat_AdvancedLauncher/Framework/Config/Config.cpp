#include "Config.h"
#include "../Utils/Utils.h"

Config g;
ConfigManager cfg;

void ConfigManager::LoadSetting(const std::string path, const std::string filename)
{
    if (filename.empty())
        return;

    std::string p = path + "\\" + filename;
    std::ifstream file(p);
    json JSON;
    file >> JSON;

    g.AutoRestarter = JSON["AutoRestarter"];
    g.g_AvatarTest = JSON["AvatarTest"];
    g.g_CCX_Enable = JSON["CCXEnable"];
    g.g_CCX_Option = JSON["CCXOption"];
    g.g_DesktopMode = JSON["DesktopMode"];
    g.g_FullScreen = JSON["FullScreen"];
    g.g_MaxFPS = JSON["MaxFPS"];
    g.g_MaxFPSEnable = JSON["MaxFPSEnable"];
    g.g_Monitor = JSON["Monitor"];
    g.g_OfflineTest = JSON["OffliteTest"];
    g.g_ProfileID = JSON["ProfileID"];
    g.g_WorldTest = JSON["WorldTest"];

    file.close();
}

void ConfigManager::SaveSetting(const std::string path, const std::string filename)
{
    if (filename.empty())
        return;

    std::string configPath = path + "\\" + filename;
    std::ifstream file(configPath);

    if (file.good()) {
        json JSON;
        file >> JSON;

        JSON["AutoRestarter"] = g.AutoRestarter;
        JSON["AvatarTest"] = g.g_AvatarTest;
        JSON["CCXEnable"] = g.g_CCX_Enable;
        JSON["CCXOption"] = g.g_CCX_Option;
        JSON["DesktopMode"] = g.g_DesktopMode;
        JSON["FullScreen"] = g.g_FullScreen;
        JSON["MaxFPS"] = g.g_MaxFPS;
        JSON["MaxFPSEnable"] = g.g_MaxFPSEnable;
        JSON["Monitor"] = g.g_Monitor;
        JSON["OffliteTest"] = g.g_OfflineTest;
        JSON["ProfileID"] = g.g_ProfileID;
        JSON["WorldTest"] = g.g_WorldTest;

        std::ofstream outputFile(configPath, std::ios::trunc);
        if (outputFile.good()) {
            outputFile << JSON.dump(4);
            std::cout << "[ dev ] jsonをアップデート" << std::endl;
        }
        else {
            std::cerr << "[ dev ] jsonのアップデートに失敗" << std::endl;
        }
    }

    file.close();
}

std::string ConfigManager::ReadInstallPath(const std::string& appdata_local)
{
    std::string configPath = appdata_local;
    std::string configFile = configPath + "\\config.json";

    if (!Utils::File::IsExistsDirectory(configPath)) {
        MessageBox(nullptr, "setup.batを実行しましたか？", "ERROR", MB_TOPMOST | MB_OK | MB_ICONERROR);
        return std::string();
    }

    std::ifstream f(configFile);

    if (f.good()) {
        json j;
        f >> j;

        std::string p = j["VRChatPath"];
        f.close();
        
        return p;
    }
    else {
        return std::string();
    }
}

void ConfigManager::WriteInstallPath(const std::string& appdata_local, const std::string& vrc_path)
{
    std::string path = appdata_local + "\\config.json";

    if (!Utils::File::IsExistsFile(path)) {
        MessageBox(nullptr, "setup.batを実行しましたか？", "ERROR", MB_TOPMOST | MB_OK | MB_ICONERROR);
        return;
    }

    std::ifstream f(path);

    if (f.good()) {
        json j;
        f >> j;

        j["VRChatPath"] = vrc_path;

        std::ofstream outputFile(path, std::ios::trunc);
        if (outputFile.good()) {
            outputFile << j.dump(4);
            std::cout << "[ dev ] jsonをアップデート" << std::endl;
        }
        else {
            std::cerr << "[ dev ] jsonのアップデートに失敗" << std::endl;
        }
    }

    f.close();
}