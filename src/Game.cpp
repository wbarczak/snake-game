#include <string>

#include "raylib.h"

#include "Game.hpp"

void Game::input()
{
	if (IsKeyPressed(KEY_W) && m_lastDirection != Direction::down) m_direction = Direction::up;
	if (IsKeyPressed(KEY_S) && m_lastDirection != Direction::up) m_direction = Direction::down;
	if (IsKeyPressed(KEY_A) && m_lastDirection != Direction::right) m_direction = Direction::left;
	if (IsKeyPressed(KEY_D) && m_lastDirection != Direction::left) m_direction = Direction::right;
}

void Game::tick()//snake should move after all the checks and fruit generations are performad cuz otherwise there are bugs
{
	if (m_lastMoveResult == MoveResult::fail)
	{
		m_lastMoveResult = MoveResult::no_op;
		return;
	}

	Position headPosition = applyDirection(m_body.front(), m_direction);

	bool foundFruit = m_fruit.exists && headPosition.x == m_fruit.pos.x && headPosition.y == m_fruit.pos.y;

	m_body.push_front(headPosition);

	if (foundFruit)
	{
		m_fruit = randomFruit();
		m_lastMoveResult = MoveResult::fruit;
	}
	else
	{
		m_body.pop_back();
		m_lastMoveResult = MoveResult::ok;
	}

	if (m_body.size() >= m_boardDimensions.x * m_boardDimensions.y)
	{
		m_lastMoveResult = MoveResult::fail;
	}

	for (auto& pos : m_body)
	{
		if ((pos.x == headPosition.x && pos.y == headPosition.y) && &pos != &m_body.front())
		{
			m_lastMoveResult = MoveResult::fail;
			return;
		}
	}
}

void Game::sound()
{
	switch (m_lastMoveResult)
	{
	case MoveResult::fruit:

	{
		float pitch = (rand() % 20 + 90) / 100.0f;
		float volume = (rand() % 20 + 80) / 100.0f;
		float pan = (rand() & 20 - 10) / 100.0f;
		SetSoundPitch(m_pickup, pitch);
		SetSoundVolume(m_pickup, volume);
		SetSoundPan(m_pickup, pan);
		PlaySound(m_pickup);
	}
	break;

	case MoveResult::fail:

		PlaySound(m_gameOver);
		break;

	};
}

void Game::draw()
{
	const Position tileDim{
		GetRenderWidth() / m_boardDimensions.x,
		GetRenderHeight() / m_boardDimensions.y
	};

	Color partColor{ 0, 255, 0, 255 };
	for (auto [x, y] : m_body)
	{
		DrawRectangle(
			x * tileDim.x,
			y * tileDim.y,
			tileDim.x,
			tileDim.y,
			partColor
		);
		partColor.g -= 2;
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

	if (m_body.size() == m_boardDimensions.x * m_boardDimensions.y)
	{
		return Fruit{{}, false};
	}

	while (!good)
	{
		out = {rand() % m_boardDimensions.x, rand() % m_boardDimensions.y};

		good = true;
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
	m_lastDirection = direction;

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