
#include <sstream>

#include "Pong.h"

int main()
{
	Pong * game = new Pong(); // start a new game
	int ret = game->run();
	delete game; // game over
	return ret;
}