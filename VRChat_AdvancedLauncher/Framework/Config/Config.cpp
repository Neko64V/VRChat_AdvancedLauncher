#include "Config.h"
#include "../Utils/Utils.h"

Config g;
ConfigManager cfg;

void ConfigManager::LoadSetting(const std::string filename)
{
    if (filename.empty())
        return;

    std::ifstream f(filename);

}

void ConfigManager::SaveSetting(const std::string filename)
{
    if (filename.empty())
        return;

    std::ifstream f(filename);
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