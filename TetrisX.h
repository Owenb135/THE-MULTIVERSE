#pragma once
#include "AsyncKey.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <ranges>
#include <random>
#include <thread>
#include <chrono>
#include <array>


/**
 * @brief Implements a version of the well-known arcade classic "Tetris" that closely resembles the original visually and in gameplay.
 * This program has been implemented to be as efficient and high-performance as possible (in terms of memory and algorithms)
 * to ensure a smooth and enjoyable gaming experience on the console.
 *
 * @author Ibrahim Ibram
 * @date June 2026
 */
class TetrisX
{
public:
	TetrisX();
	~TetrisX();

private:

	// ================================================================================================================================================
	// 												  E N C A P S U L A T E D   D A T A   S T R U C T U R E S
	// ================================================================================================================================================

	/**
	* @brief An enum for the type and direction of movement.
	*/
	enum Movement : uint8_t { ROTATION, DOWN, LEFT, RIGHT };

	/**
	* @brief An enum for the states of the completion routine's "Finite-State Machine" (FSM).
	*/
	enum State : uint8_t { IDLE, BOOM_1, BOOM_2, DROP };

	/**
	* @brief Represents a cell of the currently active Tetris block `currBlock`.
	*/
	struct B_Cell
	{
		uint16_t idx = 0;									// index on the 1D board array
		uint8_t  lvl = 0;									// distance level to the center of the block (3x3 or 4x4)
		uint8_t  off = 0;									// Offset in the corresponding ROT array (phase of clockwise rotation around the block center)
	};

	using Block = std::array<B_Cell, 4>;					// represents a Tetris block for the game board (B_Cell = {idx, lvl, off})
	using FlatBlock = std::array<std::string_view, 3>;		// represents a Tetris block as a flat string_view for the next-block preview


	/**
	 * @brief Essential game configurations (stored directly in Flash/RO-data by the compiler)
	 */
	struct Configs
	{
		constexpr static uint8_t  ROWS = 21;								// height of the board (+ bottom margin)
		constexpr static uint8_t  COLS = 14;								// width of the board (+ side margins)
		constexpr static uint16_t SIZE = 294;								// size of the board, which is a flat 1D array
		constexpr static uint16_t LAST = 280;								// last index of the board (excluding the bottom edge)

		constexpr static std::string_view CURSOR_POS0 = "\033[H";			// ANSI Code: move cursor to pos 0
		constexpr static std::string_view CLEAR_DISPL = "\033[2J";			// ANSI code: clear console
		constexpr static std::string_view HIDE_CURSOR = "\033[?25l";		// ANSI Code: hide cursor
		constexpr static std::string_view SHOW_CURSOR = "\033[?25h";		// ANSI Code: show cursor

		// Visualization of a block. Access: VISUALS[ Cell.id ]
		constexpr static std::string_view VISUALS[14] =
		{
			"  ",																								// free	[0]			 : "  "
			"\U0001F7E5", "\U0001F7E9", "\U0001F7E6", "\U0001F7E7", "\U0001F7EA", "\U0001F7E8", "\U0001F7EB",	// blocks [1..7]	 : 🟥🟩🟦🟧🟪🟨🟫
			"\U0001F4A5", "\U0001F4A2",																			// completions [8,9] : 💥💢
			"\U00002503", "\U00002517", "\U00002501\U00002501", "\U0000251B",									// borders [10..13]	 : ┃┗ ━━ ┛
		};

		/*
			┃ ┏━━━━━━━━━━━━━━━━━━━━┓ ┃
			┃ ┃  G A M E  O V E R  ┃ ┃
			┃ ┃ ------------------ ┃ ┃
			┃ ┃ [ENTER] > NEW GAME ┃ ┃
			┃ ┃  [ESC]  >   QUIT   ┃ ┃
			┃ ┗━━━━━━━━━━━━━━━━━━━━┛ ┃
		*/
		// GAME OVER label as a string_view array. Access: GAME_OVER [ 0..5 ]
		constexpr static std::string_view GAM_OVER[6] =
		{
			"\U00002503 \U0000250F\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501\U00002513 \U00002503",
			"\U00002503 \U00002503  G A M E  O V E R  \U00002503 \U00002503",
			"\U00002503 \U00002503 ------------------ \U00002503 \U00002503",
			"\U00002503 \U00002503 [ENTER] > NEW GAME \U00002503 \U00002503",
			"\U00002503 \U00002503  [ESC]  >   QUIT   \U00002503 \U00002503",
			"\U00002503 \U00002517\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501\U0000251B \U00002503"
		};

