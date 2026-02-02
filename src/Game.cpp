#include <string>

#include "raylib.h"

#include "Game.hpp"

void Game::input()
{
	if (m_inputs.size() >= 2) return;

	bool up = IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);
	bool down = IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN);
	bool left = IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT);
	bool right = IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT);

	if (up && !previousInputIsOpposite(Direction::up))
	{
		m_inputs.push_back(Direction::up);
	}
	if (down && !previousInputIsOpposite(Direction::down))
	{
		m_inputs.push_back(Direction::down);
	}
	if (left && !previousInputIsOpposite(Direction::left))
	{
		m_inputs.push_back(Direction::left);
	}
	if (right && !previousInputIsOpposite(Direction::right))
	{
		m_inputs.push_back(Direction::right);
	}
}

void Game::tick()
{
	if (m_lastMoveResult == MoveResult::fail)
	{
		m_lastMoveResult = MoveResult::no_op;
		return;
	}

	m_lastMoveResult = MoveResult::ok;

	if (!m_inputs.empty())
	{
		m_currentDirection = m_inputs.front();
		m_inputs.pop_front();
	}
	Position headPosition = applyDirection(m_body.front(), m_currentDirection);

	bool foundFruit = m_fruit.exists && headPosition.x == m_fruit.pos.x && headPosition.y == m_fruit.pos.y;

	if (foundFruit)
	{
		m_fruit = randomFruit();
		m_body.push_front(headPosition);

		if (m_body.size() >= m_boardDimensions.x * m_boardDimensions.y)
		{
			m_lastMoveResult = MoveResult::fail;
			return;
		}

		m_lastMoveResult = MoveResult::fruit;
		return;
	}

	for (auto& pos : m_body)
	{
		if ((pos.x == headPosition.x && pos.y == headPosition.y) && &pos != &m_body.back())
		{
			m_lastMoveResult = MoveResult::fail;
			return;
		}
	}

	m_body.push_front(headPosition);
	m_body.pop_back();
}

void Game::sound(Sound pickup, Sound gameOver)
{
	switch (m_lastMoveResult)
	{
	case MoveResult::fruit:

	{
		float pitch = (rand() % 20 + 90) / 100.0f;
		float volume = (rand() % 20 + 80) / 100.0f;
		float pan = (rand() & 20 - 10) / 100.0f;
		SetSoundPitch(pickup, pitch);
		SetSoundVolume(pickup, volume);
		SetSoundPan(pickup, pan);
		PlaySound(pickup);
	}
	break;

	case MoveResult::fail:

		PlaySound(gameOver);
		break;

	};
}

void Game::draw()
{
	const Position tileDim{
		GetRenderWidth() / m_boardDimensions.x,
		GetRenderHeight() / m_boardDimensions.y
	};

	Color partColor = m_snakeColor;
	for (auto [x, y] : m_body)
	{
		DrawRectangle(
			x * tileDim.x,
			y * tileDim.y,
			tileDim.x,
			tileDim.y,
			partColor
		);
		partColor.a -= 2;
	}

	if (m_fruit.exists)
	{
		DrawRectangle(
			m_fruit.pos.x * tileDim.x,
			m_fruit.pos.y * tileDim.y,
			tileDim.x,
			tileDim.y,
			RED
		);
	}

	DrawText(std::to_string(m_body.size() - 3).c_str(), 0, 0, GetRenderWidth() / 20, WHITE);

	if (m_lastMoveResult == MoveResult::no_op)
	{
		std::string gameOver = "Game Over!";
		float fontSize = GetRenderWidth() / 10;
		DrawText(
			gameOver.c_str(),
			GetRenderWidth() / 2 - MeasureText(gameOver.c_str(), fontSize) / 2,
			GetRenderHeight() / 2 - fontSize,
			fontSize,
			WHITE
		);
	}
}

Game::Fruit Game::randomFruit()
{
	Position out{};
	bool good = false;

	if (m_body.size() >= m_boardDimensions.x * m_boardDimensions.y - 1)
	{
		return Fruit{{}, false};
	}

	while (!good)
	{
		out = {rand() % m_boardDimensions.x, rand() % m_boardDimensions.y};

		good = true;
		if (m_fruit.pos.x == out.x && m_fruit.pos.y == out.y)
		{
			good = false;
			continue;
		}
		for (auto pos : m_body)
		{
			if (pos.x == out.x && pos.y == out.y)
			{
				good = false;
				break;
			}
		}
	}

	return Fruit{out, true};
}

Game::Position Game::applyDirection(Position position, Direction direction)
{
	switch (direction)
	{
	case Direction::up: position.y -= 1; break;
	case Direction::down: position.y += 1; break;
	case Direction::left: position.x -= 1; break;
	case Direction::right: position.x += 1; break;
	}

	position.x = (position.x + m_boardDimensions.x) % m_boardDimensions.x;
	position.y = (position.y + m_boardDimensions.y) % m_boardDimensions.y;

	return position;
}

bool Game::previousInputIsOpposite(Direction direction)
{
	Direction previousInput = m_inputs.empty()? m_currentDirection : m_inputs.back();
	
	return (previousInput == Direction::up && direction == Direction::down) ||
		(previousInput == Direction::down && direction == Direction::up) ||
		(previousInput == Direction::left && direction == Direction::right) ||
		(previousInput == Direction::right && direction == Direction::left);
}