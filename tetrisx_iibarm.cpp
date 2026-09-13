///Created by Owen0963///
#include <array>
#include <cstdlib>
#include <filesystem>
#include <string>
using namespace std;
const std::array<std::string, 2> files = {
	"src/main.cpp",
	"src/TetrisX.cpp"
};

void startup()
{
#ifdef MULTIVERSE_SOURCE_DIR
	const std::string workingDirectory = MULTIVERSE_SOURCE_DIR;
#else
	const std::string workingDirectory =
		std::filesystem::current_path().string();
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