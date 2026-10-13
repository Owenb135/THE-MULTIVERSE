# Changes

Drop these files over the same paths in your project (they keep your CRLF line endings).

## TetrisX (src/TetrisX.cpp, src/TetrisX.h, src/AsyncKey.h, tetrisx_iibarm.cpp, CMakeLists.txt)
- **Line-clear bug:** clearing non-adjacent rows with one piece (e.g. rows 19 and 16) left a solid 💢 row behind.
  `completion_3()` now compacts the board row by row, so it works for any combination of cleared rows.
- **New game didn't fully reset:** `line_cnt` (level progress), animation state and counters carried over.
- **Stray keys:** on Linux, an ENTER pressed during play instantly restarted the game after game over. Input is now flushed.
- **100% CPU:** the main loop busy-waited. It now sleeps 2 ms between polls and caps catch-up after a suspend.
- **Compiler compatibility:** removed the `std::format` dependency (needed `<format>`, i.e. GCC 13+). Added missing includes.
- **Windows:** ANSI escape processing is enabled explicitly (needed on classic conhost). Not tested on Windows.
- **Menu option 10 did nothing on Ubuntu:** `startup()` tried to compile TetrisX's source with g++ in a new terminal
  at runtime. On packaged builds the source isn't there, and without `x-terminal-emulator` it also failed silently.
  TetrisX is now compiled into the app like Snake (CMakeLists.txt: C++20 + `src/TetrisX.cpp`; `startup()` just runs the game).
  Needs g++ 10+ (Ubuntu 22.04 is fine). Re-run `cmake ..` after copying the files.
- Hardening: `trySolidize()` can no longer leave the game without a next block; fixed an unsigned underflow near the top row;
  cleaned up all `-Wall -Wextra` warnings.

## RPG = menu option 7 "Text-Based RPG Engine" (test.cpp)
Rewritten as a turn-based survival RPG: Attack / Power Strike / Defend / Potion / Flee, 4 monster types that unlock
over time, a boss every 5th wave, XP + levels, gold, a camp shop between fights, wave counter and a session best.
Input is validated and the game no longer uses `system("clear")`.
