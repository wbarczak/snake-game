#include <stdlib.h>
#include <time.h>
#include <string>

#include "raylib.h"

#include "Round.hpp"

int main()
{
	srand(time(0));
	const int32_t width = 720, height = width;
	const int32_t boardW = 10, boardH = boardW;
	const int delay = 10;

	InitWindow(width, height, "Snake Game");
	SetTargetFPS(60);
	InitAudioDevice();

	constexpr Color k_snakeColor = PURPLE;

	Sound pickup = LoadSound("fruit_pickup.wav");
	Sound gameOver = LoadSound("game_over.wav");
	Round round(boardW, boardH, k_snakeColor);

	RenderTexture boardRender = LoadRenderTexture(width * 0.8, height * 0.8);

	bool paused = false;
	int counter = 0;

	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_R) && !round.running())
		{
			round = Round(boardW, boardH, k_snakeColor);
		}
		if (IsKeyPressed(KEY_P) && round.running())
		{
			paused = !paused;
			counter %= delay;
		}
		
		if (round.input())
		{
			paused = false;
		}

		if (!paused)
		{
			counter = (counter + 1) % delay;
			if (counter == 0 && round.running())
			{
				round.tick();
				round.sound(pickup, gameOver);
			}
		}

		BeginTextureMode(boardRender);
		ClearBackground(BLACK);

		round.draw();

		EndTextureMode();

		BeginDrawing();
		ClearBackground(BLACK);

		const Rectangle renderRectangle{
			0, 0, boardRender.texture.width, -boardRender.texture.height
		};

		const Rectangle destinationRectangle{
			0 + GetRenderWidth() * 0.1,
			0 + GetRenderHeight() * 0.1,
			GetRenderWidth() * 0.8,
			GetRenderHeight() * 0.8
		};

		DrawTexturePro(
			boardRender.texture,
			renderRectangle,
			destinationRectangle,
			{},
			{},
			WHITE
		);

		DrawRectangleLinesEx(destinationRectangle, GetRenderHeight() * 0.002, WHITE);

		if (!round.running())
		{
			const std::string gameOver = round.won()? "You Win!" : "Game Over!";
			const float fontSize = GetRenderWidth() / 10;
			DrawText(
				gameOver.c_str(),
				GetRenderWidth() / 2 - MeasureText(gameOver.c_str(), fontSize) / 2,
				GetRenderHeight() / 2 - fontSize,
				fontSize,
				WHITE
			);

			const std::string playAgain = "Press R to play again";
			DrawText(
				playAgain.c_str(),
				GetRenderWidth() / 2 - MeasureText(playAgain.c_str(), fontSize / 2) / 2,
				GetRenderHeight() / 2 + fontSize / 4,
				fontSize / 2,
				WHITE
			);
		}

		const std::string score = "Score: " + std::to_string(round.score());
		const float scoreFontSize = GetRenderHeight() / 15;
		DrawText(
			score.c_str(),
			GetRenderWidth() / 2 - MeasureText(score.c_str(), scoreFontSize) / 2,
			GetRenderHeight() * 0.9 + (GetRenderHeight() * 0.1 - scoreFontSize) / 2,
			scoreFontSize,
			WHITE
		);

		const std::string pauseHint = "Pause with P";
		const float pauseHintFontSize = GetRenderHeight() / 20;
		DrawText(
			pauseHint.c_str(),
			GetRenderWidth() / 2 - MeasureText(pauseHint.c_str(), pauseHintFontSize) / 2,
			(GetRenderHeight() * 0.1 - pauseHintFontSize) / 2,
			pauseHintFontSize,
			WHITE
		);

		if (paused)
		{
			Rectangle pauseRectangle{
				GetRenderWidth() / 2 - GetRenderWidth() * 0.1,
				GetRenderHeight() / 2 - GetRenderHeight() * 0.1,
				GetRenderWidth() * 0.07,
				GetRenderHeight() * 0.2
			};

			DrawRectanglePro(pauseRectangle, {}, {}, WHITE);

			pauseRectangle.x += GetRenderWidth() * 0.13;

			DrawRectanglePro(pauseRectangle, {}, {}, WHITE);
		}

		EndDrawing();
	}

	UnloadSound(pickup);
	UnloadSound(gameOver);
	UnloadRenderTexture(boardRender);

	CloseAudioDevice();
	CloseWindow();
}