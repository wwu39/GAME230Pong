#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <sstream>

#define GAMETILE "RailDefender"
#define WINSCORE 5
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

// return status
#define EXIT 0
#define REPEAT 1
#define MENU 2

enum STATUS { NONE = 0, EXPLODE, FIRE, ENDING };
enum PLAYER { P1 = 0, P2 = 1 };
enum WINNER { NO_WINNER = 0, PLAYER1, PLAYER2 };

using namespace sf;

class Ball
{
	CircleShape shape;
	RectangleShape explosion;
	Texture tex;
	Texture exptex;
	SoundBuffer buf;
	Sound hitsound;
	SoundBuffer buf2;
	Sound reload;
	int curframe;
	int curexpfr = 0;
	int rate;
	const Vector2f vinc { 0.02f, 0.02f }; // was 0.1
	void animate();
	void explode();
	void playRandomHitsound();
	void playLoadHitsound();

public:
	Vector2f v;
	STATUS status = NONE;
	int lscr = 0, rscr = 0;
	PLAYER lastWin = P1;

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

	friend class Pong;
};

class Pong
{
	static int runtimes;
	Ball ball;
	Paddle player1 { LEFT, false };
	Paddle player2 { RIGHT, true }; // AI

	// background staff
	Font font;
	Text text;
	Text options;
	RectangleShape background;
	Texture bgtex;
	Texture railtex;
	RectangleShape rail1;
	RectangleShape rail2;
	RectangleShape bg;
	CircleShape cursor;
	Texture curtex;

	SoundBuffer buf;
	Sound sound;
	RectangleShape explosions[6]; // used when one loses
	Texture exptex;

	SoundBuffer buf2;
	Sound optionswitch;

	SoundBuffer buf3;
	Sound cheer;

	int rate = RATE;
	int presskeyrate = RATE;
	int curexpfr = 0;
	int exptimes = 0;
	int curcurfr = 0;

	void update_state(float dt);
	void ending_state(float dt);
	void render_frame(RenderWindow&);

public:
	STATUS status = NONE;
	WINNER winner = NO_WINNER;

	Pong(int);
	~Pong();
	int run(RenderWindow&);
};

