#pragma once

#include <stdint.h>

#include "raylib.h"

struct Config
{
	Color snakeColor = GREEN;
	Color fruitColor = RED;
	int32_t tilemapWidth = 10;
	int32_t tilemapHeight = 10;
	int32_t delay = 10;
};