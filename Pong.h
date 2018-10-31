#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#define GAMETILE "Pong!"

#define RES_WIDTH 800
#define RES_HEIGHT 600

#define BALL_R 10.0f
#define PADDLE_W 25.0f
#define PADDLE_H 100.0f

using namespace sf;

class Ball
{
	CircleShape shape;
	Texture tex;
	Sound hitsound;
	Vector2f v;

	friend class Pong;
	friend class Paddle;

public:
	Ball();
	void move(float dt);
};

class Paddle
{
	RectangleShape shape;
	Texture tex;
	friend class Pong;

	Paddle();
};

class Pong
{
	Ball ball;
	Paddle player1;
	Paddle player2; // AI
	RenderWindow window;

	void update_state(float dt);
	void render_frame();
public:
	Pong();
	~Pong();
	int run();
};

