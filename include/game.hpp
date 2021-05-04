#pragma once

#include <vector>

#include "entity.hpp"
#include "maze.hpp"
#include "player.hpp"
#include "window.hpp"

class Game {
 private:
  WindowManager win;
  bool running;
  Maze maze;
  std::vector<Direction> dkey_stack;

 public:
  Game(WindowManager win);

  void initialize();
  void loadGame();
  void update();
  void handleEvents();
  void render();
  void wait();
  void quit();
  bool isRunning();

  Player player1;
  std::vector<Bullet> bullets;
};
