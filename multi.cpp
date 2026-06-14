#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
int code;
void rpg_game();
void r10();
void Guessing_game() {
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
void gamer() {
  std::cout << "Welcome to the newest and best game I made...\n";
  std::this_thread::sleep_for(std::chrono::seconds(3));
  rpg_game();
}
int main() {
  // Write C++ code here
  int game;

  // Welcome Message
  std::cout
      << R"( __    __     _                            _          _   _                           __ __________         __  __  __    __ 
/ / /\ \ \___| | ___ ___  _ __ ___   ___  | |_ ___   | |_| |__   ___    /\/\  /\ /\  / //__   \_   \/\   /\/__\/__\/ _\  /__\
\ \/  \/ / _ \ |/ __/ _ \| '_ ` _ \ / _ \ | __/ _ \  | __| '_ \ / _ \  /    \/ / \ \/ /   / /\// /\/\ \ / /_\ / \//\ \  /_\
 \  /\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |_| | | |  __/ / /\/\ \ \_/ / /___/ //\/ /_   \ V //__/ _  \_\ \//__
  \/  \/ \___|_|\___\___/|_| |_| |_|\___|  \__\___/   \__|_| |_|\___| \/    \/\___/\____/\/ \____/    \_/\__/\/ \_/\__/\__/
)" << std::endl;

  // Keep these as text for contrast to the title
  std::cout << "This program consists with lots of games\n";
  std::cout << "\n REMEMBER THAT THIS PROGRAM IS BETA\n";
  std::cout << "CREATED BY OWENB135/OWEN0963\n";
  std::cout
      << "Remember that this program doesn't use GUI, so type instead.\n\n";

  using namespace std::chrono_literals;
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
  std::cout << "└────────────────────────────────────────┘\n\n";
  std::cout << "Enter selection index: ";
  std::cin >> game;
  if (game == 1) {
    system("clear");
    std::cout << "Starting guessing game...\n";
    Guessing_game();
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
    gamer();  
  } else if (game == 8) {
    system("clear");
    r10();
  } else {
    std::cout << "Invalid selection. Exiting program.\n";
  }
}
