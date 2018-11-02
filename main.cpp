/*
	note 11/1
	function call overhead
	inline function
	inline float cube(float f) { return f * f * f; };

	friend fns

	static members of class
	shared among all class
	int MyClass::b = 0; // need this in cpp file
	static member fns: same idea
	static member fns can't access non-static members

	op ovrld

	conversion ctor
	explicit

	conversion op
	explicit

	copy ctor
*/
#include <sstream>

#include "Pong.h"

int main()
{
	RenderWindow window;
	window.create(VideoMode(RES_WIDTH, RES_HEIGHT), GAMETILE);
	while (2) {
		// UI

		// Start game
		Pong * game = new Pong(); // start a new game
		int ret = game->run(window);
		delete game; // game over
		if (ret == 0) return ret;
	}
}