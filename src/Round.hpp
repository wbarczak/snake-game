#pragma once

#include <stdint.h>
#include <list>
#include <deque>

class Round
{
public:

	static constexpr int32_t baseWidthHeight = 10;

	Round(int32_t width = baseWidthHeight, int32_t height = baseWidthHeight, Color snakeColor = GREEN) :
		m_boardDimensions{width, height},
		m_snakeColor(snakeColor)
	{
		m_fruit = randomFruit();
	}

	~Round()
	{
	}

	bool running() const { return m_lastMoveResult != MoveResult::no_op; }
	bool won() const { return m_body.size() >= m_boardDimensions.x * m_boardDimensions.y; }
	int32_t score() const { return m_body.size() - 3; }
	
	bool input();
	void tick();
	void sound(Sound pickup, Sound gameOver);
	void draw() const;

private:

	struct Position { int32_t x, y; };
	struct Fruit { Position pos; bool exists; };
	enum class Direction : uint8_t {up, down, left, right};
	enum class MoveResult : uint8_t {ok, fruit, fail, no_op};

	Fruit randomFruit();
	Position applyDirection(Position position, Direction direction);
	bool previousInputIsOpposite(Direction direction);

	std::deque<Direction> m_inputs{Direction::right};
	std::list<Position> m_body{{2, 0}, {1, 0}, {0, 0}};
	Fruit m_fruit{{-1, -1}, true};
	Position m_boardDimensions{ 10, 10 };
	Direction m_currentDirection = Direction::right;
	Color m_snakeColor = GREEN;
	MoveResult m_lastMoveResult = MoveResult::ok;
};