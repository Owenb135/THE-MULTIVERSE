#include "guessing_game.h"
#include "core.h"

#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <thread>

void Guessing_game(sf::Music& bgMusic) {
  bgMusic.setLoop(true);
  playTrack(bgMusic, "watermello-phonk-phonk-music.mp3");

  std::random_device rd;
  std::mt19937 gen(rd());

  while (true) {
    clear_screen();
    std::cout << "┌─────────────────────────────────────────┐\n";
    std::cout << "│              GUESSING GAME              │\n";
    std::cout << "├─────────────────────────────────────────┤\n";
    std::cout << "│  Select Difficulty / Mode:              │\n";
    std::cout << "│  [1] Easy   (1 - 50)                    │\n";
    std::cout << "│  [2] Medium (1 - 100)                   │\n";
    std::cout << "│  [3] Hard   (1 - 500)                   │\n";
    std::cout << "│  [4] 4-Digit Code (1000 - 9999)         │\n";
    std::cout << "│  [0] Return to Main Menu                │\n";
    std::cout << "└─────────────────────────────────────────┘\n\n";
    std::cout << "Enter selection index: ";

    int choice;
    if (!(std::cin >> choice)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "Invalid selection. Please choose an option from the menu.\n";
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    if (choice == 0) {
      break;
    }

    int minVal = 1;
    int maxVal = 100;

    if (choice == 1) {
      minVal = 1;
      maxVal = 50;
    } else if (choice == 2) {
      minVal = 1;
      maxVal = 100;
    } else if (choice == 3) {
      minVal = 1;
      maxVal = 500;
    } else if (choice == 4) {
      minVal = 1000;
      maxVal = 9999;
    } else {
      std::cout << "Invalid selection. Please choose a valid difficulty level (0-4).\n";
      std::this_thread::sleep_for(std::chrono::seconds(1));
      continue;
    }

    std::uniform_int_distribution<int> dist(minVal, maxVal);
    int secret = dist(gen);
    int attempts = 0;
    int guess = 0;

    clear_screen();
    std::cout << "┌─────────────────────────────────────────┐\n";
    std::cout << "│              GUESSING GAME              │\n";
    std::cout << "└─────────────────────────────────────────┘\n\n";
    if (choice == 4) {
      std::cout << "A secret 4-digit code between 1000 and 9999 has been generated!\n";
    } else {
      std::cout << "I have chosen a secret number between " << minVal << " and " << maxVal << ".\n";
    }
    std::cout << "Try to guess it in as few attempts as possible!\n";
    std::cout << "(Enter 0 at any time to give up)\n\n";

    while (true) {
      std::cout << "Enter your guess: ";
      if (!(std::cin >> guess)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a valid number.\n\n";
        continue;
      }

      if (guess == 0) {
        std::cout << "\nYou gave up! The secret number was: " << secret << "\n\n";
        break;
      }

      if (guess < minVal || guess > maxVal) {
        std::cout << "Out of range! Please enter a number between " << minVal << " and " << maxVal << ".\n\n";
        continue;
      }

      attempts++;

      if (guess < secret) {
        std::cout << "Too low! Try again.\n\n";
      } else if (guess > secret) {
        std::cout << "Too high! Try again.\n\n";
      } else {
        std::cout << "\nGood job, you did it!\n";
        std::cout << "You guessed the secret number (" << secret << ") in " << attempts;
        if (attempts == 1) {
          std::cout << " attempt! First try, incredible!\n\n";
        } else {
          std::cout << " attempts!\n\n";
        }
        break;
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Would you like to play again? (y/n): ";
    std::string question;
    std::cin >> question;
    std::string q = lowercase(question);
    if (q == "yes" || q == "y" || q == "sure" || q == "yeah" || q == "okay" || q == "ok" || q == "s") {
      continue;
    } else {
      std::cout << "Thanks for playing! Returning to menu...\n";
      std::this_thread::sleep_for(std::chrono::seconds(1));
      break;
    }
  }
}

