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
#define BALL_R 30.0f // radius of the ball
#define BALL_CR 12.0f // radius of the ball used in collision
#define PADDLE_W 25.0f
#define PADDLE_H 100.0f
#define BALL_SPEED 2.0f
#define RATE 10

using namespace sf;

class Ball
{
	CircleShape shape;
	Texture tex;
	Sound hitsound;
	Vector2f v;
	int curframe;
	int rate;
	void animate();

public:
	Ball();
	void move(float dt);
	void draw(RenderWindow&);
	void setPosition(float x, float y);
	Vector2f getPosition();
};

class Paddle
{
	RectangleShape shape;
	Texture tex;

public:
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

