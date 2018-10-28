#pragma once

class Ball
{
	friend class Pong;
	friend class paddle;
};

class paddle
{
	friend class Pong;
};

class Pong
{
public:
	Pong();
	~Pong();
};

