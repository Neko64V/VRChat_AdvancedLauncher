#include "AdvancedLauncher.h"
#include "../Utils/Utils.h"
#include <thread>

void AdvancedLauncher::MainMenu()
{
    ImGui::SetNextWindowSize(ImVec2(800.f, 500.f));
    ImGui::Begin("VRChat - Advanced Launcher", &g.ApplicationActive, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::BeginChild("##C1", ImVec2(ImGui::GetContentRegionAvail().x / 2.f, ImGui::GetContentRegionAvail().y));

    ImGui::SeparatorText("Restarter");

    ImGui::Checkbox("AutoRestart", &g.AutoRestarter);

    ImGui::Spacing();
    ImGui::NewLine();
    
    ImGui::SeparatorText("Path");

    ImGui::Text("[+] VRChat Path");
    ImGui::Text(m_pVRChatInstallPath.c_str());

    ImGui::SameLine();

    if (ImGui::Button("X"))
        std::thread([&]() { Utils::File::SelectDirectoryPath(m_pVRChatInstallPath); }).detach();

    ImGui::EndChild();

    ImGui::SameLine();
}

void AdvancedLauncher::LauncherMenu()
{
    static ImColor TitleTextCol = ImColor(0.17f, 0.67f, 0.76f, 0.65f);
    static const char* profileList[] = { "Main", "Sub", "Empty" };
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

    ImGui::BeginChild("##C2-0", ImVec2(ImGui::GetContentRegionAvail()));

    ImGui::TextColored(TitleTextCol, "Display");
    ImGui::Separator();
    ImGui::Checkbox("DesktopMode", &g.g_DesktopMode); // --no-vr
    ImGui::Checkbox("FullScreen", &g.g_FullScreen); // -screen-fullscreen 0/1
    ImGui::Checkbox("FPS Limit", &g.g_MaxFPSEnable);
    ImGui::SliderInt("Max FPS", &g.g_MaxFPS, 10, 240); // --fps=**
    ImGui::Combo("Monitor", &g.g_Monitor, monitorList, m_MonitorCount); // -monitor

    ImGui::Spacing();
    ImGui::NewLine();

    ImGui::TextColored(TitleTextCol, "Test");
    ImGui::Separator();
    ImGui::Checkbox("Offline", &g.g_OfflineTest);
    ImGui::Checkbox("Avater Test", &g.g_AvatarTest); // --watch-avatars
    ImGui::Checkbox("World Test", &g.g_WorldTest); // --watch-world
    ImGui::Spacing();
    ImGui::NewLine();

    ImGui::TextColored(TitleTextCol, "Misc");
    ImGui::Separator();
    ImGui::Combo("Profile", &g.g_ProfileID, profileList, IM_ARRAYSIZE(profileList)); // --profile=?

    ImGui::Checkbox("CCX Option -", &g.g_CCX_Enable); ImGui::SameLine();
    ImGui::TextColored(ImColor(1.f, 0.f, 0.f, 1.f), "Some Ryzen 1/2/3/5/7000 user only");

    if (g.g_CCX_Enable)
        ImGui::Combo("Core per CCX", &g.g_CCX_Option, coreList, IM_ARRAYSIZE(coreList));

    // Launch
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30.f);
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

    ImGui::EndChild();

    // CleanUp
    for (int k = 0; k < m_MonitorCount; k++)
        delete[] monitorList[k];

    delete[] monitorList;
}