#include "SnakeGame.h"

#include <chrono>
#include <iostream>
#include <thread>


// ============================================================================================================================================================
// 											I n s t a n t i a t i o n   &   I n i t i a l i z a t i o n
// ============================================================================================================================================================

/**
 * @brief Custom constructor: sets up OS specific terminal configurations (via AsyncKey), shows the controls,
 * starts a new game and enters the main loop.
 */
SnakeGame::SnakeGame()
{
	AsyncKey::initTerminal();											// OS specific terminal setup
	std::cout << Configs::CLEAR_DISPL << Configs::HIDE_CURSOR;
	std::cout
		<< "Snake Controls\n\n"
		<< "Key          Action\n"
		<< "ARROW UP     Move Up\n"
		<< "ARROW DOWN   Move Down\n"
		<< "ARROW LEFT   Move Left\n"
		<< "ARROW RIGHT  Move Right\n"
		<< "ENTER        Restart Game (Only active on GAME OVER screen)\n"
		<< "K            Instant Quit (Available at any time)\n\n"
		<< "Starting game in 3 seconds..." << std::flush;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::cout << Configs::CLEAR_DISPL << Configs::CURSOR_POS0;

	board.resize(Configs::SIZE);

	initNewGame();
	startLoop();
}

/**
 * @brief Custom destructor: shows the cursor on the console again and restores the OS specific terminal configurations.
 */
SnakeGame::~SnakeGame()
{
	std::cout << Configs::SHOW_CURSOR;									// show the cursor again (user has quit the game)
	AsyncKey::restoreTerminal();										// OS specific terminal restoration
}

/**
 * @brief Sets/Resets all data for a new game session.
 */
void SnakeGame::initNewGame()
{
	setBoard();
	snake.clear();

	const uint16_t startRow = Configs::ROWS / 2;
	const uint16_t startCol = Configs::COLS / 2;

	// starting snake: 3 segments laid out horizontally, head on the right, moving right
	snake.push_back(idx(startRow, startCol));
	snake.push_back(idx(startRow, startCol - 1));
	snake.push_back(idx(startRow, startCol - 2));

	for (std::size_t i = 0; i < snake.size(); ++i)
		board[snake[i]] = (i == 0) ? 2 : 1;							// front = head (id 2), rest = body (id 1)

	dir = Direction::RIGHT;
	pendingDir = Direction::RIGHT;
	score = 0;
	gameOver = false;

	placeFood();														// also triggers the first drawBoard()
}

/**
 * @brief Initializes the entire game board, including the surrounding border walls.
 */
void SnakeGame::setBoard()
{
	std::fill(board.begin(), board.end(), static_cast<uint8_t>(0));

	for (uint16_t col = 0; col < Configs::BOARD_W; ++col)
	{
		board[idx(0, col)] = 4;
		board[idx(Configs::BOARD_H - 1, col)] = 4;
	}
	for (uint16_t row = 0; row < Configs::BOARD_H; ++row)
	{
		board[idx(row, 0)] = 4;
		board[idx(row, Configs::BOARD_W - 1)] = 4;
	}
}

/**
 * @brief Places a food block on a random EMPTY cell of the playable area (border and snake cells excluded).
 */
void SnakeGame::placeFood()
{
	std::uniform_int_distribution<int> rowDist(1, Configs::ROWS);
	std::uniform_int_distribution<int> colDist(1, Configs::COLS);

	uint16_t pos;
	do
	{
		pos = idx(static_cast<uint16_t>(rowDist(rng)), static_cast<uint16_t>(colDist(rng)));
	} while (board[pos] != 0);											// retry until a free cell is found

	foodIdx = pos;
	board[foodIdx] = 3;

	drawBoard();
}

// ============================================================================================================================================================
// 																  M a i n   L o o p
// ============================================================================================================================================================

/**
 * @brief Starts the main loop and thus the next game session.
 * Input is polled every FRAME_MS for responsive controls; the snake itself only advances every TICK_MS,
 * which keeps the game at a constant, comfortable speed regardless of how often input is sampled.
 */
void SnakeGame::startLoop()
{
	std::cout << std::flush;											// flush the console for immediate response

	const std::chrono::microseconds FRAME_LEN(Configs::FRAME_MS * 1000);
	std::chrono::microseconds accu(0);
	auto lastTime = std::chrono::steady_clock::now();

	const int TICKS_PER_STEP = Configs::TICK_MS / Configs::FRAME_MS;	// how many FRAME_LEN ticks make up one snake step
	int stepCounter = 0;

	while (isRunning)													// the MAIN LOOP starts (lasts the game session's whole lifetime)
	{
		auto currTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currTime - lastTime);

		if (elapsedTime.count() == 0) continue;							// GUARD: bypass the rest, when no time diff. is registered

		lastTime = currTime;
		accu += elapsedTime;

		while (accu >= FRAME_LEN)										// enter at each target FRAME (every FRAME_MS ms)
		{
			accu -= FRAME_LEN;

			processInput();

			if (!gameOver && ++stepCounter >= TICKS_PER_STEP)
			{
				stepCounter = 0;
				updateGame();
				drawBoard();
			}
		}
	}
}