		/*
			┏━━━━━━━━━━━━━━━┳━━━━━━━━┓	\U0000250F: ┏	\U00002513: ┓
			┃ SCORE: 000000 ┃        ┃
			┃ LEVEL: 000    ┃        ┃	\U00002533: ┳	\U0000253B: ┻
			┃ LINES: 000    ┃        ┃
			┣━━━━━━━━━━━━━━━┻━━━━━━━━┫	\U00002523: ┣	\U0000252B: ┫
		*/
		// HUD as a string_view array. Access: HUD [ i ]
		constexpr static std::string_view HUD[4] =
		{
			"\U0000250F\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501"	// Index 0: "┏━━━━━━━━━━━━━━━┳━━━━━━━━┓\n┃ SCORE: "
			"\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002533\U00002501\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002501\U00002501\U00002513\n\U00002503 SCORE: ",
			"\U00002503\n\U00002503 LEVEL: ",										 	// Index 1: "┃\n┃ LEVEL: "
			"\U00002503\n\U00002503 LINES: ",										 	// Index 2: "┃\n┃ LINES: "
			"\U00002503\n\U00002523\U00002501\U00002501\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002501\U0000253B\U00002501\U00002501\U00002501"
			"\U00002501\U00002501\U00002501\U00002501\U00002501\U0000252B\n"			// Index 3: "┃\n┣━━━━━━━━━━━━━━━┻━━━━━━━━┫\n"
		};

		/*
						   Block insertion
							 +  +  +  +
				-----------------------------------
			 0┃  1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12 ┃13		 5  6  7  8
			14┃ 15|16|17|18|19|20|21|22|23|24|25|26 ┃27		19 20 21 22
			28┃ 29|30|31|32|33|34|35|36|37|38|39|40 ┃41		33 34 35 36
			42┃ 43|44|45|46|47|48|49|50|51|52|53|54 ┃55		47 48 49 50
				-----------------------------------
		*/
		// Spawning point of Tetris blocks on the game board. Access: BLOCKS[ currID - 1 ] !!!
		constexpr static std::array<Block, 7> BLOCKS =
		{ {
			{ { { 6, 1, 0 }, { 20, 0, 0 }, { 21, 1, 1 }, { 35, 2, 2 } } },		// 1: S		 (3x3)
			{ { { 7, 2, 1 }, { 20, 0, 0 }, { 21, 1, 1 }, { 34, 1, 2 } } },		// 2: Z		 (3x3)
			{ { { 6, 1, 0 }, { 7, 2, 1 }, { 20, 0, 0 }, { 34, 1, 2 } } },		// 3: Gamma	 (3x3)
			{ { { 6, 1, 0 }, { 20, 0, 0 }, { 34, 1, 2 }, { 35, 2, 2 } } },		// 4: L		 (3x3)
			{ { { 6, 1, 0 }, { 20, 0, 0 }, { 21, 1, 1 }, { 34, 1, 2 } } },		// 5: T		 (3x3)
			{ { { 6, 0, 0 }, { 7, 0, 1 }, { 20, 0, 3 }, { 21, 0, 2 } } },		// 6: Square (4x4)	starts one line higher. Rotation is disabled !
			{ { { 19, 2, 3 }, { 20, 0, 0 }, { 21, 0, 1 }, { 22, 1, 1 } } }		// 7: Pipe	 (4x4)
			}
		};

		// The first 3 rows of each Tetris block as a flat string_view array for the preview. Access: FLAT_BLOCKS[ nextID - 1 ] !!!
		constexpr static std::array<FlatBlock, 7> FLAT_BLOCKS =
		{ {
			{ "  \U0001F7E5    ", "  \U0001F7E5\U0001F7E5  ", "    \U0001F7E5  " },		// 1: S			(3x3) 🟥
			{ "    \U0001F7E9  ", "  \U0001F7E9\U0001F7E9  ", "  \U0001F7E9    " },		// 2: Z			(3x3) 🟩
			{ "  \U0001F7E6\U0001F7E6  ", "  \U0001F7E6    ", "  \U0001F7E6    " },		// 3: Gamma		(3x3) 🟦
			{ "  \U0001F7E7    ", "  \U0001F7E7    ", "  \U0001F7E7\U0001F7E7  " },		// 4: L			(3x3) 🟧
			{ "  \U0001F7EA    ", "  \U0001F7EA\U0001F7EA  ", "  \U0001F7EA    " },		// 5: T			(3x3) 🟪
			{ "        ", "  \U0001F7E8\U0001F7E8  ", "  \U0001F7E8\U0001F7E8  " },		// 6: Square	(4x4) 🟨  1 line lower (for a better preview look)
			{ "        ", "\U0001F7EB\U0001F7EB\U0001F7EB\U0001F7EB", "        " },		// 7: Pipe		(4x4) 🟫
			}
		};

