#include "raygui.h"

#include "Screens.hpp"

void ScreenHome::ui()
{
	const Vector2 wh{GetScreenWidth() / 5, GetScreenHeight() / 10};
	const Rectangle playButtonBounds{
		GetRenderWidth() / 2 - wh.x / 2,
		GetRenderHeight() / 2 - wh.y / 2,
		wh.x,
		wh.y
	};

	if (GuiButton(playButtonBounds, "Play"))
	{
		requestScreenChange(ScreenType::game);
	}
}

void ScreenGame::tick()
{
	if (IsKeyPressed(KEY_R) && !m_game.running()) m_game = Game();
	m_game.input();

	m_counter = (m_counter + 1) % delay;
	if (m_counter == 0 && m_game.running())
	{
		m_game.tick();
	}
}

void ScreenGame::sound()
{
	if (m_counter == 0 && m_game.running())
	{
		m_game.sound();
	}
}

void ScreenGame::draw()
{
	m_game.draw();
}

void ScreenGame::ui()
{
}