// ============================================================================================================================================================
// 																	C o n t r o l s
// ============================================================================================================================================================

/**
 * @brief Essentially the listener for user inputs (uses the AsyncKey namespace).
 */
void SnakeGame::processInput()
{
	if (gameOver)
	{
		if (AsyncKey::isPressed(AsyncKey::Key::K))
			isRunning = false;
		else if (AsyncKey::isPressed(AsyncKey::Key::ENTER))
			initNewGame();
		return;
	}

	if (AsyncKey::isPressed(AsyncKey::Key::K))
	{
		isRunning = false;
		return;
	}

	// buffer the next direction; reversing straight into the snake's own neck is ignored (standard Snake rule)
	if (AsyncKey::isPressed(AsyncKey::Key::UP)    && dir != Direction::DOWN)  pendingDir = Direction::UP;
	if (AsyncKey::isPressed(AsyncKey::Key::DOWN)  && dir != Direction::UP)    pendingDir = Direction::DOWN;
	if (AsyncKey::isPressed(AsyncKey::Key::LEFT)  && dir != Direction::RIGHT) pendingDir = Direction::LEFT;
	if (AsyncKey::isPressed(AsyncKey::Key::RIGHT) && dir != Direction::LEFT)  pendingDir = Direction::RIGHT;
}

/**
 * @brief Updates the game board (the game logic): advances the snake by one step, resolves collisions
 * and growth, and ends the game if the snake has hit a wall or itself.
 */
void SnakeGame::updateGame()
{
	dir = pendingDir;

	const uint16_t headIdx = snake.front();
	int row = headIdx / Configs::BOARD_W;
	int col = headIdx % Configs::BOARD_W;

	switch (dir)
	{
		case Direction::UP:    --row; break;
		case Direction::DOWN:  ++row; break;
		case Direction::LEFT:  --col; break;
		case Direction::RIGHT: ++col; break;
	}

	// hitting any of the four edges of the board ends the game
	if (row <= 0 || row >= Configs::BOARD_H - 1 || col <= 0 || col >= Configs::BOARD_W - 1)
	{
		gameOver = true;
		return;
	}

	const uint16_t newHead = idx(static_cast<uint16_t>(row), static_cast<uint16_t>(col));
	const bool ateFood = (newHead == foodIdx);

	// the snake overlapping (running into) itself ends the game.
	// when NOT eating, the tail cell is excluded: it is about to move out of the way this very step,
	// so moving the head onto the current tail position is a normal, legal move (standard Snake rule).
	const std::size_t checkLimit = snake.size() - (ateFood ? 0 : 1);
	for (std::size_t i = 0; i < checkLimit; ++i)
	{
		if (snake[i] == newHead)
		{
			gameOver = true;
			return;
		}
	}

	board[snake.front()] = 1;											// old head becomes a regular body segment
	snake.push_front(newHead);
	board[newHead] = 2;													// new head

	if (ateFood)
	{
		++score;														// the snake's length grows by one (the tail is simply not popped)
		placeFood();													// spawn a new block at a random empty position
	}
	else
	{
		board[snake.back()] = 0;										// clear the vacated tail cell
		snake.pop_back();
	}
}

/**
 * @brief Populates the `std::stringstream` with the Unicode characters referenced by each cell's ID and
 * flushes it to the console in one shot (avoids per-character flicker).
 */
void SnakeGame::drawBoard()
{
	ss.str("");
	ss << Configs::CURSOR_POS0;
	ss << "SCORE: " << score << "   |   Arrows = move   |   K = quit\n\n";

	for (uint16_t row = 0; row < Configs::BOARD_H; ++row)
	{
		for (uint16_t col = 0; col < Configs::BOARD_W; ++col)
			ss << Configs::VISUALS[board[idx(row, col)]];
		ss << '\n';
	}

	if (gameOver)
	{
		ss << "\n              G A M E   O V E R\n";
		ss << "            Final Score: " << score << "\n";
		ss << "     [ENTER] New Game        [K] Quit\n";
	}

	std::cout << ss.str() << std::flush;
}
