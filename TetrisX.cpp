#include "TetrisX.h"
#include <iomanip>


// ============================================================================================================================================================
// 													I n s t a n t i a t i o n   &   I n i t i a l i z a t i o n
// ============================================================================================================================================================

/**
 * @brief Custom Constructor: Initializes the 7-bag-system and OS specific terminal configurations (via AsyncKey), reserves space for the `std::stringstream`
 * and for `complVec` with expected sizes, initiates a new game and starts the main loop.
 */
TetrisX::TetrisX() : bag({ 1, 2, 3, 4, 5, 6, 7 })
{
	AsyncKey::initTerminal();														// OS specific terminal setup
	std::cout << Configs::CLEAR_DISPL << Configs::HIDE_CURSOR;						// clear all characters from the console and hide the cursor

	// reserve sufficient memory for the std::stringstream
	// (Emojis require 4 bytes in UTF-8 + any line breaks)
	uint16_t assumedSize = (Configs::ROWS + 5) * 5;									// ((Configs::ROWS + 5) << 2) + (Configs::ROWS + 5) ==> (Configs::ROWS + 5) * 5
	ss.rdbuf()->str().reserve(assumedSize);

	complVec.reserve((Configs::COLS << 2));

	initNewGame();
	startLoop();
}

/**
 * @brief Custom destructor: Shows the cursor on the console again and restores the OS specific terminal configurations.
 */
TetrisX::~TetrisX()
{
	std::cout << Configs::SHOW_CURSOR;												// show the cursor again (user has quit the game)
	AsyncKey::restoreTerminal();													// OS specific terminal restoration
}

/**
 * @brief Sets/Resets all data for a new game session
 */
void TetrisX::initNewGame()
{
	setBoard();
	shuffleBag();

	nextID = bag[nIDX];																// implicitly results in: currID = bag[0] at the start of the game
	actionPerformed = true;															// implies an initial call to drawBoard()
	hitGround = false;																// currBlock hasn't hit the ground yet
	isBlocked = false;																// currBlock isn't blocked yet
	gameOver = false;																// reset game over flag
	newBlock = true;																// immediately spwaning the first block

	// HUD reset
	score = 0;
	lines = 0;
	level = 0;
}

/**
 * @brief Initializes the entire game board, including the borders.
 */
void TetrisX::setBoard()
{
	for (uint16_t i = 0; i < Configs::SIZE; i++)												// initialize the entire board
	{
		board[i].idx = i;
		board[i].id = 0;
		board[i].solid = false;
	}

	for (uint16_t i = 0, j = 13; i < Configs::LAST; i += Configs::COLS, j += Configs::COLS)		// process the vertical borders
	{
		board[i].id = 10; board[i].solid = true;
		board[j].id = 10; board[j].solid = true;
	}

	for (uint16_t i = Configs::LAST; i < Configs::SIZE; i++)									// process the bottom row
	{
		switch (i)
		{
			case Configs::LAST:
				board[i] = { i, 11, true }; break;
			case Configs::SIZE - 1:
				board[i] = { i, 13, true }; break;
			default:
				board[i] = { i, 12, true }; break;
		}
	}
}

/**
 * @brief Shuffles the cells of the bag with the "Mersenne Twister" and resets `nIDX` to the first cell
 */
void TetrisX::shuffleBag()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::shuffle(bag.begin(), bag.end(), gen);

	nIDX = 0;
}

// ============================================================================================================================================================
// 																	   M a i n   L o o p
// ============================================================================================================================================================

/**
 * @brief Starts the main loop and thus the next game session.
 */
void TetrisX::startLoop()
{
	std::cout << std::flush;														// flush the console for immediate response

	const std::chrono::microseconds FRAME_LEN(20000);								// fixed update rate (1000 ms / 20 ms => 50 FPS)
	std::chrono::microseconds accu(0);												// accumulator for summation of the elapsed time
	auto lastTime = std::chrono::steady_clock::now();								// save current time


	while (isRunning)																// the MAIN LOOP starts (vasts the applications whole lifetime)
	{
		auto currTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currTime - lastTime);

		if (elapsedTime.count() == 0) continue;										// GUARD: bypass the rest, when no time diff. is registered

		lastTime = currTime;
		accu += elapsedTime;

		while (accu >= FRAME_LEN)													// enter at each target FRAME (every 20 ms => 50 FPS)
		{
			accu -= FRAME_LEN;

			processInput();

			if (!gameOver)
			{
				updateGame();
				drawBoard();

				++autoDown;
				++complFrame;
			}
		}
	}
}

