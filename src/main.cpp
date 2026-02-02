#include <list>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "raylib.h"

#include "Game.hpp"

int main()
{
	srand(time(0));
	const int w = 720, h = w;

	InitWindow(w, h, "Snake Game");
	SetTargetFPS(60);
	InitAudioDevice();

	Game game;

	const int delay = 10;
	int counter = 0;

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_R) && !game.running()) game = Game();
		game.input();

		counter = (counter + 1) % delay;
		if (counter == 0 && game.running())
		{
			game.tick();
			game.sound();
		}

		BeginDrawing();
		ClearBackground(BLACK);

		game.draw();

		EndDrawing();
	}

	CloseAudioDevice();
	CloseWindow();
}