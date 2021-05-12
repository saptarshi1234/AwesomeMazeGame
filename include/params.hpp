#pragma once

namespace Params {
const int WALL_WIDTH = 1;
const int PATH_WIDTH = 2;
const int ACTUAL_CELL_SIZE = 20;
const int NUM_CELLS_X = 15;
const int NUM_CELLS_Y = 15;
const int SCREEN_WIDTH =
    (NUM_CELLS_X * PATH_WIDTH + (NUM_CELLS_X + 1) * WALL_WIDTH) *
    ACTUAL_CELL_SIZE;
const int SCREEN_HEIGHT =
    (NUM_CELLS_Y * PATH_WIDTH + (NUM_CELLS_Y + 1) * WALL_WIDTH) *
    ACTUAL_CELL_SIZE;
const int MAX_BOTS = 5;
};  // namespace Params