#include "Pong.h"

#include <cmath>

#define PI 3.14159265358979323846

void Pong::update_state(float dt)
{
	ball.move(dt);
}

void Pong::render_frame()
{
	window.clear(Color(0, 0, 255));
	window.draw(ball.shape);

	Font font;
	font.loadFromFile("font.ttf");
	Text text;
	text.setFont(font);
	text.setCharacterSize(50);
	text.setString("Hello, cowboy!");
	text.setFillColor(Color::Green);
	text.setPosition(50, 300);
	window.draw(text);
}

Pong::Pong()
{
	window.create(VideoMode(RES_WIDTH, RES_HEIGHT), GAMETILE);
	ball.shape.setPosition({ RES_WIDTH / 2.0f, RES_HEIGHT / 2.0f });
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

Ball::Ball()
{
	shape.setRadius(BALL_R);
	shape.setOrigin({ BALL_R, BALL_R });

	// random initial angle between 0-180
	srand((unsigned)time(nullptr));
	float angle = (float)(rand() % 180) / 180.0f * PI;
	v = { cosf(angle), sinf(angle) };
}

void Ball::move(float dt)
{
	Vector2f pos = shape.getPosition();
	pos += v;
	if ((pos.x <= BALL_R && v.x < 0) || (pos.x >= RES_WIDTH - BALL_R && v.x > 0)) v.x = -v.x;
	if ((pos.y <= BALL_R && v.y < 0) || (pos.y >= RES_HEIGHT - BALL_R && v.y > 0))v.y = -v.y;
	shape.setPosition(pos);
}

Paddle::Paddle()
{
	shape.setSize({ PADDLE_W, PADDLE_H });
	shape.setOrigin({ PADDLE_W / 2.0f, PADDLE_H / 2.0f });
}
