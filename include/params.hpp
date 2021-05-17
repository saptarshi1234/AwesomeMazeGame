#pragma once

namespace Params {
const double BULLET_POWER_UNIT = 50;
const int WALL_WIDTH = 2;
const int PATH_WIDTH = 2;
const int ACTUAL_CELL_SIZE = 20;
const int NUM_CELLS_X = 15;
const int NUM_CELLS_Y = 15;
const int MAX_HP = 1000;
const int POWERUP_TIME = 100;
const int SCREEN_WIDTH =
    (2 + NUM_CELLS_X * PATH_WIDTH + (NUM_CELLS_X - 1) * WALL_WIDTH) *
    ACTUAL_CELL_SIZE;
const int SCREEN_HEIGHT =
    (2 + NUM_CELLS_Y * PATH_WIDTH + (NUM_CELLS_Y - 1) * WALL_WIDTH) *
    ACTUAL_CELL_SIZE;
const int MAX_BOTS = 10;
};  // namespace Params