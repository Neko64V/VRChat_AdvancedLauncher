#include "AdvancedLauncher.h"
#include "../Utils/Utils.h"

void AdvancedLauncher::RestarterFunc()
{
	std::string InCommand;

	while (true)
	{
		//if (g.AutoRestarter)
		{
			// VRChatが開始されるまで待機
			while (!Utils::Process::IsProcessRunning("VRChat.exe"))
				std::this_thread::sleep_for(std::chrono::seconds(5));

			// ターゲットのディレクトリのファイルを取得しログファイルを抽出
			std::vector<std::filesystem::path> LogList;
			auto VRC_LogFileList = std::filesystem::directory_iterator(m_pAppData_VRChat);

			// 有効なログファイルを抽出
			for (const auto& file : VRC_LogFileList)
			{
				if (file.file_size() == 0)
					continue;
				else if (file.path().extension().string().compare(".txt"))
					continue;
				else if (file.path().extension().string().find("output_log") != std::string::npos)
					continue;

				LogList.push_back(file);
			}

			// 新しいログファイルが存在するかを確認
			std::string last_log_file = GetLatestVRChatLogFile();

			if (last_log_file.compare(m_fLatestLogFile)) {
				m_fLatestLogFile = last_log_file;
				std::cout << "[ LOG ] Logfile changed: " << last_log_file << std::endl;
			}

			// ログファイルが存在しない、またはサイズが0だったら
			if (LogList.size() == 0) {
				continue;
			}
			else if (LogList.size() > 1) {
				for (const auto& log : LogList) {
					if (log.string().compare(m_fLatestLogFile))
						remove(log.string().c_str());
				}
			}

			// 読み込むファイルを指定
			std::ifstream file(last_log_file.c_str());
			std::string line;

			// ログファイルを1行づつ読み取り、対象の文字列を探す
			while (std::getline(file, line))
			{
				// タイムアウト
				if (line.find("[Behaviour] Timeout:") != std::string::npos)
				{
					std::cout << "[-] Timeout detected!" << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(3));

					Utils::Process::StopProcess("VRChat.exe");

					std::this_thread::sleep_for(std::chrono::seconds(3));
					file.close();

					if (remove(last_log_file.c_str()) == -1) {
						perror("[ ERROR ] Failed to delete log file. reason :");
						continue;
					}

					// プロセスの再起動
					std::cout << "[+] VRChatを起動中…" << std::endl;

					Utils::Process::StartProcess("");

					// 再起動を待つ
					while (!Utils::Process::IsProcessRunning("VRChat.exe"))
						std::this_thread::sleep_for(std::chrono::seconds(5));

					std::cout << "[+] VRChatの再起動が完了" << std::endl;

					// 新しいログファイルが生成されるまで待っておく
					std::this_thread::sleep_for(std::chrono::seconds(20));

					break;
				}

				// 最新のワールドとインスタンスを記録
				if (line.find("[Behaviour] Joining") != std::string::npos && line.find("wrld_") != std::string::npos)
				{
					try
					{
						/*
						[+] 起動コマンド
						Public:
						launch.exe "vrchat://launch?id=wrld_4432ea9b-729c-46e3-8eaf-846aa0a37fdd:12345~region(jp)"

						Private:
						launch.exe "vrchat://launch?id=wrld_4432ea9b-729c-46e3-8eaf-846aa0a37fdd~private(usr_ab1234cd-123f-abc0-1234-0123456HIJK)~region(jp)
						*/

						std::smatch matches;

						if (std::regex_search(line, matches, std::regex("wrld_([a-fA-F0-9\\-]+):([0-9]+)~region\\(([^)]+)\\)"))) {
							// Public
							if (matches.size() > 0)
								InCommand = "vrchat://launch?id=wrld_" + matches[1].str() + ":" + matches[2].str() + "~region(" + matches[3].str() + ")";
						}
						else if (std::regex_search(line, matches, std::regex("wrld_([a-fA-F0-9\\-a-z]+):([0-9]+)~([a-fA-F\\-a-z]+)\\(usr_([a-fA-F0-9\\-]+)\\)~region\\(([^)]+)\\)"))) {
							// Not Public
							if (matches.size() > 0)
								InCommand = "vrchat://launch?id=wrld_" + matches[1].str() + ":" + matches[2].str() + "~" + matches[3].str() + "(usr_" + matches[4].str() + ")" + "~region(" + matches[5].str() + ")";
						}

					}
					catch (const std::exception& e)
					{
						std::cerr << "Regex error: " << e.what() << std::endl;
					}
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}