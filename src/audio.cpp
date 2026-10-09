#include "audio.h"

#include <iostream>

#ifndef NOSOUND
bool playTrack(sf::Music& music, const std::string& filename) {
    music.stop();

    if (music.openFromFile("/usr/share/TheMultiverse/music/" + filename)) {
        music.play();
        return true;
    }

    if (music.openFromFile("rsc/music/" + filename)) {
        music.play();
        return true;
    }

    std::cerr << "Failed to find audio track: " << filename << '\n';
    return false;
}
#else
bool playTrack(sf::Music&, const std::string&) {
    return true;
}
#endif
