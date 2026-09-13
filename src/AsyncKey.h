#ifndef ASYNC_KEY_H
#define ASYNC_KEY_H

#include <cstdint>

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN				// get rid of unnecessary Windows bloat (speeds up compilation)
#include <windows.h>
#else
#include <poll.h>
#include <unistd.h>
#include <termios.h>
#endif


/**
 * @brief Utilizes the APIs for asynchronous keyboard control on Windows and Linux systems.
 * @author Ibrahim Ibram
 * @date June 2026
 */
namespace AsyncKey
{
	/**
	 * @brief Platform-independent representation of supported game controls.
	 *
	 * Acts as an adapter token to unify keyboard inputs across different operating systems.
	 */
	enum class Key : uint8_t
	{
		UP, DOWN, LEFT, RIGHT,
		K, ENTER, SPACE, TAB,
		W, A, S, D
	};

	// ================================================================================================================
	// WINDOWS IMPLEMENTATION
	// ================================================================================================================
	#if defined(_WIN32) || defined(_WIN64)

	/**
	 * @brief Sets the console to UTF-8 (codepage 65001) for this session, when Windows is detected.
	 */
	inline void initTerminal()
	{
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
	}

	/**
	 * @brief Upon exiting, restores the terminal to its original state and flushes the input buffer.
	 * @note This function is a No-op on Windows and only affects Linux environments
	 */
	inline void restoreTerminal() {}

	/**
	 * @brief Checks asynchronously whether a specific key is currently pressed.
	 * @param key The platform-independent key token to check
	 * @return `true` If the key is currently held down or was pressed
	 * @return `false` If the key is not active
	 * @note Windows implementation: Utilizes native Win32 API polling.
	 */
	inline bool isPressed(Key key)
	{
		int vKey = 0;
		switch (key)
		{
			case Key::UP:    vKey = 0x26; break; // VK_UP
			case Key::DOWN:  vKey = 0x28; break; // VK_DOWN
			case Key::LEFT:  vKey = 0x25; break; // VK_LEFT
			case Key::RIGHT: vKey = 0x27; break; // VK_RIGHT
			case Key::K:   vKey = 0x4B; break; // VK_K
			case Key::ENTER: vKey = 0x0D; break; // VK_RETURN
			case Key::SPACE: vKey = 0x20; break; // VK_SPACE
			case Key::TAB:   vKey = 0x09; break; // VK_TAB
			case Key::W:     vKey = 0x57; break; // VK_W
			case Key::A:     vKey = 0x41; break; // VK_A
			case Key::S:     vKey = 0x53; break; // VK_S
			case Key::D:     vKey = 0x44; break; // VK_D
			default: return false;
		}
		return (GetAsyncKeyState(vKey) & 0x8000) != 0;
	}

	// ================================================================================================================
	// LINUX TERMINAL IMPLEMENTATION
	// ================================================================================================================
	#else

	inline struct termios oldt{};
	inline bool terminalConfigured = false;
	inline uint16_t pendingKeys = 0;

	inline uint16_t keyMask(Key key)
	{
		return static_cast<uint16_t>(1u << static_cast<uint8_t>(key));
	}

	inline void queueKey(Key key)
	{
		pendingKeys |= keyMask(key);
	}

	inline bool readByte(char& ch, int timeoutMilliseconds = 0)
	{
		struct pollfd input{STDIN_FILENO, POLLIN, 0};
		if (poll(&input, 1, timeoutMilliseconds) <= 0 ||
			(input.revents & POLLIN) == 0)
			return false;

		return read(STDIN_FILENO, &ch, 1) == 1;
	}

	inline void readInput()
	{
		char ch;
		while (readByte(ch))
		{
			if (ch == '\033')
			{
				char sequence[2];
				if (readByte(sequence[0], 10) &&
					sequence[0] == '[' &&
					readByte(sequence[1], 10))
				{
					switch (sequence[1])
					{
						case 'A': queueKey(Key::UP); break;
						case 'B': queueKey(Key::DOWN); break;
						case 'C': queueKey(Key::RIGHT); break;
						case 'D': queueKey(Key::LEFT); break;
						default: break;
					}
				}
				else
				{
					queueKey(Key::K);
				}
			}
			else
			{
				switch (ch)
				{
					case '\n':
					case '\r': queueKey(Key::ENTER); break;
					case ' ': queueKey(Key::SPACE); break;
					case '\t': queueKey(Key::TAB); break;
					case 'w':
					case 'W': queueKey(Key::W); break;
					case 'a':
					case 'A': queueKey(Key::A); break;
					case 's':
					case 'S': queueKey(Key::S); break;
					case 'd':
					case 'D': queueKey(Key::D); break;
					case 'k':
					case 'K': queueKey(Key::K); break;
					default: break;
				}
			}
		}
	}

	/**
	 * @brief Completely disables terminal echo and line buffering for this session, when Linux is detected.
	 */
	inline void initTerminal()
	{
		if (!isatty(STDIN_FILENO) || tcgetattr(STDIN_FILENO, &oldt) != 0)
			return;

		struct termios newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);								// echo and line buffer OFF
		newt.c_cc[VMIN] = 0;											// non-blocking
		newt.c_cc[VTIME] = 0;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == 0)
			terminalConfigured = true;
	}

	/**
	 * @brief Upon exiting, restores the terminal to its original state and flushes the input buffer.
	 * @note This function is a No-op on Windows and only affects Linux environments.
	 */
	inline void restoreTerminal()
	{
		if (terminalConfigured)
		{
			tcflush(STDIN_FILENO, TCIFLUSH);							// clear all terminal characters accumulated during the game
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
			terminalConfigured = false;
		}
	}

	/**
	 * @brief Checks asynchronously whether a specific key is currently pressed.
	 * @param key The platform-independent key token to check
	 * @return `true` If the key is currently held down or was pressed
	 * @return `false` If the key is not active
	 * @note Linux implementation: Parses the non-blocking terminal input stream
	 */
	inline bool isPressed(Key key)
	{
		readInput();
		const uint16_t mask = keyMask(key);
		const bool pressed = (pendingKeys & mask) != 0;
		pendingKeys &= static_cast<uint16_t>(~mask);
		return pressed;
	}

	#endif
}
#endif // ASYNC_KEY_H
