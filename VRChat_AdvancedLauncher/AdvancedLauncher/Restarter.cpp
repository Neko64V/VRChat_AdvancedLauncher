#include "AdvancedLauncher.h"
#include "../Utils/Utils.h"
#include <sstream>
#include <time.h>

void AdvancedLauncher::RestarterFunc()
{
	// �f�[�^�ۑ��p
	std::string latestFileName;
	std::string worldName;
	std::filesystem::file_time_type latestWriteTime;

	while (true)
	{
		if (g.AutoRestarter)
		{
			while (!Utils::Process::IsProcessRunning("VRChat.exe"))
				std::this_thread::sleep_for(std::chrono::seconds(5));

			// ���O�t�@�C�����ŐV�ł��邩�̊m�F
			for (const auto& entry : std::filesystem::directory_iterator(m_pAppData_VRChat))
			{
				if (entry.is_regular_file())
				{
					// output_log_�Ƃ����������܂݊g���q��.txt�ȃt�@�C��
					if (entry.path().filename().string().find("output_log_") != std::string::npos && !entry.path().extension().compare(".txt"))
					{
						// �ŐV�̃t�@�C���H
						if (latestWriteTime < entry.last_write_time()) {
							latestFileName = entry.path().filename().string();
							latestWriteTime = entry.last_write_time();
						}
					}
				}
			}

			// �ǂݍ��ރt�@�C�����w��
			std::string line;
			std::ifstream file(m_pAppData_VRChat + "\\" + latestFileName);

			// ���O�t�@�C����1�s�Âǂݎ��A�Ώۂ̕������T��
			while (std::getline(file, line))
			{
				// ��s�̓X�L�b�v
				if (line.empty())
					continue;

				std::smatch matches;

				// ReJoin���郏�[���h��ۑ�
				if (line.find("[Behaviour] Joining") != std::string::npos && line.find("wrld_") != std::string::npos) 
				{
					try {
						if (std::regex_search(line, matches, std::regex("wrld_(.+)")))
							m_restartCmd = " \"vrchat://launch?id=" + matches[0].str() + "\"";
					}
					catch (const std::exception& e) {
						std::cerr << "Regex error: " << e.what() << std::endl; // dev
					}
				}

				// ���[���h�̖���
				if (line.find("[Behaviour] Joining or Creating") != std::string::npos) 
				{
					try {
						if (std::regex_search(line, matches, std::regex(R"(Joining or Creating Room: (.+))")))
							worldName = matches[1].str();
					}
					catch (const std::exception& e) {
						std::cerr << "Regex error: " << e.what() << std::endl; // dev
					}
				}

				// �^�C���A�E�g
				if (line.find("[Behaviour] Timeout") != std::string::npos)
				{
					std::cout << "[-] Timeout Detected" << std::endl;

					// �Ō�Ƀ^�C���A�E�g�����o�������Ԃ��L�^
					std::time_t tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					std::tm now{};
					if (SUCCEEDED(_localtime64_s(&now, &tm)))
					{
						now.tm_year += 1900;
						now.tm_mon += 1;
						std::string t_year = std::to_string(now.tm_year).length() == 1 ? "0" + std::to_string(now.tm_year) : std::to_string(now.tm_year);
						std::string t_mon = std::to_string(now.tm_mon).length() == 1 ? "0" + std::to_string(now.tm_mon) : std::to_string(now.tm_mon);
						std::string t_day = std::to_string(now.tm_mday).length() == 1 ? "0" + std::to_string(now.tm_mday) : std::to_string(now.tm_mday);
						std::string t_hour = std::to_string(now.tm_hour).length() == 1 ? "0" + std::to_string(now.tm_hour) : std::to_string(now.tm_hour);
						std::string t_min = std::to_string(now.tm_min).length() == 1 ? "0" + std::to_string(now.tm_min) : std::to_string(now.tm_min);
						std::string t_sec = std::to_string(now.tm_sec).length() == 1 ? "0" + std::to_string(now.tm_sec) : std::to_string(now.tm_sec);

						std::ostringstream vTime;
						vTime << t_year + "/";
						vTime << t_mon + "/";
						vTime << t_day + " - ";
						vTime << t_hour + ":";
						vTime << t_min + ":";
						vTime << t_sec;
						m_latestRestartTime = vTime.str();
					}

					// VRChat�����s���H
					if (Utils::Process::IsProcessRunning("VRChat.exe"))
						Utils::Process::StopProcess("VRChat.exe");

					file.close();
					std::this_thread::sleep_for(std::chrono::seconds(5));

					// �v���Z�X�̍ċN��
					std::string start_full_cmd = m_pVRChatInstallPath + "\\launch.exe" + m_restartCmd;
					Utils::Process::StartProcess(start_full_cmd);

					// �ŐV�̃��O�t�@�C�������������܂őҋ@
					while (true)
					{
						std::string fileName;

						for (const auto& entry : std::filesystem::directory_iterator(m_pAppData_VRChat))
						{
							if (entry.is_regular_file())
							{
								// output_log_�Ƃ����������܂݊g���q��.txt�ȃt�@�C��
								if (entry.path().filename().string().find("output_log_") != std::string::npos && !entry.path().extension().compare(".txt"))
								{
									// �ŐV�̃t�@�C���H
									if (latestWriteTime < entry.last_write_time()) {
										fileName = entry.path().filename().string();
										latestWriteTime = entry.last_write_time();
									}
								}
							}
						}

						// �t�@�C�����������ꂽ��
						if (latestFileName.compare(fileName)) {
							std::cout << "[+] New log file generated!" << std::endl;
							latestFileName = fileName;
							std::this_thread::sleep_for(std::chrono::seconds(1));
							break;
						}

						std::this_thread::sleep_for(std::chrono::seconds(5));
					}

					break;
				}
			}

			file.close();
		}

		m_latestWorldName = worldName;
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}