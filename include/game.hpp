#pragma once

#include <vector>

#include "bot.hpp"
#include "client.hpp"
#include "entity.hpp"
#include "maze.hpp"
#include "networking.hpp"
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
  CustomSocket* c_sock;
  int num_other_bullets;

  std::vector<Direction> dkey_stack;

 public:
  Game(WindowManager win, bool isServer, char* ip);

  void initialize();
  void loadGame();
  void sync();
  void update();
  void handleEvents();
  void render();
  void wait(int);
  void quit();
  bool isRunning();

  Player player1, player2;
  std::vector<Bullet> bullets;
  std::vector<Bullet> unsynced_bullets;
  std::vector<Bullet> other_bullets;
  std::vector<Bot> bots;
};
