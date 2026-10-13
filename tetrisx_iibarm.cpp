///Created by Owen0963///
// Menu entry for TetrisX.
//
// TetrisX is compiled into the app like Snake. (It used to compile its own source with g++ in a new
// terminal at runtime, which silently did nothing on installed/packaged builds and on systems without
// x-terminal-emulator or the source tree.)
#include "TetrisX.h"

void startup()
{
	TetrisX game; // runs the whole game inside the constructor and restores the terminal in the destructor
}
