#pragma once

#include <stdint.h>
#include <list>

class Game
{
public:

	Game() : m_fruit(randomFruit())
	{
	}

	~Game()
	{
		UnloadSound(m_pickup);
		UnloadSound(m_gameOver);
	}

	bool running() { return m_lastMoveResult != MoveResult::no_op; }
	
	void input();
	void tick();
	void sound();
	void draw();

private:

	struct Position { int32_t x, y; };
	struct Fruit { Position pos; bool exists; };
	enum class Direction : uint8_t {up, down, left, right};
	enum class MoveResult : uint8_t {ok, fruit, fail, no_op};

	Fruit randomFruit();
	Position applyDirection(Position position, Direction direction);

	static constexpr Position m_boardDimensions{10, 10};
	Sound m_pickup = LoadSound("fruit_pickup.wav");
	Sound m_gameOver = LoadSound("game_over.wav");
	std::list<Position> m_body{{4, 5}, {3, 5}, {2, 5}};
	Fruit m_fruit{};
	Direction m_direction = Direction::right;
	Direction m_lastDirection = m_direction;
	MoveResult m_lastMoveResult = MoveResult::ok;
};