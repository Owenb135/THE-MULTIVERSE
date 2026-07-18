#include <SFML/Audio.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
namespace {
    using namespace std::chrono_literals;
    std::string CURRENT_VERSION = "1.3.0";
    std::string& get_version() { return CURRENT_VERSION; }

    // 2. Clear path pointing directly to your live manifest file on GitHub
    static const std::string MANIFEST_URL = "https://raw.githubusercontent.com/Owenb135/THE-MULTIVERSE/10-feat-automatic-updates/upds/manifest.json";

    // Lightweight inline string locator to extract JSON values without heavy external libraries
    std::string parse_json_value(const std::string& json, const std::string& key) {
        size_t pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos) return "";
        pos = json.find(":", pos);
        size_t start = json.find("\"", pos);
        size_t end = json.find("\"", start + 1);
        if (start == std::string::npos || end == std::string::npos) return "";
        return json.substr(start + 1, end - start - 1);
    }

    void handle_automatic_updates() {
        std::cout << "[Updater] Connecting to GitHub update stream...\n";

        // Download manifest.json from your branch
        std::string downloadCmd = "wget -q \"" + MANIFEST_URL + "\" -O /tmp/manifest.json";
        int status = system(downloadCmd.c_str());

        if (status != 0) {
            std::cout << "[Updater] Unable to reach GitHub updates. Launching offline mode.\n\n";
            return;
        }

        // Read the entire file into a C++ string stream
        std::ifstream file("/tmp/manifest.json");
        if (!file.is_open()) return;
        std::string manifest((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Extract key pieces of information from the JSON string
        std::string latest_version = parse_json_value(manifest, "latest_version");
        std::string release_notes = parse_json_value(manifest, "release_notes");

        // Clean up the temp manifest file
        std::remove("/tmp/manifest.json");

        // 3. Compare the version numbers!
        if (latest_version != CURRENT_VERSION && !latest_version.empty()) {
            std::cout << "\n=============================================\n";
            std::cout << "UPDATE AVAILABLE: New version v" << latest_version << " is ready!\n";
            if (!release_notes.empty()) {
                std::cout << "Changelog: " << release_notes << "\n";
            }
            std::cout << "=============================================\n";
            std::cout << "Would you like to download and install this update? (y/n): ";

            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y')
            {
              std::string platform_key = "ubuntu_deb";

#if defined(_WIN32)
              platform_key = "windows_exe";
#endif

              // Find the selected platform data block inside the manifest string
              size_t plat_pos = manifest.find("\"" + platform_key + "\"");
              if (plat_pos == std::string::npos) {
                std::cout << "[Error] Platform settings missing from manifest. Launching game...\n\n";
                return;
              }
              std::string plat_block = manifest.substr(plat_pos, manifest.find("}", plat_pos) - plat_pos);

              std::string download_url = parse_json_value(plat_block, "url");
              std::string filename = parse_json_value(plat_block, "filename");

              if (download_url.empty() || filename.empty()) {
                std::cout << "[Error] Failed to read download metadata from manifest. Launching game...\n\n";
                return;
              }

              std::cout << "[Updater] Streaming package download via curl...\n";
              std::string download_cmd = "curl -L \"" + download_url + "\" -o /tmp/" + filename;
              int progress_res = system(download_cmd.c_str());

              if (progress_res != 0) {
                std::cout << "[Error] Download failed. Launching engine offline...\n\n";
                return;
              }

              CURRENT_VERSION = latest_version;
              std::cout << "[Updater] Version updated to v" << CURRENT_VERSION << "\n";
              std::cout << "[Updater] Launching background platform installer and closing game...\n";

#if defined(_WIN32)
              // Windows: Run the installer executable silently
              std::ofstream batch("apply_update.bat");
              batch << "@echo off\n"
                    << "timeout /t 1 /nobreak > nul\n"
                    << "start /wait /tmp\\" << filename << " /SILENT\n"
                    << "del \"%~f0\"\n";
              batch.close();
              system("start /b apply_update.bat");
              exit(0);
#else
              // Ubuntu Linux: Installs the downloaded .deb package cleanly
              std::ofstream sh("/tmp/apply_update.sh");
              sh << "#!/bin/bash\n"
                 << "sleep 0.5\n"
                 << "touch /tmp/update_started.flag\n"
                 << "pkexec env DEBIAN_FRONTEND=noninteractive apt-get install -y --fix-broken /tmp/" << filename << " > /tmp/update_log.txt 2>&1\n"
                 << "rm -f /tmp/update_started.flag\n"
                 << "rm -- \"$0\"\n";
              sh.close();

              std::remove("/tmp/update_started.flag");
              system("chmod +x /tmp/apply_update.sh");

              // Use 'nohup' so the script survives completely independent of the game closing!
              system("nohup /tmp/apply_update.sh >/dev/null 2>&1 &");

              std::cout << "[Updater] Waiting for authentication terminal hook...\n";

              int timeout_counter = 0;
              while (!std::ifstream("/tmp/update_started.flag") && timeout_counter < 30) {
                  std::this_thread::sleep_for(100ms);
                  timeout_counter++;
              }

              std::this_thread::sleep_for(500ms);
              exit(0);
#endif
            }
        } else {
            std::cout << "[Updater] Version check passed. Core build (v" << CURRENT_VERSION << ") is active.\n\n";
        }
        std::this_thread::sleep_for(3000ms);
        system("clear");
    }
} // namespace

int code;
void rpg_game();
void r10();
int CODERS_TTYPE();
bool playTrack(sf::Music& music, const std::string& filename) {
  // 1. FORCE the old track to stop playing immediately
  music.stop();

  // 2. Try loading from Ubuntu installed directory first
  if (music.openFromFile("/usr/share/TheMultiverse/music/" + filename)) {
    music.play();
    return true;
  }
  // 3. Fall back to local development directory
  if (music.openFromFile("rsc/music/" + filename)) {
    music.play();
    return true;
  }
  std::cerr << "Failed to find audio track: " << filename << std::endl;
  return false;
}

void r11(sf::Music& bgMusic) {
  bgMusic.setLoop(true);
  playTrack(bgMusic, "watermello-phonk-phonk-music.mp3");
  r10();
}
void Guessing_game(sf::Music& bgMusic) {
  bgMusic.setLoop(true);
  playTrack(bgMusic, "watermello-phonk-phonk-music.mp3");
  std::cout << "Hello and welcome to the program!\n";
  std::cout << "This is v.1.2\n";
  int pass = 0;
  while (true) {
    std::cout << " \n \n \n \n Hello! Please enter the 4 character code...\n";
    std::cin >> pass;
    if (pass == 7699) {
      std::cout << "Good job you did it!\n";
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
      break;
    } else {
      std::cout << "Try again\n";
      std::this_thread::sleep_for(std::chrono::seconds(2));
      system("clear");
    }
  }
}
void Users() {

  std::string user;
  std::cout << "Please enter your username:\n";
  std::cin >> user;
  if (user == "Owen2024dj" || user == "owen2024dj") {
    std::cout << "Welcome Owen, Do you want to game?\n";
    std::string y_n;
    // This is for if they want to game.
    std::cin >> y_n;
    if (y_n == "y" || y_n == "yes" || y_n == "Yes") {
      std::cout << "Then go to www.roblox.com or Minecraft.net\n";
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
    }

    else {
      std::cout << "WHAT'S WRONG WITH YOU!!!!\n";
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
    }
  }
  if (user == "Eli" || user == "eli") {
    std::cout << "Welcome Eli do you want to play mathplayground?\n";
    std::string yorn;
    std::cout << "Select y or n\n";
    std::cin >> yorn;
    if (yorn == "Y" || yorn == "y") {
      std::cout << "Then copy and paste this in your browser:\n";
      std::cout << "https://www.mathplayground.com/\n";
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
    } else {
      std::cout << "WHAT'S WRONG WITH YOU!!!\n";
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
    }
  }
  if (user == "braden" || user == "Braden") {
    std::cout << "Hello and welcome...\n";
    std::cout << "Would you a fact?\n";
    std::cout << "Select y or n\n";
    std::string q1;
    std::cin >> q1;
    if (q1 == "y" || q1 == "Y") {
      std::cout << "Did you know that the moon is lemon shaped?\n";
      std::cout << "y or n\n";
      std::string q2;
      std::cin >> q2;
      if (q2 == "y" || q2 == "Y") {
        std::cout << "I knew you know\n";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
      }
      if (q2 == "n" || q2 == "N") {
        std::cout << "I am surprised\n";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
      }
    }
  }
}
void eli() {
  std::string eli;
  std::cout << "HI elIi WE;llcome myoto thhhe ggamem!1/\n";
  std::cout << "Warning do not type ELI!!!!\n";
  std::cin >> eli;
  if (eli == "ELI" || eli == "eli" || eli == "Eli") {
    std::cout << "Sttoop TtHhhis Nnow wwwwnooowwww.1.1.1.1.1.1.1.1!\n";
    std::cout << "WWWWARNNINNNG\n";
    std::cout << "Ending software issuennnnn!\n";
    std::cout << "Enter a response yes or no(y or n)\n";
    std::string response;
    std::cin >> response;
    if (response == "y" || response == "Y") {
      std::cout << "Software ended\n";
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
    } else {
      std::cout << "Syetem error....\n";
      std::cout << "WARNING THE FDA HAS ISSUED THIS PROGRAM WILL EXPLODE THE "
                   "UNIVERSE IF YOU ARE STILL ON IT. WARNING,,WARNING!!\n";
      std::cout << "DO NOT PRESIST DO NOT PRESIST!\n";
      std::this_thread::sleep_for(std::chrono::seconds(5));
      std::cout << "ENTER SHUTDOWN CODE:\n";
      int code;
      std::cin >> code;
      if (code == 8501) {
        std::cout << "HAHAHHHBJB YYOOU FAIIOLLLED BBYE BYEEE "
                     "HAHAHAHHAHAHHAHAHAH@!!!\n";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
      }
    }
  }
}
void tyler() {
    std::string tyler;
    std::cout << "Hi tyler don't type NUKE\n";
    std::cin >> tyler;

    if (tyler == "NUKE" || tyler == "nuke" || tyler == "Nuke") {
        std::cout << "MAHHHHAHAHHA I WILLLL hhhAUNtt yoiu fororrrevr HHAHHAHAHHAHAHAHHA\n";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(4000ms);
    } else {
        std::cout << "Good boy\n";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(4000ms);
    }
}

void why() {
  std::cout << "Why why why does no one use this\n";
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
}
void jason() {
  std::cout << "Weeelcome Jaaason tooo theeee pproggram...\n";
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << " Would you like to continue?\n";
  std::string inpu53;
  std::cin >> inpu53;
  if (inpu53 == "Yes" || inpu53 == "yes" || inpu53 == "y" || inpu53 == "Y" ||
      inpu53 == "Sure" || inpu53 == "sure") {
    std::cout << "Ok then lets continue\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Do you like the color blue? Yes or no\n";
    std::string inpu54;
    std::cin >> inpu54;
    if (inpu54 == "Yes" || inpu54 == "yes" || inpu54 == "y" || inpu54 == "Y" ||
        inpu54 == "Sure" || inpu54 == "sure") {
      std::cout << "I guess you are not like 5 perecters\n";
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Good bye!\n";
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
  }
}
void gamer(sf::Music& bgMusic) {
  bgMusic.setLoop(true);
  playTrack(bgMusic, "watermello-phonk-phonk-music.mp3");
  std::cout << "Welcome to the newest and best game I made...\n";
  std::this_thread::sleep_for(std::chrono::seconds(3));
  rpg_game();
}
int main() {
  using namespace std::chrono_literals;
  handle_automatic_updates();
  int game;
  sf::Music bgMusic;
bgMusic.setLoop(true);

// Example 1: Play hiphop track
playTrack(bgMusic, "kontraa-no-sleep-hiphop-music.mp3");

// Example 2: Switch to phonk for a boss/game battle later

  // Welcome Message
  std::cout
      << R"( __    __     _                            _          _   _                           __ __________         __  __  __    __
/ / /\ \ \___| | ___ ___  _ __ ___   ___  | |_ ___   | |_| |__   ___    /\/\  /\ /\  / //__   \_   \/\   /\/__\/__\/ _\  /__\
\ \/  \/ / _ \ |/ __/ _ \| '_ ` _ \ / _ \ | __/ _ \  | __| '_ \ / _ \  /    \/ / \ \/ /   / /\// /\/\ \ / /_\ / \//\ \  /_\
 \  /\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |_| | | |  __/ / /\/\ \ \_/ / /___/ //\/ /_   \ V //__/ _  \_\ \//__
  \/  \/ \___|_|\___\___/|_| |_| |_|\___|  \__\___/   \__|_| |_|\___| \/    \/\___/\____/\/ \____/    \_/\__/\/ \_/\__/\__/
)" << '\n';
  std::cout << "VERSION: "<< CURRENT_VERSION << '\n';
  std::this_thread::sleep_for(5s);
  system("clear");
  // Keep these as text for contrast to the title
  std::cout << "This program consists with lots of games\n";
  std::cout << "\n REMEMBER THAT THIS PROGRAM IS BETA\n";
  std::cout << "CREATED BY OWENB135/OWEN0963\n";
  std::cout
      << "Remember that this program doesn't use GUI, so type instead.\n\n";

  std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
  system("clear");

  // Selection table
  std::cout << "┌────────────────────────────────────────┐\n";
  std::cout << "│          SELECT A PROGRAM MODULE       │\n";
  std::cout << "├────────────────────────────────────────┤\n";
  std::cout << "│  [1] Guessing Game                     │\n";
  std::cout << "│  [2] User Verification                 │\n";
  std::cout << "│  [3] Eli's Crazy Program               │\n";
  std::cout << "│  [4] Tyler's Module                    │\n";
  std::cout << "│  [5] Runtime System Error Sandbox      │\n";
  std::cout << "│  [6] Jason's Sandbox                   │\n";
  std::cout << "│  [7] Text-Based RPG Engine             │\n";
  std::cout << "│  [8] Reptor Tasks                      │\n";
  std::cout << "│  [9] Coder's Ttype                     │\n";
  std::cout << "└────────────────────────────────────────┘\n\n";
  std::cout << "Enter selection index: ";
  std::cin >> game;
  if (game == 1) {
    system("clear");
    std::cout << "Starting guessing game...\n";
    Guessing_game(bgMusic);
  } else if (game == 2) {
    system("clear");
    Users();
  } else if (game == 3) {
    system("clear");
    eli();
  } else if (game == 4) {
    system("clear");
    tyler();
  } else if (game == 5) {
    system("clear");
    why();
  } else if (game == 6) {
    system("clear");
    jason();
  } else if (game == 7) {
    system("clear");
    gamer(bgMusic);
  } else if (game == 8) {
    system("clear");
    r11(bgMusic);
  } else if (game == 9) {
    system("clear");
    CODERS_TTYPE();
  } else {
    std::cout << "Invalid selection. Exiting program.\n";
  }
}
