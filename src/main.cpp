#include <list>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "raylib.h"

enum class Direction
{
	up, down, left, right
};

struct GameState
{

};

GameState initGameState();
bool move(std::list<Vector2>& body, Direction dir, Vector2& fruit, Vector2 boardDimensions);
Vector2 newFruit(const std::list<Vector2>& body, Vector2 boardDimensions);

//todo: buffer inputs

int main()
{
	srand(time(0));
	const int w = 400, h = w;

	// move all that into game state struct and make r actually reset the game
	std::list<Vector2> body = { {0,0}, {0,1}, {0,2} };
	Direction movementDirection = Direction::right;
	Direction lastMove = movementDirection;
	Vector2 boardDimensions{10, 10};
	Vector2 fruit = newFruit(body, boardDimensions);
	bool gameRunning = true;
	GameState state = initGameState();

	InitWindow(w, h, "Test circle");
	SetTargetFPS(60);
	const int delay = 10;
	int counter = 0;

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_W) && lastMove != Direction::down) movementDirection = Direction::up;
		if (IsKeyPressed(KEY_S) && lastMove != Direction::up) movementDirection = Direction::down;
		if (IsKeyPressed(KEY_A) && lastMove != Direction::right) movementDirection = Direction::left;
		if (IsKeyPressed(KEY_D) && lastMove != Direction::left) movementDirection = Direction::right;
		if (IsKeyPressed(KEY_R) && !gameRunning) state = initGameState();

		counter = (counter + 1) % delay;
		if (counter == 0 && gameRunning)
		{
			gameRunning = move(body, movementDirection, fruit, boardDimensions);
			lastMove = movementDirection;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		Color partColor{0, 255, 0, 255};
		for (auto [x, y] : body)
		{
			DrawRectangle(
				x * w / boardDimensions.x,
				y * h / boardDimensions.y,
				w / boardDimensions.x,
				h / boardDimensions.y,
				partColor
			);
			partColor.g -= 2;
		}

		DrawRectangle(
			fruit.x * w / boardDimensions.x,
			fruit.y * h / boardDimensions.y,
			w / boardDimensions.x,
			h / boardDimensions.y,
			RED
		);

		DrawText(std::to_string(body.size() - 3).c_str(), 0, 0, 20, WHITE);

		if (!gameRunning)
		{
			std::string gameOver = "Game Over!";
			float fontSize = 40;
			DrawText(
				gameOver.c_str(),
				w / 2 - MeasureText(gameOver.c_str(), fontSize) / 2,
				h / 2 - 40,
				fontSize,
				WHITE
			);
		}

		EndDrawing();
	}

	CloseWindow();
}

GameState initGameState()
{
	return {};
}

bool move(std::list<Vector2>& body, Direction dir, Vector2& fruit, Vector2 boardDimensions)
{
	Vector2 newPosition = body.front();
	switch (dir)
	{
	case Direction::up: newPosition.y -= 1; break;
	case Direction::down: newPosition.y += 1; break;
	case Direction::left: newPosition.x -= 1; break;
	case Direction::right: newPosition.x += 1; break;
	}

	newPosition.x = ((int)newPosition.x + (int)boardDimensions.x) % (int)boardDimensions.x;
	newPosition.y = ((int)newPosition.y + (int)boardDimensions.y) % (int)boardDimensions.y;

	body.push_front(newPosition);

	bool foundFruit = newPosition.x == fruit.x && newPosition.y == fruit.y;
	if (foundFruit)
	{
		fruit = newFruit(body, boardDimensions);
	}

	for (auto& pos : body)
	{
		if ((pos.x == newPosition.x && pos.y == newPosition.y) && &pos != &body.front())
		{
			return false;
		}
	}

	if (!foundFruit)
	{
		body.pop_back();
	}
	return true;
}

Vector2 newFruit(const std::list<Vector2>& body, Vector2 boardDimensions)
{
	Vector2 out{};
	bool good = false;

	while (!good)
	{
		out = {(float)(rand() % (int)boardDimensions.x), (float)(rand() % (int)boardDimensions.y)};

		good = true;
		for (auto pos : body)
		{
			if (pos.x == out.x && pos.y == out.y)
			{
				good = false;
				break;
			}
		}
	}

	return out;
}