// ============================================================================================================================================================
// 																		C o n t r o l s
// ============================================================================================================================================================

/**
 * @brief Essentially the listener for user inputs (uses the AsyncKey namespace)
 */
void TetrisX::processInput()
{
	if (AsyncKey::isPressed(AsyncKey::Key::ESC)) isRunning = false;

	else if (!gameOver)
	{
		if (currID)
		{
			bool ROT = AsyncKey::isPressed(AsyncKey::Key::UP);
			bool LEFT = AsyncKey::isPressed(AsyncKey::Key::LEFT);
			bool RIGHT = AsyncKey::isPressed(AsyncKey::Key::RIGHT);

			CURR_DOWN = AsyncKey::isPressed(AsyncKey::Key::DOWN);
			if (LEFT && !PREV_L)  move(Movement::LEFT);
			if (RIGHT && !PREV_R) move(Movement::RIGHT);
			if (currID != 6 && (ROT && !PREV_ROT)) rotate();						// Square (ID = 6) is not rotated! (saving resources)

			PREV_L = LEFT;
			PREV_R = RIGHT;
			PREV_ROT = ROT;
		}
	}

	else if (AsyncKey::isPressed(AsyncKey::Key::ENTER)) initNewGame();
}

/**
 * @brief Updates the game board (the game logic)
 */
void TetrisX::updateGame()
{
	// if applicable: traverse the completion routine's FSM
	if (complFSM != State::IDLE)
	{
		if (complFSM == State::BOOM_1)												// ignores complFrame, but resets it for the subsequent stages
		{
			completion_1(bottomRow);
			complFrame = 0;
		}

		else if (complFSM == State::BOOM_2 && complFrame == 22)
		{
			completion_2();
			complFrame = 0;
			actionPerformed = true;
		}

		else if (complFSM == State::DROP && complFrame == 15)
		{
			completion_3();
			complFSM = State::IDLE;
			actionPerformed = true;
			newBlock = true;														// completion routine finished, newBlock phase begins
		}
	}

	if (complFSM == State::IDLE)													// also enabling entrance right after an acomplished completion routine
	{
		// if applicable: spawn a new block and point to the next nextBlock
		if (newBlock)
		{
			currID = nextID;

			if (nIDX == 6)
				shuffleBag();
			else
				++nIDX;

			nextID = bag[nIDX];

			currBlock = Configs::BLOCKS[currID - 1];

			isBlocked = false;														// isBlocked will be set in the loop accordingly
			for (B_Cell& b : currBlock)
			{
				isBlocked |= board[b.idx].solid;									// is the specific spawn area of the currBlock already blocked?
				board[b.idx] = { b.idx, currID };
			}																		// if so ==> isBlocked = true

			autoDown = 0;
			actionPerformed = true;
			newBlock = false;
		}

		else
		{
			// current block has hit a solid part or the DOWN key was just released.
			// Allows continuous holding the DOWN key without waiting for the autoDown counter to expire;
			// upon hitting the ground, releasing the DOWN key becomes mandatory (decoupling the DOWN key).
			if (hitGround || (PREV_DOWN && !CURR_DOWN))
			{
				PREV_DOWN = CURR_DOWN;
				if (hitGround)
				{
					hitGround = false;
					autoDown = 0;
				}
			}

			// drop currBlock by one row (on continuous DOWN | AUTO DOWN reached)
			if (CURR_DOWN && !PREV_DOWN || autoDown == Configs::SPEEDS[level])
			{
				move(Movement::DOWN);
				trySolidize();

				if (autoDown == Configs::SPEEDS[level])
					autoDown = 0;
			}
		}
	}
}

/**
 * @brief Populates the `std::stringstream` with the Unicode characters referenced by each cell's ID and inserts a newline character after each row.
 * Flushing causes the console to immediately output the contents of the `std::stringstream`. The stream is also prepared for the next frame's output.
 */
