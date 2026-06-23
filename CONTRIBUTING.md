# 🌌 Contributing to THE MULTIVERSE

Thank you for checking out THE MULTIVERSE! This is an open-source, console-based C++ game collection. We welcome developers of all skill levels to help us expand the universe.

🚀 **Our Current Goal:** We are actively pushing to get **v1.1.0-stable launched by July 10!** 

---

## 🎮 How You Can Help

You can contribute to the project in several ways:
* **Add a Mini-Game:** Write a new terminal-based text game or story section.
* **Fix Bugs:** Review our open issues and submit code fixes.
* **Code Quality:** Help optimize our CMAKE setup or clean up existing code.
* **Test the Beta:** Run the game on your system and report any bugs you find.

---

## 🛠️ Development Setup

To build and run the project locally, ensure you have a C++ compiler and CMAKE installed.

1. **Fork the repository** to your own GitHub account.
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/owenb135/THE-MULTIVERSE
   cd THE-MULTIVERSE
   ```
3. **Build the project:**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
4. **Run the application:**
   ```bash
   ./MultiverseApp
   ```

---

## 📝 Code Guidelines

To keep the codebase organized for the v1.1.0 release, please follow these rules:
* **C++11 or Higher:** Ensure your code compiles with standard C++11 features.
* **Multi-File Architecture:** Keep your game logic modular. Do not dump all your code into `multi.cpp`. Use separate files (like `reptor-tasks.cpp`) and link them.
* **Case Sensitivity:** Document whether user text inputs are case-sensitive.

---

## 🚀 Submitting Your Changes

1. Create a new branch for your feature:
   ```bash
   git checkout -b feature/my-new-game
   ```
2. Commit your changes with a clear message:
   ```bash
   git commit -m "Add text-based adventure mini-game"
   ```
3. Push to your branch:
   ```bash
   git push origin feature/my-new-game
   ```
4. Open a **Pull Request (PR)** against our `main` branch. 

*Note: Please check the GitHub Discussions page before starting on a game to ensure no one else is already building the same idea!*

---

## 💬 Questions?

If you want to pitch a game idea before writing code, jump right into our **GitHub Discussions** tab and start a thread. Let's build something awesome together!
