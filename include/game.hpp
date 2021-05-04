#pragma once

#include <vector>

#include "client.hpp"
#include "entity.hpp"
#include "maze.hpp"
#include "player.hpp"
#include "server.hpp"
#include "window.hpp"

class Game {
 private:
  WindowManager win;
  bool running;
  bool isServer;
  Maze maze;
  Server server;
  Client client;
  std::vector<Direction> dkey_stack;

 public:
  Game(WindowManager win, bool isServer, char* ip);

  void initialize();
  void loadGame();
  void update();
  void handleEvents();
  void render();
  void wait();
  void quit();
  bool isRunning();

  Player player1, player2;
  std::vector<Bullet> bullets;
};
