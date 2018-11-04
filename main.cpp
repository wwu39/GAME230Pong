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

#include "Pong.h"

#define ONE_PLAYER 1
#define TWO_PLAYERS 2

int main()
{
	RenderWindow window;
	RectangleShape background;
	CircleShape cursor;
	Texture bgtex;
	Texture curtex;
	Font font;
	Text options;
	SoundBuffer buf;
	Sound optionswitch;
	int rate = RATE;
	int rate2 = 0;
	int ret = REPEAT;
	int mode = ONE_PLAYER;
	STATUS status = NONE;

	// seting up menu background
	background.setSize({ RES_WIDTH, RES_HEIGHT });
	bgtex.loadFromFile("sprites/menu.png");
	background.setTexture(&bgtex);

	// setting up texts
	font.loadFromFile("font.ttf");
	options.setFont(font);
	options.setCharacterSize(50);
	options.setFillColor(Color::Green);
	options.setPosition(300, 350);
	std::ostringstream str;
	str << "1 PLAYER" << std::endl << "2 PLAYERS" << std::endl << "EXIT";
	options.setString(str.str());

	// setting up cursor
	cursor.setRadius(BALL_R);
	cursor.setOrigin({ BALL_R, BALL_R });
	curtex.loadFromFile("sprites/bomb 0014.png");
	cursor.setTexture(&curtex);
	cursor.setPosition(265, 375);

	// setting up sound
	buf.loadFromFile("sound/option.wav");
	optionswitch.setBuffer(buf);

	// creating window
	window.create(VideoMode(RES_WIDTH, RES_HEIGHT), GAMETILE);

	Clock clock;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				return EXIT;
			}
		}

		float dt = clock.restart().asSeconds();
		// update state
		if (status == NONE) {
			++rate;
			if (rate / RATE) {
				Vector2f pos = cursor.getPosition();
				if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
					if (pos.y > 375.0f && pos.y <= 475.0f) {
						pos.y -= 50.0f;
						optionswitch.play();
					}
				if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
					if (pos.y >= 375.0f && pos.y < 475.0f) {
						pos.y += 50.0f;
						optionswitch.play();
					}
				cursor.setPosition(pos);
				if (Keyboard::isKeyPressed(Keyboard::Enter) || Keyboard::isKeyPressed(Keyboard::Space)) {
					Vector2f pos = cursor.getPosition();
					if (pos.y == 375.0f) {}
					if (pos.y == 425.0f) mode = TWO_PLAYERS;
					if (pos.y == 475.0f) return EXIT;

					// Start game
					while (ret == REPEAT) {
						Pong * game = new Pong(mode); // start a new game
						ret = game->run(window);
						delete game; // game over
						if (ret == EXIT) return ret; // if exit from inside
						if (ret == MENU) status = ENDING; // if exit from inside
						game = nullptr;
					}
					ret = REPEAT;
				}
				rate = 0;
			}
		}
		if (status == ENDING) {
			++rate2;
			if (rate2 / (RATE * 8)) {
				status = NONE;
				rate2 = 0;
			}
		}
		// rander frame
		window.clear(Color(0, 0, 255));
		window.draw(background);
		window.draw(cursor);
		window.draw(options);
		window.display();
	}
}