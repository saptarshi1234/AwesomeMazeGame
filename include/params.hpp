#pragma once

namespace Params {
const int WALL_WIDTH = 1;
const int PATH_WIDTH = 3;

const int CELLS_W = 20;
const int CELLS_H = 20;

const int SCREEN_WIDTH = (CELLS_W * (WALL_WIDTH + PATH_WIDTH) + 1) * 10;
const int SCREEN_HEIGHT = (CELLS_W * (WALL_WIDTH + PATH_WIDTH) + 1) * 10;
};  // namespace Params