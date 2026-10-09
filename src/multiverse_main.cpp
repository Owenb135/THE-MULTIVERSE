#include "audio.h"
#include "core.h"
#include "external_modules.h"
#include "game_modules.h"
#include "guessing_game.h"
#include "menu.h"
#include "personality_modules.h"
#include "updater.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <thread>

int main() {
    using namespace std::chrono_literals;

    handle_automatic_updates();

    sf::Music bgMusic;
    bgMusic.setLoop(true);
    playTrack(bgMusic, "kontraa-no-sleep-hiphop-music.mp3");

    std::cout
        << R"( __    __     _                            _          _   _                           __ __________         __  __  __    __
 / / /\ \ \___| | ___ ___  _ __ ___   ___  | |_  ___   | |_| |__   ___    /\/\  /\ /\  / //__   \_   \/\   /\/__\/__\/ _\  /__\
 \ \/  \/ / _ \ |/ __/ _ \| '_ ` _ \ / _ \ | __/ _ \  | __| '_ \ / _ \  /    \/ / \ \/ /   / /\// /\/\ \ / /_\ / \//\ \  /_\
  \  /\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |_| | | |  __/ / /\/\ \ \_/ / /___/ //\/ /_   \ V //__/ _  \_\ \//__
   \/  \/ \___|_|\___\___/|_| |_| |_|\___|  \__\___/   \__|_| |_|\___| \/    \/\___/\____/\/ \____/    \_/\__/\/ \_/\__/\__/
)" << '\n';

    std::cout << "VERSION: " << get_version() << '\n';
    std::this_thread::sleep_for(5s);
    clear_screen();
    std::cout << "This program consists with lots of games\n";
    std::cout << "CREATED BY OWENB135/OWEN0963\n";
    std::cout << "Remember that this program doesn't use GUI, so type instead.\n\n";
    std::this_thread::sleep_for(4000ms);
    clear_screen();

    while (true) {
        int game;
        showMenu();

        if (!(std::cin >> game)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clear_screen();
            std::cout << "Invalid selection. Please try again.\n";
            std::this_thread::sleep_for(2s);
            clear_screen();
            continue;
        }

        if (game == 1) {
            clear_screen();
            std::cout << "Starting guessing game...\n";
            Guessing_game(bgMusic);
            clear_screen();
        } else if (game == 2) {
            clear_screen();
            Users();
            clear_screen();
        } else if (game == 3) {
            clear_screen();
            eli();
            clear_screen();
        } else if (game == 4) {
            clear_screen();
            tyler();
            clear_screen();
        } else if (game == 5) {
            clear_screen();
            why();
            clear_screen();
        } else if (game == 6) {
            clear_screen();
            jason();
            clear_screen();
        } else if (game == 7) {
            clear_screen();
            gamer(bgMusic);
            clear_screen();
        } else if (game == 8) {
            clear_screen();
            r11(bgMusic);
            clear_screen();
        } else if (game == 9) {
            clear_screen();
            CODERS_TTYPE();
            clear_screen();
        } else if (game == 10) {
            clear_screen();
            startup();
            clear_screen();
        } else if (game == 0) {
            std::cout << "Thanks for playing! Goodbye!\n";
            std::this_thread::sleep_for(1s);
            break;
        } else {
            clear_screen();
            std::cout << "Invalid selection. Please try again.\n";
            std::this_thread::sleep_for(2s);
            clear_screen();
        }
    }

    return 0;
}
