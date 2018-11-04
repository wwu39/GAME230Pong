#include "Pong.h"

#include <cmath>

#define PI 3.14159265358979323846f

int Pong::runtimes = 0; // static var

void Pong::update_state(float dt)
{
	Vector2f bpos = ball.getPosition();
	Vector2f p1pos = player1.getPosition();
	Vector2f p2pos = player2.getPosition();
	player1.move(dt, bpos);
	player2.move(dt, bpos);
	if (ball.status == NONE) if (ball.move(dt, p1pos, p2pos) == HIT) {
		if (bpos.x < RES_WIDTH / 2.0f) {
			player1.status = FIRE;
			player1.ballv = ball.v;
			player1.playCannonFireSound();
		}
		if (bpos.x > RES_WIDTH / 2.0f) {
			player2.status = FIRE;
			player2.ballv = ball.v;
			player2.playCannonFireSound();
		}
	}
	std::ostringstream scores;
	scores << ball.lscr << " : " << ball.rscr;
	if (ball.lscr == WINSCORE) winner = PLAYER1;
	if (ball.rscr == WINSCORE) winner = PLAYER2;
	text.setString(scores.str());
}

void Pong::ending_state(float dt)
{
	++rate;
	if (rate / RATE) {
		// explosion
		float x = (winner == PLAYER1 ? RES_WIDTH - 28.0f : 28.0f);
		for (int i = 0; i < 6; ++i) {
			explosions[i].setPosition({ x, 50.0f + 100.0f * i });
			explosions[i].setTexture(&exptex[curexpfr]);
		}
		++curexpfr;
		if (curexpfr == 28) { // single explosion ends
			curexpfr = 0;
			for (int i = 0; i < 6; ++i) explosions[i].setTexture(&exptex[curexpfr]);
			if (exptimes < 1) {
				if (sound.getStatus() != SoundSource::Playing) sound.play();
				if (cheer.getStatus() != SoundSource::Playing) cheer.play();
			}
			++exptimes;
		}
		if (exptimes == 2) { // two explosions end
			status = ENDING;
			exptimes = 0;
		}
		rate = 0;
	}
	std::ostringstream str;
	str << "PLAYER ";
	if (winner == PLAYER1) str << "1";
	if (winner == PLAYER2) str << "2";
	str << std::endl << "  WINS";
	text.setString(str.str());
	text.setCharacterSize(100);
	text.setLineSpacing(0.75);
}

void Pong::render_frame(RenderWindow& window)
{
	window.clear(Color(0, 0, 255));

	// draw background
	window.draw(background);
	if (winner != PLAYER2 || status != ENDING) window.draw(rail1);
	if (winner != PLAYER1 || status != ENDING) window.draw(rail2);
	window.draw(text);

	if (ball.status == NONE) {
		if (winner==NO_WINNER) ball.draw(window);
		if (winner != PLAYER2 || status != ENDING) player1.draw(window);
		if (winner != PLAYER1 || status != ENDING) player2.draw(window);
	}
	if (ball.status == EXPLODE) {
		if (winner != PLAYER2 || status != ENDING) player1.draw(window);
		if (winner != PLAYER1 || status != ENDING) player2.draw(window);
		if (winner == NO_WINNER) ball.draw(window);
	}

	if (winner != NO_WINNER) for (int i = 0; i < 6; ++i)
		window.draw(explosions[i]);
	if (status == ENDING) {
		window.draw(cursor);
		window.draw(options);
	}
}

