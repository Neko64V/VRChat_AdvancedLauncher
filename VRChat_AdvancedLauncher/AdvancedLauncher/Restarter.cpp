#include "AdvancedLauncher.h"
#include "../Utils/Utils.h"
#include <sstream>
#include <time.h>

void AdvancedLauncher::RestarterFunc()
{
	// �f�[�^�ۑ��p
	std::string latestFileName;
	std::filesystem::file_time_type latestWriteTime;

	while (true)
	{
		// ��
		std::string worldId = std::string();
		std::string instanceID = std::string();
		std::string instanceType = std::string();
		std::string worldHost = std::string();
		std::string worldRegion = std::string();

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
				// �^�C���A�E�g
				if (line.find("[Behaviour] Timeout:") != std::string::npos)
				{
					std::cout << "[-] Timeout Detected!" << std::endl;

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

				// �ŐV�̃��[���h�ƃC���X�^���X���L�^
				if (line.find("[Behaviour] Joining") != std::string::npos && line.find("wrld_") != std::string::npos)
				{
					try
					{
						m_restartCmd = " \"vrchat://launch?id=wrld_" + line + "\"";

						/*
						/*
						[+] �N���R�}���h
						Public:
						launch.exe "vrchat://launch?id=wrld_4432ea9b-729c-46e3-8eaf-846aa0a37fdd:12345~region(jp)"

						Private:
						launch.exe "vrchat://launch?id=wrld_4432ea9b-729c-46e3-8eaf-846aa0a37fdd~private(usr_ab1234cd-123f-abc0-1234-0123456HIJK)~region(jp)

						std::smatch matches;

						if (std::regex_search(line, matches, std::regex("wrld_([a-fA-F0-9\\-]+):([0-9]+)~region\\(([^)]+)\\)"))) 
						{
							// Public
							if (matches.size() > 0)
								m_restartCmd = " \"vrchat://launch?id=wrld_" + matches[1].str() + ":" + matches[2].str() + "~region(" + matches[3].str() + ")\"";

							// GUI�p�̂��
							worldId = matches[1].str();
							instanceID = matches[2].str();
							worldRegion = matches[3].str();
							instanceType = "Public";
						}
						else if (std::regex_search(line, matches, std::regex("wrld_([a-fA-F0-9\\-a-z]+):([0-9]+)~([a-fA-F\\-a-z]+)\\(usr_([a-fA-F0-9\\-]+)\\)~region\\(([^)]+)\\)"))) 
						{
							// !(Public)
							if (matches.size() > 0)
								m_restartCmd = " \"vrchat://launch?id=wrld_" + matches[1].str() + ":" + matches[2].str() + "~" + matches[3].str() + "(usr_" + matches[4].str() + ")" + "~region(" + matches[5].str() + ")\"";
						
							// GUI�p�̂��
							worldId = matches[1].str();
							instanceID = matches[2].str();
							instanceType = matches[3].str();
							worldHost = matches[4].str();
							worldRegion = matches[5].str();
						}
						else // �O���[�v
						{
							// https://vrchat.com/home/launch?worldId=wrld_2eb69983-95e5-4e43-a7d3-9b4033ea5b62&instanceId=85998~group(grp_1a78d42c-eae6-4ca6-b87e-5191243161ae)~groupAccessType(public)~region(jp)
							if (std::regex_search(line, matches, std::regex("wrld_([a-fA-F0-9\\-a-z]+):([0-9]+)~([a-fA-F\\-a-z]+)\\(grp_([a-fA-F0-9\\-]+)\\)~groupAccessType\\(([^)]+)~region\\(([^)]+)\\)")))
							{

							}
						}
						*/
					}
					catch (const std::exception& e)
					{
						std::cerr << "Regex error: " << e.what() << std::endl;
					}
				}
			}

			file.close();
		}

		// ��
		m_latestWorldID = worldId;
		m_latestInstanceID = instanceID;
		m_latesInstanceType = instanceType;
		m_latestWorldHostUser = worldHost;
		m_latestWorldRegion = worldRegion;

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}