void TetrisX::drawBoard()
{
	if (actionPerformed)
	{
		// set cursor to position 0
		ss << Configs::CURSOR_POS0;

		// temporary FlatBlock
		FlatBlock nextBlock = Configs::FLAT_BLOCKS[nextID - 1];

		// ss << title bar (incl. updates)
		ss << Configs::HUD[0] << std::setfill('0') << std::setw(6) << score << " \U00002503" << nextBlock[0];
		ss << Configs::HUD[1] << std::setfill('0') << std::setw(3) << level << "    \U00002503" << nextBlock[1];
		ss << Configs::HUD[2] << std::setfill('0') << std::setw(3) << lines << "    \U00002503" << nextBlock[2];
		ss << Configs::HUD[3];

		// Just in terms of efficiency and performance, we resort to code duplication here (we don't want to disrupt every draw loop) !!!
		if (!isBlocked)
		{
			// ss << current game board
			for (uint8_t r = 0; r < Configs::ROWS; ++r)
			{
				uint16_t base = r * Configs::COLS;

				for (uint8_t c = 0; c < Configs::COLS; ++c)
					ss << Configs::VISUALS[board[base + c].id];

				ss << '\n';
			}
		}

		else // isBlocked = GAME OVER screen (same code as above, but called once in a game session)
		{
			for (uint8_t r = 0; r < Configs::ROWS; ++r)
			{
				uint16_t base = r * Configs::COLS;

				switch (r)
				{
					case  6: ss << Configs::GAM_OVER[0]; break;
					case  7: ss << Configs::GAM_OVER[1]; break;
					case  8: ss << Configs::GAM_OVER[2]; break;
					case  9: ss << Configs::GAM_OVER[3]; break;
					case 10: ss << Configs::GAM_OVER[4]; break;
					case 11: ss << Configs::GAM_OVER[5]; break;

					default:
						for (uint8_t c = 0; c < Configs::COLS; ++c)
							ss << Configs::VISUALS[board[base + c].id];
						break;
				}
				ss << '\n';
			}

			gameOver = true;
		}

		// immediately output the current game board frame to the console and reset the string stream
		std::cout << ss.str() << std::flush;
		ss.str(""); ss.clear();

		actionPerformed = false;
	}
}

// ============================================================================================================================================================
// 																		M o v e m e n t
// ============================================================================================================================================================

/**
 * @brief Captures information regarding the specified movement type and direction, stores the resulting grid indices, and provides feedback on the
 * feasibility of the intended movement direction. The function terminates early if a collision is detected.
 * @param dir enum class Movement { ROTATION, DOWN, LEFT, RIGHT }
 */
void TetrisX::setMoveInfo(Movement dir)
{
	moveInfo.canMove = false;														// assumption: currBlock can't move
	short offIdx;
	short i = -1;

	if (dir == Movement::ROTATION)
	{
		for (B_Cell& b : currBlock)
		{
			// a rotation for the square (ID = 6) never actually arrives here! Listed also as ROT_4X4 solely for the sake of correctness
			offIdx = (currID < 6 ? Configs::ROT_3X3[b.lvl][b.off] : Configs::ROT_4X4[b.lvl][b.off]);

			moveInfo.idx[++i] = b.idx + offIdx;										// collecting the target cell indices for the actual move
			if (board[moveInfo.idx[i]].solid)										// the check: is the targeted cell free?
				return;
		}
	}

	else
	{	// DOWN | LEFT | RIGHT
		if (dir == Movement::DOWN) offIdx = Configs::COLS;
		else if (dir == Movement::LEFT) offIdx = -1;
		else if (dir == Movement::RIGHT) offIdx = 1;

		for (B_Cell& b : currBlock)													// same technique as above (just the offsets can set before once)
		{
			moveInfo.idx[++i] = b.idx + offIdx;
			if (board[moveInfo.idx[i]].solid)
				return;
		}
	}

	moveInfo.canMove = true;														// when arrived here, the currBlock can move
	actionPerformed = true;															// an action - requiring an update of the "canvas" - is happened
}

/**
 * @brief Executes the passed movement for the `currBlock`, if applicable.
 *
 * Invokes `setMoveInfo(Movement)` to check and pre-collect the resulting cells to occupy. If the target movement is applicable, this function performs
 * the movement by updating the board and the currBlock data according to the pre-collected data and the target direction. Also, according the direction
 * of the movement, two different approaches are required during the loop so that the current blocks data isn't overriden faulty.
 * @param dir direction of movement -> enum class Movement { DOWN, LEFT, RIGHT } \ {ROTATION}
 */