Pong::Pong(int numOfPlayers)
{
	if (numOfPlayers == 2) player2.isAI = false; // 1 or 2 players
	ball.setPosition(RES_WIDTH / 2.0f, RES_HEIGHT / 2.0f); // ball init pos
	// background
	bgtex.loadFromFile("sprites/bg.jpg");
	background.setSize({ RES_WIDTH, RES_HEIGHT });
	background.setTexture(&bgtex);
	// 2 railways
	railtex.loadFromFile("sprites/rail.png");
	rail1.setSize({ 28, RES_HEIGHT });
	rail1.setOrigin({ 14, RES_HEIGHT / 2.0f });
	rail1.setTexture(&railtex);
	rail1.setPosition({ 36, RES_HEIGHT / 2.0f });
	rail2.setSize({ 28, RES_HEIGHT });
	rail2.setOrigin({ 14, RES_HEIGHT / 2.0f });
	rail2.setTexture(&railtex);
	rail2.setPosition({ RES_WIDTH - 36, RES_HEIGHT / 2.0f });
	// score text
	font.loadFromFile("font.ttf");
	text.setFont(font);
	text.setCharacterSize(200);
	text.setString("0 : 0");
	text.setFillColor(Color::Green);
	text.setOrigin({ 215, 120 });
	text.setPosition(RES_WIDTH / 2.0f, RES_HEIGHT / 2.0f);
	// option text
	options.setFont(font);
	options.setCharacterSize(50);
	options.setFillColor(Color::Green);
	options.setPosition(300, 350);
	std::ostringstream str;
	str << "PLAY AGAIN" << std::endl << "MENU" << std::endl << "EXIT";
	options.setString(str.str());
	// cursor
	cursor.setRadius(BALL_R);
	cursor.setOrigin({ BALL_R, BALL_R });
	curtex.loadFromFile("sprites/bomb 0014.png");
	cursor.setTexture(&curtex);
	cursor.setPosition(265, 375);

	// initialize losing explosions
	for (int i = 0; i < 27; ++i) {
		std::ostringstream filename;
		filename << "sprites/explode 00" << (i >= 10 ? "" : "0") << i << ".png";
		exptex[i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 6; ++i) {
		explosions[i].setSize({ 120, 134 });
		explosions[i].setOrigin({ 60, 67 });
		explosions[i].setTexture(&exptex[0]);
	}

	// explosion sound
	buf.loadFromFile("sound/explosion.wav");
	sound.setBuffer(buf);
	// option switching sound
	buf2.loadFromFile("sound/option.wav");
	optionswitch.setBuffer(buf2);
	// cheer sound
	buf3.loadFromFile("sound/cheer.wav");
	cheer.setBuffer(buf3);

	++runtimes;
}


Pong::~Pong()
{
}

int Pong::run(RenderWindow& window)
{
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
		if (winner == NO_WINNER) update_state(dt);
		else if(status != ENDING) ending_state(dt);
		else {
			++presskeyrate;
			if (presskeyrate / (RATE)) {
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
				presskeyrate = 0;
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter) || Keyboard::isKeyPressed(Keyboard::Space)) {
				Vector2f pos = cursor.getPosition();
				if (pos.y == 375.0f) return REPEAT; // repeat
				if (pos.y == 425.0f) return MENU; // menu
				if (pos.y == 475.0f) return EXIT; // exit
			}
		}
		render_frame(window);
		window.display();
	}
	return EXIT;
}

void Ball::animate()
{
	shape.setTexture(&tex[curframe]);
	curframe = (curframe + 1) % 16;
}

void Ball::explode()
{
	explosion.setTexture(&exptex[curexpfr]);
	++curexpfr;
	if (curexpfr == 28) { // explodsion ends
		status = NONE;
		curexpfr = 0;
		//reset explosion texture
		explosion.setTexture(&exptex[curexpfr]);
		// reset the ball
		srand((unsigned)time(nullptr));
		float angle = (0.75f + float(rand() % 50 + 1) / 100.0f) * PI;
		v = { BALL_SPEED * cosf(angle), BALL_SPEED * sinf(angle) };
		if (lastWin == P2) v.x = -v.x;
		shape.setPosition({ RES_WIDTH / 2.0f, RES_HEIGHT / 2.0f });
		reload.play();
	}
}

void Ball::playRandomHitsound()
{
	srand((unsigned)time(nullptr) * unsigned int(v.x));
	std::ostringstream filename;
	int idx = 1 + rand() % 10;
	filename << "sound/metalhit(" << idx << ").wav";
	buf.loadFromFile(filename.str());
	hitsound.setBuffer(buf);
	if (hitsound.getStatus() != SoundSource::Playing) hitsound.play();
}

