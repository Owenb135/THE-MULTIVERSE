///Created by Owen0963///
#include <array>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
using namespace std;
const std::array<std::string, 2> files = {
	"src/main.cpp",
	"src/TetrisX.cpp"
};

void startup()
{
#ifdef MULTIVERSE_SOURCE_DIR
	const std::string workingDirectory = MULTIVERSE_SOURCE_DIR;
	
	// Check if source files exist in the development directory
	bool sourceFilesExist = true;
	for (const std::string& file : files)
	{
		std::string fullPath = workingDirectory + "/" + file;
		if (!std::filesystem::exists(fullPath))
		{
			sourceFilesExist = false;
			break;
		}
	}
	
	if (!sourceFilesExist)
	{
		std::cerr << "Error: TetrisX source files not found in development directory." << std::endl;
		std::cerr << "This feature requires the development source files." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}
#else
	const std::string workingDirectory = std::filesystem::current_path().string();
	
	// In installed mode, check if source files exist locally
	bool sourceFilesExist = true;
	for (const std::string& file : files)
	{
		if (!std::filesystem::exists(file))
		{
			sourceFilesExist = false;
			break;
		}
	}
	
	if (!sourceFilesExist)
	{
		std::cerr << "Error: TetrisX source files not found." << std::endl;
		std::cerr << "Please run this from the project root directory or reinstall from source." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;
	}
#endif

	std::string compileCommand = "g++ -std=c++20 -O2";
	for (const std::string& file : files)
	{
		compileCommand += " \"" + file + "\"";
	}

#if defined(__linux__)
	compileCommand =
		"cd \"" + workingDirectory + "\" && " + compileCommand +
		" -o tetris && ./tetris";
	const std::string terminalCommand =
		"x-terminal-emulator -e sh -c '" + compileCommand + "'";
	std::system(terminalCommand.c_str());
#elif defined(_WIN32)
	compileCommand =
		"cd /d \"" + workingDirectory + "\" && " + compileCommand +
		" -o tetris.exe && tetris.exe";
	std::string terminalCommand =
		"start \"\" cmd /C \"\"" + compileCommand + "\"";
	std::system(terminalCommand.c_str());
#endif
}