#include "Pong.h"

#include <cmath>
#include <sstream>

#define PI 3.14159265358979323846f

void Pong::update_state(float dt)
{
	ball.move(dt);
	player1.move(dt);
}

void Pong::render_frame()
{
	window.clear(Color(0, 0, 255));

	Font font;
	font.loadFromFile("font.ttf");
	Text text;
	text.setFont(font);
	text.setCharacterSize(50);
	text.setString("Hello, Pong!");
	text.setFillColor(Color::Green);
	text.setPosition(50, 300);
	window.draw(text);

	// draw background
	window.draw(rail1);
	window.draw(rail2);

	ball.draw(window);
}

Pong::Pong()
{
	window.create(VideoMode(RES_WIDTH, RES_HEIGHT), GAMETILE);
	ball.setPosition(RES_WIDTH / 2.0f, RES_HEIGHT / 2.0f);

	railtex.loadFromFile("sprites/rail.png");

	rail1.setSize({ 14, RES_HEIGHT });
	rail1.setOrigin({ 7, RES_HEIGHT / 2.0f });
	rail1.setTexture(&railtex);
	rail1.setPosition({ 36, RES_HEIGHT / 2.0f });

	rail2.setSize({ 14, RES_HEIGHT });
	rail2.setOrigin({ 7, RES_HEIGHT / 2.0f });
	rail2.setTexture(&railtex);
	rail2.setPosition({ RES_WIDTH - 36, RES_HEIGHT / 2.0f });
}


Pong::~Pong()
{
}

int Pong::run()
{
	Clock clock;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();

		update_state(dt);
		render_frame();
		window.display();
	}
	return 0;
}

void Ball::animate()
{
	std::ostringstream filename;
	filename << "sprites/bomb 00" << (curframe >= 10 ? "" : "0") << curframe << ".png";
	tex.loadFromFile(filename.str());
	shape.setTexture(&tex);
	curframe = (curframe + 1) % 16;
}

Ball::Ball()
{
	shape.setRadius(BALL_R);
	shape.setOrigin({ BALL_R, BALL_R });
	curframe = 0;
	rate = RATE;

	// random initial angle between 0-180
	srand((unsigned)time(nullptr));
	float angle = (float)(rand() % 180) / 180.0f * PI;
	v = { BALL_SPEED * cosf(angle), BALL_SPEED * sinf(angle) };
}

void Ball::move(float dt)
{
	Vector2f pos = shape.getPosition();
	if ((pos.x <= BALL_CR && v.x < 0) || (pos.x >= RES_WIDTH - BALL_CR && v.x > 0)) v.x = -v.x;
	if ((pos.y <= BALL_CR && v.y < 0) || (pos.y >= RES_HEIGHT - BALL_CR && v.y > 0)) v.y = -v.y;
	shape.setPosition(pos + v);
}

void Ball::draw(RenderWindow &w)
{
	++rate;
	if (rate / RATE) { 
		animate();
		rate = 0;
	}
	w.draw(shape);
}

void Ball::setPosition(float x, float y)
{
	shape.setPosition({ x, y });
}

Vector2f Ball::getPosition()
{
	return shape.getPosition();
}

Paddle::Paddle()
{
	shape.setSize({ PADDLE_W, PADDLE_H });
	shape.setOrigin({ PADDLE_W / 2.0f, PADDLE_H / 2.0f });
}

void Paddle::move(float dt)
{
}

void Paddle::draw(RenderWindow &w)
{
	w.draw(shape);
}

void Paddle::setPosition(float x, float y)
{
	shape.setPosition({ x, y });
}

Vector2f Paddle::getPosition()
{
	return shape.getPosition();
}
