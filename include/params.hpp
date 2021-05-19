#pragma once

namespace Params {
const double BULLET_POWER_UNIT = 50;
const int WALL_WIDTH = 2;
const int PATH_WIDTH = 2;
const int ACTUAL_CELL_SIZE = 16;
const int NUM_CELLS_X = 10;
const int NUM_CELLS_Y = 10;
const int MAX_HP = 1000;
const int MAX_BOT_HP = 100;

const int POWERUP_TIME = 300;
const int GAME_DURATION = 180;
const int HP_INCREASE = 250;
const int SCREEN_WIDTH =
    (2 + NUM_CELLS_X * PATH_WIDTH + (NUM_CELLS_X - 1) * WALL_WIDTH) *
    ACTUAL_CELL_SIZE;
const int SCREEN_HEIGHT =
    (2 + NUM_CELLS_Y * PATH_WIDTH + (NUM_CELLS_Y - 1) * WALL_WIDTH) *
    ACTUAL_CELL_SIZE;
const int MAX_BOTS = 6;
const int WIDTH_OFFSET = ACTUAL_CELL_SIZE * 10;
const int TOTAL_SCREEN_WIDTH = WIDTH_OFFSET + SCREEN_WIDTH;

};  // namespace Params