void Ball::playLoadHitsound()
{
	srand((unsigned)time(nullptr) * unsigned int(v.y));
	std::ostringstream filename;
	int idx = 1 + rand() % 6;
	filename << "sound/load(" << idx << ").wav";
	buf.loadFromFile(filename.str());
	hitsound.setBuffer(buf);
	if (hitsound.getStatus() != SoundSource::Playing) hitsound.play();
}

Ball::Ball()
{
	shape.setRadius(BALL_R);
	shape.setOrigin({ BALL_R, BALL_R });
	explosion.setSize({ 120, 134 });
	explosion.setOrigin({ 60, 67 });
	curframe = 0;
	rate = RATE;

	// random initial angle between 0-180
	srand((unsigned)time(nullptr));
	float angle = (0.75f + float(rand() % 50 + 1) / 100.0f) * PI;
	v = { BALL_SPEED * cosf(angle), BALL_SPEED * sinf(angle) };

	// int ball texture
	for (int i = 0; i < 16; ++i) {
		std::ostringstream filename;
		filename << "sprites/bomb 00" << (i >= 10 ? "" : "0") << i << ".png";
		tex[i].loadFromFile(filename.str());
	}

	// init exp texture
	for (int i = 0; i < 27; ++i) {
		std::ostringstream filename;
		filename << "sprites/explode 00" << (i >= 10 ? "" : "0") << i << ".png";
		exptex[i].loadFromFile(filename.str());
	}
	explosion.setTexture(&exptex[0]);

	buf2.loadFromFile("sound/reload.wav");
	reload.setBuffer(buf2);
	reload.play();
}

int Ball::move(float dt, Vector2f p1pos, Vector2f p2pos)
{
	Vector2f pos = shape.getPosition();
	int ret = 0;
	if (pos.x > PADDLE_LOC && pos.x < RES_WIDTH - PADDLE_LOC) {
		// hit a wall
		if ((pos.y <= BALL_CR && v.y < 0) || (pos.y >= RES_HEIGHT - BALL_CR && v.y > 0)) {
			v.y = -v.y;
			playRandomHitsound();
		}
		// hit a paddle from left or right
		if ((p1pos.y - PADDLE_H / 2.0f <= pos.y && pos.y <= p1pos.y + PADDLE_H / 2.0f && pos.x - BALL_CR <= p1pos.x && v.x < 0)
			|| (p2pos.y - PADDLE_H / 2.0f <= pos.y && pos.y <= p2pos.y + PADDLE_H / 2.0f && pos.x + BALL_CR >= p2pos.x && v.x > 0)) {
			v.x = -v.x;
			// The angle at which it bounces off depends on where on the paddle the ball hit.
			if (v.x > 0.0f) v.x += (2 * ((p2pos.y + PADDLE_H / 2.0f) - pos.y) / PADDLE_H * vinc.x);
			if (v.x < 0.0f) v.x -= (2 * ((p1pos.y + PADDLE_H / 2.0f) - pos.y) / PADDLE_H * vinc.x);
			if (v.y > 0.0f) v.y += (2 * (pos.y - (p2pos.y - PADDLE_H / 2.0f)) / PADDLE_H * vinc.y);
			if (v.y < 0.0f) v.y -= (2 * (pos.y - (p1pos.y - PADDLE_H / 2.0f)) / PADDLE_H * vinc.y);
			playLoadHitsound();
			ret = HIT;
		}
	}
	else { 
		status = EXPLODE;
		explosion.setPosition(pos);
		buf.loadFromFile("sound/explosion.wav");
		hitsound.setBuffer(buf);
		if (hitsound.getStatus() != SoundSource::Playing) hitsound.play();
		if (pos.x < RES_WIDTH / 2.0f) { ++rscr; lastWin = P2; }
		if (pos.x > RES_WIDTH / 2.0f) { ++lscr; lastWin = P1; }
	}
	shape.setPosition(pos + v);
	return ret;
}

