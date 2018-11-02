#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#define GAMETILE "Rail Defender"
#define RES_WIDTH 800 // resolution
#define RES_HEIGHT 600 // resolution
#define BALL_R 30.0f // radius of the ball
#define BALL_CR 12.0f // radius of the ball used in collision
#define PADDLE_W 148.0f
#define PADDLE_H 132.0f
#define PADDLE_LOC 36
#define BALL_SPEED 0.2f //1.0f
#define PADDLE_SPEED 300
#define RATE 100

enum STATUS { NONE = 0, EXPLODE, FIRE };

using namespace sf;

class Ball
{
	CircleShape shape;
	RectangleShape explosion;
	Texture tex;
	Texture exptex;
	SoundBuffer buf;
	Sound hitsound;
	int curframe;
	int curexpfr = 0;
	int rate;
	const Vector2f vinc { 0.02f, 0.02f }; // was 0.1
	void animate();
	void explode();
	void playRandomHitsound();
	void playLoadHitsound();
	float length(Vector2f, Vector2f);

public:
	Vector2f v;
	STATUS status = NONE;

	Ball();
#define HIT 1
	int move(float dt, Vector2f p1pos, Vector2f p2pos);
	void draw(RenderWindow&);
	void setPosition(float x, float y);
	Vector2f getPosition();
};

#define LEFT 1
#define RIGHT 2

class Paddle
{
	RectangleShape shape;
	Texture tex;
	SoundBuffer buf;
	Sound firesound;
	int curframe;
	int curfrfr = 0;
	int rate;
	bool isAI;
	int side;
	bool reverse;
	void animate();
	void fire();

public:
	Vector2f ballv;
	STATUS status = NONE;

	Paddle(int side, bool isAI);
	void move(float dt, Vector2f bpos);
	void draw(RenderWindow&);
	void setPosition(float x, float y);
	Vector2f getPosition();
	void playCannonFireSound();
};

class Pong
{
	static int runtimes;

	Ball ball;
	Paddle player1 { LEFT, false };
	Paddle player2 { RIGHT, true }; // AI

	// background staff
	Texture railtex;
	RectangleShape rail1;
	RectangleShape rail2;
	RectangleShape bg;

	int rate;

	void update_state(float dt);
	void render_frame(RenderWindow&);

public:
	Pong();
	~Pong();
	int run(RenderWindow&);
};

