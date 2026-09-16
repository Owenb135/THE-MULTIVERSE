[![li](https://uxwing.com/wp-content/themes/uxwing/download/brands-and-social-media/github-icon.png)](https://github.com/owenb135)
[![logo](https://img.shields.io/badge/DOWNLOAD%20-brightgreen?style=for-the-badge)](https://github.com/Owenb135/THE-MULTIVERSE/releases)
[![How to run](https://img.shields.io/badge/How%20to%20run-brightgreen?style=for-the-badge)](#how-to-run)
[![How to run](https://img.shields.io/badge/Add%20your%20game!-brightgreen?style=for-the-badge)](#add-your-game)
[![Latest windows release](https://img.shields.io/badge/Latest%20Windows%20Release-brightgreen?style=for-the-badge)](https://github.com/Owenb135/THE-MULTIVERSE/releases/tag/v1.3.7-w) 
# 🌌 THE MULTIVERSE (C++ Project)

Welcome to **THE MULTIVERSE**, a console-based C++ game collection created as a fun open source project.  
It includes multiple mini-games, interactive story sections, and simple RPG-style battles.

---

## 📌 About the Project

This project is a **stable-stage multi-game program** built in C++.  
It runs in the terminal and uses text input/output for gameplay.

The goal of this project is to:
- Practice C++ programming
- Build simple game logic systems
- Experiment with interactive storytelling
- Learn structuring larger programs

---

## Features

Menu modules currently available in `multi.cpp`:

- **[1] Guessing Game** — number-guessing mini-game with basic RNG
- **[2] User Verification** — user-specific interactive prompts
- **[3] Eli's Crazy Program** — character dialogue / story sandbox
- **[4] Tyler's Module** — interactive text module
- **[5] Runtime System Error Sandbox** — error / sandbox demo
- **[6] Jason's Sandbox** — additional sandbox module
- **[7] Text-Based RPG Engine** — RPG-style battle flow (`rpg_game`)
- **[8] Reptor Tasks** — task mini-game module
- **[9] Coder's Ttype** — typing-oriented module
- **[10] TetrisX** - A Tetris game created by [@iibram](https://github.com/iibram) and upgraded by [@Owenb135](https://github.com/Owenb135)

Cross-cutting:

- Multi-module selection menu at startup
- Background music via SFML (`rsc/music/`)
- Optional automatic update check from the release manifest (beta)


---

## How to Run

### Simple way
1. Download the latest release from [GitHub Releases](https://github.com/Owenb135/THE-MULTIVERSE/releases)
2. For Ubuntu/Debian, install the `.deb` package
3. For Windows, just run the latest release `.exe` file

### Advanced way (optional)
1. Make a folder named build
2. Run `cd build`
3. Run `cmake .. && make`
4. Run `./MultiverseApp` or `MultiverseApp.exe` if on Windows

### Build requirements (optional)
These are only needed if you build from source. Windows users who download a release `.exe` do not need to install these dependencies.
- C++ compiler (g++, clang++, or Visual Studio)
- C++11 or higher
- CMake
- SFML development libraries for audio

### Linux package install example
For Ubuntu/Debian:
```bash
sudo apt update
sudo apt install build-essential cmake libsfml-dev
```

### Windows release usage
For Windows users, no build is required.
1. Download the latest release from [GitHub Releases](https://github.com/Owenb135/THE-MULTIVERSE/releases)
2. Run the latest `.exe` file from the release

### Advanced way (optional)
If you want to build from source instead of using a release executable:
1. Create a folder named `build`
2. Run `cd build`
3. Run `cmake .. && make`
4. Run `./MultiverseApp` or `MultiverseApp.exe` on Windows

---
## Add Your Game
If you would like to add you game go to the [games](https://github.com/Owenb135/THE-MULTIVERSE/discussions/7) disscussion to talk about it
## ⚠️ Notes
Input is case-sensitive in some areas.
The program is text-based (no GUI).

---
## Contrubutors:
<a href="https://github.com/owenb135/THE-MULTIVERSE/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=owenb135/THE-MULTIVERSE" alt="Contributors" />
</a>



## Credit:
[Patorjk's TAAG Tool](https://patorjk.com/software/taag/) for ASCII text generation

[@iibram](http://www.github.com/iibram) for the making of TetrisX

## 👨‍💻 Author

- "I have fun with coding because I can turn my imagination into reality."
     -Owen
---
## 🚀 Future Ideas
- Save system
- Better combat mechanics
- Improved AI system
- Cleaner UI/menu system
- Better Code quility
## Legal
(c) 2026 Owenb135


This project is licensed under Apache 2.0.


TetrisX is a project licensed under MIT.

