#pragma once
#include "AsyncKey.h"
#include <cstdint>
#include <deque>
#include <random>
#include <sstream>
#include <string_view>
#include <vector>


/**
 * @brief Implements a classic console "Snake" game. Uses the same cross-platform
 * async keyboard input (AsyncKey) and fixed-timestep render loop approach as TetrisX,
 * so it fits the rest of the project's game modules.
 *
 * Controls:
 *   ARROW UP / DOWN / LEFT / RIGHT   Steer the snake (cannot reverse directly into itself)
 *   ENTER                            Restart (only on the GAME OVER screen)
 *   K                                Quit at any time
 *
 * Rules:
 *   - A food block spawns at a random empty position on the board.
 *   - Hitting any of the four edges of the board ends the game.
 *   - The snake running into its own body ends the game.
 *   - Eating a food block grows the snake by one segment and spawns a new block.
 */
class SnakeGame
{
public:
	SnakeGame();
	~SnakeGame();

private:

	enum class Direction : uint8_t { UP, DOWN, LEFT, RIGHT };

	/**
	 * @brief Essential game configurations (stored directly in Flash/RO-data by the compiler)
	 */
	struct Configs
	{
		constexpr static uint16_t ROWS = 20;								// height of the playable area
		constexpr static uint16_t COLS = 30;								// width of the playable area
		constexpr static uint16_t BOARD_W = COLS + 2;						// + left/right border
		constexpr static uint16_t BOARD_H = ROWS + 2;						// + top/bottom border
		constexpr static uint32_t SIZE = static_cast<uint32_t>(BOARD_W) * BOARD_H;

		constexpr static std::string_view CURSOR_POS0 = "\033[H";			// ANSI Code: move cursor to pos 0
		constexpr static std::string_view CLEAR_DISPL = "\033[2J";		// ANSI Code: clear console
		constexpr static std::string_view HIDE_CURSOR = "\033[?25l";		// ANSI Code: hide cursor
		constexpr static std::string_view SHOW_CURSOR = "\033[?25h";		// ANSI Code: show cursor

		// Visualization of a cell. Access: VISUALS[ cell id ]
		constexpr static std::string_view VISUALS[5] =
		{
			"  ",							// 0: free
			"\U0001F7E9",					// 1: snake body  🟩
			"\U0001F7E8",					// 2: snake head  🟨
			"\U0001F7E5",					// 3: food block  🟥
			"\U00002B1B",					// 4: border wall ⬛
		};

		constexpr static int TICK_MS = 120;								// time (ms) between each snake step (constant speed)
		constexpr static int FRAME_MS = 20;								// input-polling interval; keeps controls responsive between steps
	};

	std::vector<uint8_t> board;				// flat board of cell ids (0 free, 1 body, 2 head, 3 food, 4 wall)
	std::deque<uint16_t> snake;				// indices of snake cells on the board; front() = head, back() = tail
	std::stringstream ss;						// reusable stringstream for rendering the board every step
	std::mt19937 rng{ std::random_device{}() };// RNG used to place food blocks

	Direction dir = Direction::RIGHT;			// current direction of travel
	Direction pendingDir = Direction::RIGHT;	// buffered next direction from input (applied on the next step)

	uint16_t foodIdx = 0;						// board index of the current food block
	uint32_t score = 0;						// number of food blocks eaten

	bool isRunning = true;						// flag: app state (overall app lifetime)
	bool gameOver = false;						// flag: current game session has ended

	void initNewGame();
	void setBoard();
	void placeFood();
	void startLoop();

	void processInput();
	void updateGame();
	void drawBoard();

	static constexpr uint16_t idx(uint16_t row, uint16_t col) { return row * Configs::BOARD_W + col; }
};
