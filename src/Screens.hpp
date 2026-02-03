#pragma once

#include <assert.h>
#include <vector>

#include "ClassyEnum.hpp"
#include "Game.hpp"

CLASSY_ENUM(ScreenType, uint8_t,
	home,
	settings,
	game,

	COUNT
)

class Screen
{
public:

	Screen() = default;
	virtual ~Screen() = default;

	virtual void tick(){};
	virtual void sound(){};
	virtual void draw(){};
	virtual void ui(){};
	
	bool shouldChangeScreen() { return m_hasRequestedChange; }
	ScreenType requestedScreen() { return m_requestedScreen; }
	void clearScreenRequestFlag() { m_hasRequestedChange = false; }

protected:

	void requestScreenChange(ScreenType screenType)
	{
		assert(screenType != ScreenType::COUNT);

		m_requestedScreen = screenType;
		m_hasRequestedChange = true;
	}

private:

	ScreenType m_requestedScreen{};
	bool m_hasRequestedChange{};
};

class ScreenHome : public Screen
{
public:

	ScreenHome() = default;
	virtual ~ScreenHome() = default;

	virtual void ui() override;
};

class ScreenSettings : public Screen
{
public:

	ScreenSettings() = default;
	virtual ~ScreenSettings() = default;
};

class ScreenGame : public Screen
{
public:

	ScreenGame() = default;
	virtual ~ScreenGame() = default;

	virtual void tick() override;
	virtual void sound() override;
	virtual void draw() override;
	virtual void ui() override;

private:

	static constexpr int32_t delay = 10;
	Game m_game;
	int32_t m_counter = 0;
};