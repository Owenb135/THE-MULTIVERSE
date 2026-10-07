#include "game_modules.h"
#include "audio.h"
#include "external_modules.h"

#include <chrono>
#include <iostream>
#include <thread>

void rpg_game();

void r11(sf::Music& bgMusic) {
    bgMusic.setLoop(true);
    playTrack(bgMusic, "watermello-phonk-phonk-music.mp3");
    r10();
}

void gamer(sf::Music& bgMusic) {
    bgMusic.setLoop(true);
    playTrack(bgMusic, "watermello-phonk-phonk-music.mp3");
    std::cout << "Welcome to the newest and best game I made...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    rpg_game();
}
