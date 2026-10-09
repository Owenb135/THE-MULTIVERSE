#include "menu.h"

#include <iostream>

void showMenu() {

  std::cout << "┌─────────────────────────────────────────┐\n";
  std::cout << "│          SELECT A PROGRAM MODULE        │\n";
  std::cout << "├─────────────────────────────────────────┤\n";
  std::cout << "│  [1] Guessing Game                      │\n";
  std::cout << "│  [2] User Verification                  │\n";
  std::cout << "│  [3] Eli's Crazy Program                │\n";
  std::cout << "│  [4] Tyler's Module                     │\n";
  std::cout << "│  [5] Runtime System Error Sandbox       │\n";
  std::cout << "│  [6] Jason's Sandbox                    │\n";
  std::cout << "│  [7] Text-Based RPG Engine              │\n";
  std::cout << "│  [8] Reptor Tasks                       │\n";
  std::cout << "│  [9] Coder's Ttype                      │\n";
  std::cout << "│  [10] TetrisX                           │\n";
  std::cout << "│  [0] Exit Program                       │\n";
  std::cout << "└─────────────────────────────────────────┘\n\n";
  std::cout << "Enter selection index: ";
}