		// Rotation templates for 3x3 blocks [ B_Cell.lvl ] [ B_Cell.off ]
		constexpr static std::array<std::array<short, 4>, 3> ROT_3X3 =
		{ {
			{ { 0, 0, 0, 0 } },			// Level 0 (center: never moves)
			{ { 15, 13, -15, -13 } },	// Level 1
			{ { 2, 28, -2, -28 } }		// Level 2
			}
		};

		// Rotation templates for 4x4 blocks [ B_Cell.lvl ] [ B_Cell.off ]
		constexpr static std::array<std::array<short, 4>, 3> ROT_4X4 =
		{ {
			{ { 1, 14, -1, -14 } },		// Level 0
			{ { 16, 27, -16, -27 } },	// Level 1A
			{ { 29, 12, -29, -12 } },	// Level 1B
			}
		};

		// Score table. Access: SCORE_TABLE [ row - 1 ] !!!
		constexpr static std::array<uint16_t, 4> SCORE_TABLE = { 40, 100, 300, 1200 };

		// Game speed table. Number of 20 ms ticks to wait for autoDown. Access: SPEEDS [ level ]
		constexpr static std::array<int, 18> SPEEDS = { 50, 45, 40, 35, 31, 27, 23, 20, 17, 15, 13, 11, 10, 9, 8, 7, 6, 5 };
	};


	/**
	 * @brief Represents a cell of the game board
	 */
	struct Cell
	{
		uint16_t idx = 0;									// index on the 1D board array
		uint8_t id = 0;										// the id to visualize
		bool solid = false;									// solid or not
	};

	/**
	 * @brief Data structure for data collection during any collision detection.
	 * @note saves the re-calculation of the resulting board indices of the `currBlock` when the movement is applied
	 */
	struct MoveInfo
	{
		uint16_t idx[4] = {};								// is populated with the board indices of the `currBlock` (during each check)
		bool canMove = false;								// `true` if the user-specified movement can be applied to the `currBlock`.
	};

	// ================================================================================================================================================
	// 														 M E M B E R S   &   D E C L A R A T I O N S
	// ================================================================================================================================================

	std::array<Cell, Configs::SIZE > board;		// the entire game board (including borders) as a flat 1D array (Cell = {idx, id, solid})
	std::stringstream ss;						// reusable StringStream for rendering the game board every frame (20 ms)
	std::vector<uint16_t > complVec;			// container for the indices of the blocks of completed rows

	Block currBlock;							// the current (controllable) block containing all the information
	MoveInfo moveInfo;							// captured infos of the latest ability to move check
	std::array<uint8_t, 7> bag;					// 7-bag system array
	State complFSM = State::IDLE;				// the current "Completion Routine"s FSM state (IDLE, BOOM_1, BOOM_2, DROP)
	uint32_t score = 0;							// player score
	uint16_t lines = 0;							// total number of completed lines
	uint8_t level = 0;							// difficulty level

	uint8_t currID = 0;							// the ID of the current block
	uint8_t nextID = 0;							// the ID of the next block
	uint8_t nIDX = 0;							// next index for the 7-bag system

	uint8_t bottomRow = 0;						// the lowest row from which to check upwards for completed rows
	uint8_t autoDown = 0;						// counts the duration until the next autoDown
	uint8_t complFrame = 0;						// counts the duration of a completion phase
	uint8_t line_cnt = 0;						// counter for completing 10 lines (level increase every 10 lines)

	bool isRunning = true;						// flag: app state (overall app lifetime)
	bool newBlock = true;						// flag: new block spawns
	bool hitGround = false;						// flag: currBlock has hit a SOLID field vertically
	bool actionPerformed = false;				// flag: an action was performed that requires an update of the game board
	bool isBlocked = false;						// flag: the currBlock is immediately blocked upon appearance
	bool gameOver = false;						// flag: current game session has ended

	bool CURR_DOWN = false;						// key control: current value of down key
	bool PREV_L = false;						// key control: previous value of left key
	bool PREV_R = false;						// key control: previous value of right key
	bool PREV_ROT = false;						// key control: previous Value of rotation Key
	bool PREV_DOWN = true;						// key control: Previous value of down key

	void initNewGame();
	void setBoard();
	void shuffleBag();
	void startLoop();

	void processInput();
	void updateGame();
	void drawBoard();

	void setMoveInfo(Movement dir);
	void move(Movement dir);
	void rotate();
	void trySolidize();
	void completion_1(uint8_t bottomRow);
	void completion_2();
	void completion_3();
};