void Ball::draw(RenderWindow &w)
{
	++rate;
	if (rate / RATE) { 
		if (status == NONE) animate();
		if (status == EXPLODE) explode();
		rate = 0;
	}
	if (status == NONE) w.draw(shape);
	if (status == EXPLODE) w.draw(explosion);
}

void Ball::setPosition(float x, float y)
{
	shape.setPosition({ x, y });
}

Vector2f Ball::getPosition()
{
	return shape.getPosition();
}

void Paddle::animate()
{
	shape.setTexture(&tex[curframe]);
	if (!reverse) ++curframe; else --curframe;
	if (side == RIGHT) {
		if (curframe == 37) reverse = true;
		if (curframe == 19) reverse = false;
	}
	else {
		if (curframe == 18) reverse = true;
		if (curframe == 0) reverse = false;
	}
}

void Paddle::fire()
{
	// decide which frame to use
	float angle = acos(-ballv.y / sqrt(ballv.x * ballv.x + ballv.y * ballv.y));
	if (side == LEFT) {
		curframe = int(angle * 18.0f / PI);
	}
	if (side == RIGHT) {
		curframe = int(angle * 18.0f / PI) + 19;
	}
	shape.setTexture(&tex[curframe + 38]);
	++curfrfr;
	// switch back to NONE status
	if (curfrfr == 5) { // explodsion ends
		status = NONE;
		curfrfr = 0;
	}
}

Paddle::Paddle(int side, bool isAI)
{
	this->isAI = isAI;
	this->side = side;
	shape.setSize({ PADDLE_W, PADDLE_H });
	shape.setOrigin({ PADDLE_W / 2.0f, PADDLE_H / 2.0f });
	shape.setPosition({ float(isAI ? RES_WIDTH - PADDLE_LOC : PADDLE_LOC), RES_HEIGHT / 2.0f });

	// init texture
	for (int i = 0; i < 76; ++i) {
		std::ostringstream filename;
		filename << "sprites/paddle 00" << (i >= 10 ? "" : "0") << i << ".png";
		tex[i].loadFromFile(filename.str());
	}
	curframe = side == RIGHT ? 19 : 0;
	shape.setTexture(&tex[curframe]);
	
	rate = RATE;
	reverse = false;
}

void Paddle::move(float dt, Vector2f bpos)
{
	Vector2f pos = shape.getPosition();
	if (!isAI) {
		if (side == LEFT) {
			if (Keyboard::isKeyPressed(Keyboard::W) && pos.y >= 53)
				pos.y -= PADDLE_SPEED * dt;
			if (Keyboard::isKeyPressed(Keyboard::S) && pos.y <= RES_HEIGHT - 53)
				pos.y += PADDLE_SPEED * dt;
		}
		if (side == RIGHT) {
			if (Keyboard::isKeyPressed(Keyboard::Up) && pos.y >= 53)
				pos.y -= PADDLE_SPEED * dt;
			if (Keyboard::isKeyPressed(Keyboard::Down) && pos.y <= RES_HEIGHT - 53)
				pos.y += PADDLE_SPEED * dt;
		}
	}
	else if (pos.x - bpos.x < RES_WIDTH * 0.75) {
		if (pos.y > bpos.y && pos.y >= 53)
			pos.y -= PADDLE_SPEED * dt;
		if (pos.y < bpos.y && pos.y <= RES_HEIGHT - 53)
			pos.y += PADDLE_SPEED * dt;
	}
	shape.setPosition(pos);
}

void Paddle::draw(RenderWindow &w)
{
	++rate;
	if (rate / RATE) {
		if (status == NONE) animate();
		if (status == FIRE) fire();
		rate = 0;
	}
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

void Paddle::playCannonFireSound()
{
	buf.loadFromFile("sound/cannonfire.wav");
	firesound.setBuffer(buf);
	if (firesound.getStatus() != SoundSource::Playing) firesound.play();
}
