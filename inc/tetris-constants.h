#pragma once

#include "raylib.h"

constexpr double MOVE_LEFT_RIGHT_SPEED_IN_SECONDS = 0.1;
constexpr double MOVE_DOWN_SPEED_IN_SECONDS = 0.05;
constexpr double GRAVITY_SPEED_IN_SECONDS = 0.5;

constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 20;
constexpr int BLOCK_SIZE = 30;
constexpr Color BOARD_COLOR = Color{0, 0, 20, 150};
constexpr Color PREVIEW_COLOR = Color{0, 0, 10, 210};

constexpr int SCORE[] = {0, 40, 100, 300, 1200}; // 0, 1, 2, 3, 4 lines cleared
