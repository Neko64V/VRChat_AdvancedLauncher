#include "AdvancedLauncher.h"
#include "../Utils/Utils.h"
#include <thread>

void AdvancedLauncher::MainMenu()
{

}

void AdvancedLauncher::LauncherMenu()
{
    static const char* profileList[] = { "Main", "Sub", "Empty" };
    static const char* coreList[] = { "3 [ Ryzen5 (3Core * 2CCX)]", "4 [ Ryzen7 (4Core * 2CCX)]", "6 [ Ryzen9 (6Core * 2CCX)]", "8 [ Ryzen9+ (8Core * 2CCX)]" };

    ImGuiStyle& style = ImGui::GetStyle();
    style.SeparatorTextBorderSize = 1.f;

    ImGui::SetNextWindowSize(ImVec2(400.f, 500.f));
    ImGui::Begin("VRChat - Advanced Launcher", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::SeparatorText("Display");
    ImGui::Checkbox("DesktopMode", &g.g_DesktopMode); // --no-vr
    ImGui::Checkbox("FullScreen", &g.g_FullScreen); // -screen-fullscreen 0/1
    ImGui::Checkbox("FPS Limit", &g.g_MaxFPSEnable);
    ImGui::SliderInt("Max FPS", &g.g_MaxFPS, 10, 240); // --fps=**

    ImGui::Spacing();
    ImGui::NewLine();

    ImGui::SeparatorText("Test");
    ImGui::Checkbox("Offline", &g.g_OfflineTest);
    ImGui::Checkbox("Avater Test", &g.g_AvatarTest); // --watch-avatars
    ImGui::Checkbox("World Test", &g.g_WorldTest); // --watch-world
 
    ImGui::Spacing();
    ImGui::NewLine();

    ImGui::SeparatorText("Misc");
    ImGui::Combo("Profile", &g.g_ProfileID, profileList, IM_ARRAYSIZE(profileList)); // --profile=?

    ImGui::Checkbox("CCX Option -", &g.g_CCX_Enable); ImGui::SameLine();
    ImGui::TextColored(ImColor(1.f, 0.f, 0.f, 1.f), "Some Ryzen 1/2/3/5/7000 user only");

    if (g.g_CCX_Enable)
        ImGui::Combo("Core per CCX", &g.g_CCX_Option, coreList, IM_ARRAYSIZE(coreList));

    // Launch
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 30.f - (style.WindowPadding.y));
    if (ImGui::Button("Launch", ImVec2(ImGui::GetContentRegionAvail().x, 30.f)))
        std::thread([&]() { Utils::Process::StartProcess("", ""); }).detach();

    ImGui::End();
}

void AdvancedLauncher::RestarterMenu()
{

}