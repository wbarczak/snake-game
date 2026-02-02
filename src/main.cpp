#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "Game.hpp"

int main()
{
	srand(time(0));
	const int32_t w = 720, h = w;
	const int32_t boardW = 10, boardH = boardW;
	const int delay = 10;

	InitWindow(w, h, "Snake Game");
	SetTargetFPS(60);
	InitAudioDevice();

	constexpr Color k_snakeColor = PURPLE;

	Sound pickup = LoadSound("fruit_pickup.wav");
	Sound gameOver = LoadSound("game_over.wav");
	Game game(boardW, boardH, k_snakeColor);

	int counter = 0;

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_R) && !game.running()) game = Game(boardW, boardH, k_snakeColor);
		game.input();

		counter = (counter + 1) % delay;
		if (counter == 0 && game.running())
		{
			game.tick();
			game.sound(pickup, gameOver);
		}

		BeginDrawing();
		ClearBackground(BLACK);

		game.draw();

		EndDrawing();
	}

	UnloadSound(pickup);
	UnloadSound(gameOver);
	CloseAudioDevice();
	CloseWindow();
}