#include "audio.h"

#include <filesystem>
#include <iostream>
#include <vector>

#ifndef NOSOUND
bool playTrack(sf::Music& music, const std::string& filename) {
    music.stop();

    std::vector<std::filesystem::path> musicDirectories = {
        "/usr/share/TheMultiverse/music"
    };

#if defined(__linux__)
    std::error_code error;
    const auto executablePath = std::filesystem::read_symlink("/proc/self/exe", error);
    if (!error) {
        const auto executableDirectory = executablePath.parent_path();
        musicDirectories.push_back(executableDirectory / "../share/TheMultiverse/music");
    }
#endif

#ifdef MULTIVERSE_SOURCE_DIR
    musicDirectories.push_back(
        std::filesystem::path(MULTIVERSE_SOURCE_DIR) / "rsc/music");
#endif
    musicDirectories.push_back(std::filesystem::current_path() / "rsc/music");

    for (const auto& directory : musicDirectories) {
        const auto trackPath = directory / filename;
        std::error_code error;
        if (!std::filesystem::is_regular_file(trackPath, error)) {
            continue;
        }
        if (music.openFromFile(trackPath.string())) {
            music.play();
            return true;
        }
        std::cerr << "Failed to open audio track: " << trackPath << '\n';
        return false;
    }

    std::cerr << "Failed to find audio track: " << filename
              << " (checked installed, executable-relative, and source music directories)\n";
    return false;
}
#else
bool playTrack(sf::Music&, const std::string&) {
    return true;
}
#endif
