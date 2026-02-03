#include <stdlib.h>
#include <time.h>
#include <memory>
#include <functional>
#include <stack>

#include "raylib.h"
#include "raygui.h"

#include "Screens.hpp"

int32_t main()
{
	std::unique_ptr<Screen>(*factory[ScreenType::COUNT])();
	factory[ScreenType::home] = []() -> std::unique_ptr<Screen> { return std::make_unique<ScreenHome>(); };
	factory[ScreenType::settings] = []() -> std::unique_ptr<Screen> { return std::make_unique<ScreenSettings>(); };
	factory[ScreenType::game] = []() -> std::unique_ptr<Screen> { return std::make_unique<ScreenGame>(); };

	std::unique_ptr<Screen> screen = factory[ScreenType::home]();

	srand(time(0));
	const int32_t k_width = 720, k_height = k_width;

	InitWindow(k_width, k_height, "Snake Game");
	SetTargetFPS(60);
	InitAudioDevice();

	GuiLoadStyle("style_cyber.rgs");
	GuiSetStyle(DEFAULT, TEXT_SIZE, GetRenderHeight() / 20);

	Global::pickup = LoadSound("fruit_pickup.wav");
	Global::gameOver = LoadSound("game_over.wav");

	while (!WindowShouldClose())
	{
		screen->tick();

		BeginDrawing();
		ClearBackground(BLACK);

		screen->draw();

		screen->ui();

		EndDrawing();

		screen->sound();

		if (screen->shouldChangeScreen())
		{
			screen = factory[screen->requestedScreen()]();
		}
	}

	UnloadSound(Global::pickup);
	UnloadSound(Global::gameOver);
	CloseAudioDevice();
	CloseWindow();
}