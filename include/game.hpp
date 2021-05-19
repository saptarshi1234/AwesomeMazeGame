#pragma once

#include <vector>

#include "bot.hpp"
#include "entity.hpp"
#include "items.hpp"
#include "maze.hpp"
#include "networking.hpp"
#include "player.hpp"
#include "textures.hpp"
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
  bool single_player;
  int time_elapsed;

  std::vector<Direction> dkey_stack;
  bool space_down;

 public:
  Game();
  Game(WindowManager win, bool isServer, bool single, const char* ip);

  void initialize();
  void loadGame();
  void sync();
  void update();
  void updateBots();
  void handleEvents();
  void render();
  void wait(int);
  void quit();
  bool isRunning();
  void modifyTime(int time);

  Player player1, player2;
  std::vector<Bullet> bullets;
  std::vector<Bullet> unsynced_bullets;
  std::vector<Bot> unsynced_bots;
  std::vector<Bullet> other_bullets;
  std::vector<Item> items;
  std::vector<Bot> bots;
};
