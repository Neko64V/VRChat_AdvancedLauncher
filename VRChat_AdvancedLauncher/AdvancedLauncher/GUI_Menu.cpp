#include "AdvancedLauncher.h"
#include "../Utils/Utils.h"
#include <thread>

static ImColor TitleTextCol = ImColor(0.17f, 0.67f, 0.76f, 0.65f);

void AdvancedLauncher::LauncherMenu()
{
    static const char* profileList[] = { "Profile 1", "Profile 2", "Profile 3" };
    static const char* coreList[] = { "3 [ Ryzen5  (3Core * 2CCX)]", "4 [ Ryzen7  (4Core * 2CCX)]", "6 [ Ryzen9  (6Core * 2CCX)]", "8 [ Ryzen9+ (8Core * 2CCX)]" };
    const char** monitorList = new const char* [m_MonitorCount];

    ImGuiStyle& style = ImGui::GetStyle();
    style.SeparatorTextBorderSize = 1.f;

    for (int j = 0; j < m_MonitorCount; j++) {
        std::string itemStr = "Monitor " + std::to_string(j + 1);
        char* itemCopy = new char[itemStr.size() + 1];
        strcpy_s(itemCopy, itemStr.size() + 1, itemStr.c_str());
        monitorList[j] = itemCopy;
    }

    ImGui::TextColored(TitleTextCol, "Display");
    ImGui::Separator();
    ImGui::Checkbox("DesktopMode", &g.g_DesktopMode); // --no-vr
    ImGui::Checkbox("FullScreen", &g.g_FullScreen); // -screen-fullscreen 0/1
    ImGui::Checkbox("FPS Limit", &g.g_MaxFPSEnable);
    ImGui::SliderInt("Max FPS", &g.g_MaxFPS, 10, 240); // --fps=**
    ImGui::Combo("Monitor", &g.g_Monitor, monitorList, m_MonitorCount); // -monitor

    ImGui::NewLine();

    ImGui::TextColored(TitleTextCol, "Test");
    ImGui::Separator();
    ImGui::Checkbox("Offline", &g.g_OfflineTest);
    ImGui::Checkbox("Avater Test", &g.g_AvatarTest); // --watch-avatars
    ImGui::Checkbox("World Test", &g.g_WorldTest); // --watch-world

    ImGui::NewLine();

    ImGui::TextColored(TitleTextCol, "Misc");
    ImGui::Separator();
    ImGui::Combo("Profile", &g.g_ProfileID, profileList, IM_ARRAYSIZE(profileList)); // --profile=?

    ImGui::Checkbox("CCX Option -", &g.g_CCX_Enable); ImGui::SameLine();
    ImGui::TextColored(ImColor(1.f, 0.f, 0.f, 1.f), "Some Ryzen 1/2/3/5/7000 user only");

    if (g.g_CCX_Enable)
        ImGui::Combo("Core per CCX", &g.g_CCX_Option, coreList, IM_ARRAYSIZE(coreList));

    // Launch
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30.f - (style.WindowPadding.y * 3));
    if (ImGui::Button("Launch", ImVec2(ImGui::GetContentRegionAvail().x, 30.f))) {

        if (Utils::Process::IsProcessRunning("VRChat.exe"))
        {
            int result = MessageBox(NULL, "既に他のVRChatインスタンスが存在します。\n本当に実行しますか？", "確認", MB_YESNO | MB_TOPMOST | MB_ICONWARNING);

            if (result == IDYES)
                std::thread([&]() { ProcessThread(); }).detach();
        }
        else {
            std::thread([&]() { ProcessThread(); }).detach();
        }
    }

    // CleanUp
    for (int k = 0; k < m_MonitorCount; k++)
        delete[] monitorList[k];

    delete[] monitorList;
}

void AdvancedLauncher::MiscMenu()
{
    ImGui::TextColored(TitleTextCol, "Restarter");
    ImGui::Separator();

    ImGui::Checkbox("AutoRestart", &g.AutoRestarter);
    ImGui::Text("[ Latest Disconnected ]");
    ImGui::Text(m_latestRestartTime.c_str());

    ImGui::Spacing();
    ImGui::NewLine();

    ImGui::TextColored(TitleTextCol, "Latest World");
    ImGui::Separator();

    ImGui::Text("[ World ID ]");
    ImGui::Text(m_latestWorldID.c_str());
    ImGui::Text("[ Instance ID ]");
    ImGui::Text(m_latestInstanceID.c_str());
    ImGui::Text("[ Instance Type ]");
    ImGui::Text(m_latesInstanceType.c_str());

    if (m_latesInstanceType.compare("Public")) {
        ImGui::Text("[ HostUser ]");
        ImGui::Text(m_latestWorldHostUser.c_str());
    }

    ImGui::Text("[ Region ]");
    ImGui::Text(m_latestWorldRegion.c_str());

    ImGui::NewLine();

    // Launch
    if (ImGui::Button("Launch", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 8.f, 24.f))) {

        //std::string run_cmd = m_pVRChatInstallPath + "\\" + BuildCommand();
        std::string run_cmd = m_pVRChatInstallPath + "\\launch.exe" + m_restartCmd;

        if (Utils::Process::IsProcessRunning("VRChat.exe"))
        {
            int result = MessageBox(NULL, "既に他のVRChatインスタンスが存在します。\n本当に実行しますか？", "確認", MB_YESNO | MB_TOPMOST | MB_ICONWARNING);

            if (result == IDYES)
                Utils::Process::StartProcess(run_cmd);
        }
        else {
            Utils::Process::StartProcess(run_cmd);
        }
    }

    ImGui::SameLine();

    // Open with Web
    if (ImGui::Button("Open WebPage", ImVec2(ImGui::GetContentRegionAvail().x, 24.f))) {
        std::string temp_link = "https://vrchat.com/home/launch?worldId=wrld_" + m_latestWorldID + "&instanceId=" + m_latestInstanceID + "~";

        if (m_latesInstanceType.compare("Public")) {
            std::string temp_pms =  m_latesInstanceType + "(usr_" + m_latestWorldHostUser + ")~";
            temp_link += temp_pms;
        }

        temp_link += "region(" + m_latestWorldRegion + ")";
         
        ShellExecute(nullptr, "open", temp_link.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }

    ImGui::Spacing();
    ImGui::NewLine();
    
    ImGui::TextColored(TitleTextCol, "Path");
    ImGui::Separator();

    ImGui::Text("[+] VRChat Path");
    ImGui::Text(m_pVRChatInstallPath.c_str());

    ImGui::SameLine();

    if (ImGui::Button("X"))
        std::thread([&]() { Utils::File::SelectDirectoryPath("VRChatのインストール先を選択", m_pVRChatInstallPath); }).detach();

    ImGui::SameLine();
}