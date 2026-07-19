#ifndef ASYNC_KEY_H
#define ASYNC_KEY_H

#include <cstdint>

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN				// get rid of unnecessary Windows bloat (speeds up compilation)
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
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
		ESC, ENTER, SPACE, TAB,
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
			case Key::ESC:   vKey = 0x1B; break; // VK_ESCAPE
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
	// LINUX / X11 IMPLEMENTATION (True Hardware State Polling)
	// ================================================================================================================
	#else

	inline struct termios oldt, newt;

	/**
	 * @brief Completely disables terminal echo and line buffering for this session, when Linux is detected.
	 */
	inline void initTerminal()
	{
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);								// echo and line buffer OFF
		newt.c_cc[VMIN] = 0;											// non-blocking
		newt.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	}

	/**
	 * @brief Upon exiting, restores the terminal to its original state and flushes the input buffer.
	 * @note This function is a No-op on Windows and only affects Linux environments.
	 */
	inline void restoreTerminal()
	{
		tcflush(STDIN_FILENO, TCIFLUSH);								// clear all terminal characters accumulated during the game
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
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
		// open connection to the local display server
		Display* dpy = XOpenDisplay(NULL);
		if (!dpy) return false;

		// request keyboard vector from the server (32 bytes = 256 bits for all keys)
		char keys_return[32];
		XQueryKeymap(dpy, keys_return);

		// map the platform-independent key to X11 keysyms (virtual keys)
		KeySym keysym = 0;
		switch (key)
		{
			case Key::UP:	 keysym = XK_Up;		break;
			case Key::DOWN:	 keysym = XK_Down;		break;
			case Key::LEFT:	 keysym = XK_Left;		break;
			case Key::RIGHT: keysym = XK_Right;		break;
			case Key::ESC:	 keysym = XK_Escape;	break;
			case Key::ENTER: keysym = XK_Return;	break;
			case Key::SPACE: keysym = XK_space;		break;
			case Key::TAB:	 keysym = XK_Tab;		break;
			case Key::W:	 keysym = XK_w;			break;
			case Key::A:	 keysym = XK_a;			break;
			case Key::S:	 keysym = XK_s;			break;
			case Key::D:	 keysym = XK_d;			break;
			default: XCloseDisplay(dpy);	 return false;
		}

		// translate the keysym into the hardware-dependent keycode
		KeyCode kc = XKeysymToKeycode(dpy, keysym);

		// check the corresponding bit in the 32-byte array
		bool pressed = (keys_return[kc / 8] & (1 << (kc % 8))) != 0;

		// close connection cleanly
		XCloseDisplay(dpy);

		return pressed;
	}

	#endif
}
#endif // ASYNC_KEY_H
