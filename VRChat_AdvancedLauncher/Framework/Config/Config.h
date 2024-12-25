#pragma once
#include <fstream>
#include <Windows.h>
#include <filesystem>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct Config
{
    // System
    bool ApplicationActive = true;
   
    // Launcher
    bool g_DesktopMode = false;
    bool g_FullScreen  = false;
    bool g_MaxFPSEnable = true;
    int  g_MaxFPS = 144;
    int  g_Monitor = 0;
    bool g_OfflineTest = false;
    bool g_WorldTest = false;
    bool g_AvatarTest = false;
    int  g_ProfileID = 0;
    bool g_CCX_Enable = false;
    int  g_CCX_Option = 0;
};

// https://github.com/nlohmann/json
class ConfigManager
{
private:

public:
    void LoadSetting(const std::string path, const std::string filename);
    void SaveSetting(const std::string path, const std::string filename);
    std::string ReadInstallPath(const std::string& appdata_local);
    void WriteInstallPath(const std::string& appdata_local, const std::string& vrc_path);
};

extern Config g;
extern ConfigManager cfg;