#include "personality_modules.h"
#include "core.h"

#include <chrono>
#include <cctype>
#include <iostream>
#include <string>
#include <thread>

void Users() {

  std::string user;
  std::cout << "Please enter your username:\n";
  std::cin >> user;
  if (lowercase(user) == "Owen2024dj") { // You can reject this if you want your style (?)
    std::cout << "Welcome Owen, Do you want to game?\n";
    std::string y_n;
    // This is for if they want to game.
    std::cin >> y_n;
    //if (y_n == "y" || y_n == "yes" || y_n == "Yes") {
    if(lowercase(y_n) == "y") { // "yeah, actually no" also yes btw
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
  if (lowercase(user) == "eli") {
    std::cout << "Welcome Eli do you want to play mathplayground?\n";
    char yorn;
    std::cout << "Select y or n\n";
    std::cin >> yorn;
    if (tolower(yorn) == 'y') {
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
  if (user == "braden" || user == "Bradll") { // Case Sensitive?
    std::cout << "Hello and welcome...\n";
    std::cout << "Would you a fact?\n";
    std::cout << "Select y or n\n";
    std::string q1;
    std::cin >> q1;
    if (tolower(q1[0]) == 'y') {
      std::cout << "Did you know that the moon is lemon shaped?\n";
      std::cout << "y or n\n";
      std::string q2;
      std::cin >> q2;
      if (tolower(q2[0]) == 'y') {
        std::cout << "I knew you know\n";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(4000ms); // Sleep for 100 milliseconds
      }
      if (tolower(q2[0]) == 'n') {
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