void TetrisX::move(Movement dir)
{
	setMoveInfo(dir);																// check and get the data for the user defined movement

	if (moveInfo.canMove)															// if the move is possible, the pre collected indices will be occupied
	{
		short i = -1;
		if (dir == Movement::LEFT)
		{
			for (B_Cell& b : currBlock)												// iterates in ascending order of b.idx (B_cell is sorted that way)
			{
				board[b.idx].id = 0;
				b.idx = moveInfo.idx[++i];
				board[b.idx] = { b.idx, currID };
			}
		}

		else
		{	// DOWN | RIGHT		(Difference: iteration in reverse direction)
			short i = 4;
			for (auto it = currBlock.rbegin(); it != currBlock.rend(); ++it)			// iterates in descending order of the indices
			{
				B_Cell& b = *it;
				board[b.idx].id = 0;
				b.idx = moveInfo.idx[--i];
				board[b.idx] = { b.idx, currID };
			}
		}
	}
}

/**
 * @brief Rotates the current block clockwise. Like `move(Movement)`, this function calls `setMoveInfo(Movement)` and occupies the designated cells,
 * provided the movement is possible. The loop strictly requires the B-cell indices to be in ascending order; therefore, the order must be sorted into
 * ascending sequence again after each rotation to ensure future movements work correctly.
 */
