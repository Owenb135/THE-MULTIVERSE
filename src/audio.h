#pragma once

#include <string>

#ifndef NOSOUND
#include <SFML/Audio.hpp>
#else
namespace sf {
class Music {
public:
    void setLoop(bool) {}
};
} // namespace sf
#endif

bool playTrack(sf::Music& music, const std::string& filename);