void TetrisX::rotate()
{
	setMoveInfo(Movement::ROTATION);

	if (moveInfo.canMove)
	{
		short i = -1;
		for (B_Cell& b : currBlock)
		{
			board[b.idx].id = 0;
			b.idx = moveInfo.idx[++i];
			b.off = b.off < 3 ? (b.off + 1) : 0;									// bypassing a high cost modulo operation
		}

		for (B_Cell& b : currBlock)													// process the rotated blocks onto the board
			board[b.idx] = { b.idx, currID };

		std::sort(currBlock.begin(), currBlock.end(), [](const B_Cell& a, const B_Cell& b) { return a.idx < b.idx; }); // sort the B_Cells in ascending order by board index
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// 															   C o m p l e t i o n   R o u t i n e
// ------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief When `currBlock` encounters a solid block or the bottom edge of the game board during its downward movement, this function converts `currBlock`
 * into a solid block as well. It also determines the row where the block has landed; starting from this row, the system checks — moving from right to left
 * and upwards — whether and how many complete rows have been formed. This bottommost row is captured for further processing.
 */
void TetrisX::trySolidize()
{
	if (!moveInfo.canMove)
	{
		uint16_t bottomIdx = 1;														// to avoid division by zero (even though it is guaranteed to be populated)
		uint8_t k = 0;																// index for the comparison with BLOCKS

		for (B_Cell& b : currBlock)
		{
			board[b.idx].solid = true;												// currBlock is registered as SOLID in the board
			bottomIdx = b.idx;														// since currBlock is sorted in ascending order => last B_cell.idx = max
		}
		bottomIdx /= Configs::COLS;													// bottommost row as the index, from which 4 rows upwards must be checked

		if (bottomIdx > 0)
		{
			bottomRow = bottomIdx;													// bottomRow is uint8_t, but indexing unfortunately requires values ​​> 255
			complFSM = State::BOOM_1;												// the completion routine begins
		}

		currID = 0;																	// key control and autoDown are implicitly "disabled"
		hitGround = true;															// impact detected
	}
}

/**
 * @brief Stage 1 of the completion routine: Determines whether and how many rows have been completed. If applicable, the IDs of the affected block cells
 * are assigned code 8 (💥) so that with the next `drawBoard()` call, the first stage of completion is displayed accordingly. Additionally, the function
 * collects the indices of these block cells in `complVec` and initiates stage 2 of the completion routine for subsequent animation stages.
 * @param bottomRow the bottommost row where the `currBlock` encountered a fixed block from which the number of completed rows (max. 4) is to be checked upwards
 */
void TetrisX::completion_1(uint8_t bottomRow)
{
	complFSM = State::IDLE;															// assumption: No line was completed. Reset complFSM to IDLE.
	newBlock = true;																// assumption: a newBlock phase then begins
	complVec.clear();

	std::array<uint16_t, 12> line;													// buffer for candidate indices per row
	bool isContinous = true;														// continuity detection (line without a gap?)
	uint8_t cnt = 0;

	uint16_t endIdx = bottomRow * Configs::COLS + 13;								// -1 is performed directly on the element -> [--endIdx]
	uint16_t begIdx = (bottomRow - 3) * Configs::COLS;								// max. 4 lines are possible, but we index from the start of the line

	while (endIdx > begIdx)															// a maximum of 4 lines are checked
	{
		isContinous &= board[--endIdx].solid;
		line[cnt] = endIdx;

		if (++cnt == 12)
		{
			if (isContinous)														// if 12 elements of a row are SOLID => row completed
			{
				for (uint8_t i = 0; i < 12; ++i)
				{
					complVec.push_back(line[i]);									// store the indices of every cell in the entire row in complVec
					board[line[i]].id = 8;											// mark the block cells of the entire row with code 8 = 💥
				}
				newBlock = false;													// the next newBlock phase begins only after full completion.
				actionPerformed = true;												// an action requiring a visual update is performed
				complFSM = State::BOOM_2;											// next stage of the completion routine
			}
			cnt = 0; isContinous = true;											// reset the control elements
			if (endIdx > 1)
				endIdx -= 2;														// skip the left margin
		}
	}
}

/**
 * @brief Stage 2 of the completion routine: the IDs of the previously identified block cells are assigned the next emoji (Code 9 = 💢),
 * and the next stage of the routine is scheduled.
 */
void TetrisX::completion_2()
{
	for (uint16_t& idx : complVec)
		board[idx].id = 9;															// set the next VISUALS stage by ID (Code 9 = 💢)

	complFSM = State::DROP;															// next stage of the completion routine
}

/**
 * @brief The final stage of the completion routine: the cells marked as completed (`complVec`) are now replaced by the cells located above them,
 * offset by the determined number of rows. Here, too, the loop is exited early for efficiency reasons as soon as an empty row is encountered after
 * all lines to be deleted have been processed. Additionally, the new (lower) indices are set accordingly, and the original cells are marked as empty.
 * Thus, only the relevant stack above the completed area is correctly shifted downwards.
 */
void TetrisX::completion_3()
{
	uint8_t rows = complVec.size() / (Configs::COLS - 2);							// number of rows to delete, excluding the L/R margins
	uint16_t minIdx = rows * Configs::COLS;											// offset now adjusted to a full row of the board (including margins)

	score += Configs::SCORE_TABLE[rows - 1];										// updating the score
	lines += rows;																	// updating the completed lines

	if (level < Configs::SPEEDS.size() - 1)											// only when game speed is NOT at max
	{
		line_cnt += rows;
		if (line_cnt > 9)															// at at least 10th completed line => level += 1
		{
			++level;
			line_cnt -= 10;															// any extra complete lines are retained
		}
	}

	// The algorithm: final stage of the completion routine
	uint8_t cnt = 0, solids = 0, off = Configs::COLS;
	bool checkOffset = true;

	for (uint16_t i = complVec[0]; i > minIdx; --i)									// i = complVec[0]: lowest & last cell to be deleted
	{
		if (checkOffset)
		{
			while (board[i - off].id == 9)
				off += Configs::COLS;												// skip completed lines (detect the number of rows to offset)

			checkOffset = false;
		}

		uint16_t offIdx = i - off;													// pointing to first uncompleted lines rightmost cell

		board[i] = board[offIdx];													// shift the cell down by "off" rows
		board[i].idx = i;															// update Cell.idx to the new location
		board[offIdx] = { offIdx, 0, false };										// set source cell to empty = {offIdx, 0, false}

		if (board[i].solid) ++solids;												// count the SOLID cells

		if (++cnt == 12)															// line finished?
		{
			if (rows > 0) --rows;													// only after all lines to be deleted have been processed
			else if (solids == 0) break;											// check if a line without blocks was encountered => break

			checkOffset = true;														// set -> the offset must be checked again
			solids = 0; cnt = 0;													// reset counter after every full line
			i -= 2; 																// skip margins
		}
	}
}
