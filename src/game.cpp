#include "game.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "bullet.hpp"
#include "entity.hpp"
#include "maze.hpp"
#include "params.hpp"
#include "window.hpp"

using std::cout;
using std::endl;

Game::Game(WindowManager win, bool isServer, char *ip)
    : win(win), isServer(isServer) {
  if (isServer) {
    cout << "Server" << endl;
    server.initialize();
    server.bind();
    server.listen();
    server.accept();

    cout << server.recv() << endl;

    c_sock = &server;
  } else {
    cout << "Client" << endl;
    client.initialize();
    client.connect(ip);
    client.send("hello");

    c_sock = &client;
  }
}

void printMaze(std::vector<std::vector<int>> v) {
  std::cout << v.size() << " " << v[0].size() << "\n";
  for (auto m : v) {
    for (auto k : m) {
      if (k == 0)
        std::cout << "  ";
      else
        std::cout << "* ";
    }
    std::cout << "\n";
  }
}

std::string serialize(std::vector<std::vector<int>> v) {
  std::string s;
  for (auto row : v) {
    for (auto i : row) {
      s += std::to_string(i);
    }
  }
  return s;
}

std::vector<std::vector<int>> deserialize(std::string s) {
  std::vector<std::vector<int>> v;
  int pos = 0;
  for (int i = 0; i < Params::SCREEN_HEIGHT / Params::ACTUAL_CELL_SIZE; i++) {
    std::vector<int> row;
    for (int j = 0; j < Params::SCREEN_WIDTH / Params::ACTUAL_CELL_SIZE; j++) {
      row.push_back(s[pos++] - 48);
    }
    v.push_back(row);
  }
  return v;
}

void Game::initialize() {
  maze.setSize(Params::NUM_CELLS_X, Params::NUM_CELLS_Y);
  maze.setPathLength(Params::PATH_WIDTH);
  maze.setWallLength(Params::WALL_WIDTH);

  if (isServer) {
    maze.generate();
    maze.render();
    maze.addPadding();

    std::string s = serialize(maze.getPixelV());
    server.send(s);
    cout << s << endl;

  } else {
    auto data = client.recv();
    maze.setPixelV(deserialize(data));
    cout << data << endl;
  }
  maze.maze_tex = TextureManager::getTex(TextureID::GROUND);
}

void Game::loadGame() {
  int w = Params::ACTUAL_CELL_SIZE;
  int p = Params::PATH_WIDTH;
  int ww = Params::WALL_WIDTH;
  if (isServer) {
    player1.init({w, w, p * w, p * w});
    player2.init({w, w, p * w, p * w});
    server.send(player1.to_string());
    player2.from_string(server.recv());

  } else {
    player1.init({Params::SCREEN_WIDTH - (p + 1) * w, w, p * w, p * w});
    player2.init({Params::SCREEN_WIDTH - (p + 1) * w, w, p * w, p * w});

    client.send(player1.to_string());
    player2.from_string(client.recv());
  }

  running = true;
}

void Game::handleEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit();
      break;
    }

    if (event.type == SDL_KEYUP) {
      std::vector<Direction>::iterator it;
      switch (event.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_w:
          it = std::find(dkey_stack.begin(), dkey_stack.end(), TOP);
          break;
        case SDLK_DOWN:
        case SDLK_s:
          it = std::find(dkey_stack.begin(), dkey_stack.end(), BOTTOM);
          break;
        case SDLK_LEFT:
        case SDLK_a:
          it = std::find(dkey_stack.begin(), dkey_stack.end(), LEFT);
          break;
        case SDLK_RIGHT:
        case SDLK_d:
          it = std::find(dkey_stack.begin(), dkey_stack.end(), RIGHT);
          break;
        case SDLK_SPACE:
          space_down = false;
          break;
        default:
          break;
      }
      switch (event.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_LEFT:
        case SDLK_RIGHT: {
          if (it != dkey_stack.end()) dkey_stack.erase(it);
          if (dkey_stack.size() == 0)
            player1.stopMoving();
          else
            player1.setDirection(dkey_stack[dkey_stack.size() - 1]);
          break;
        }
      }
    }

    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
        case SDLK_w:
        case SDLK_UP:
          player1.setDirection(TOP);
          if (dkey_stack.size() != 0) {
            if (dkey_stack[dkey_stack.size() - 1] != TOP)
              dkey_stack.push_back(TOP);
          } else
            dkey_stack.push_back(TOP);
          break;
        case SDLK_s:
        case SDLK_DOWN:
          player1.setDirection(BOTTOM);
          if (dkey_stack.size() != 0) {
            if (dkey_stack[dkey_stack.size() - 1] != BOTTOM)
              dkey_stack.push_back(BOTTOM);
          } else
            dkey_stack.push_back(BOTTOM);
          break;
        case SDLK_a:
        case SDLK_LEFT:
          player1.setDirection(LEFT);
          if (dkey_stack.size() != 0) {
            if (dkey_stack[dkey_stack.size() - 1] != LEFT)
              dkey_stack.push_back(LEFT);
          } else
            dkey_stack.push_back(LEFT);
          break;
        case SDLK_d:
        case SDLK_RIGHT:
          player1.setDirection(RIGHT);
          if (dkey_stack.size() != 0) {
            if (dkey_stack[dkey_stack.size() - 1] != RIGHT)
              dkey_stack.push_back(RIGHT);
          } else
            dkey_stack.push_back(RIGHT);
          break;
        case SDLK_SPACE: {
          if (!space_down) {
            Bullet b = player1.fireBullet();
            bullets.push_back(b);
            unsynced_bullets.push_back(b);
            space_down = true;
          }
          break;
        }
        default:
          break;
      }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        Bullet b = player1.fireBullet();
        bullets.push_back(b);
        unsynced_bullets.push_back(b);
      }
    }
  }
}

void Game::update() {
  unsynced_bullets.clear();

  if (isServer) {
    for (auto bullet_it = bullets.begin(); bullet_it != bullets.end();
         bullet_it++) {
      if (bullet_it->destroyBullet()) {
        bullet_it = bullets.erase(bullet_it);
        bullet_it--;
      }
    }

    for (auto bullet_it = other_bullets.begin();
         bullet_it != other_bullets.end(); bullet_it++) {
      if (bullet_it->destroyBullet()) {
        bullet_it = other_bullets.erase(bullet_it);
        bullet_it--;
      }
    }

    // spawn bots at random
    int w = Params::ACTUAL_CELL_SIZE;
    int k = rand() % Params::MAX_BOTS;
    if (k >= bots.size()) {
      int row = rand() % Params::NUM_CELLS_X;
      int col = rand() % Params::NUM_CELLS_Y;
      Bot b;
      int x = w * (row * (Params::PATH_WIDTH + Params::WALL_WIDTH) + 1);
      int y = w * (col * (Params::PATH_WIDTH + Params::WALL_WIDTH) + 1);
      b.init({x, y, Params::PATH_WIDTH * w - 1, Params::PATH_WIDTH * w - 1});
      bots.push_back(b);
    }

    // spawn collectibles
    auto type = static_cast<Item::ItemType>(rand() % Item::numTypes);
    bool generate = rand() % 20 == 0;
    int width = Params::PATH_WIDTH * w;
    if (generate) {
      int row = rand() % Params::NUM_CELLS_X;
      int col = rand() % Params::NUM_CELLS_Y;
      int x = w * (row * (Params::PATH_WIDTH + Params::WALL_WIDTH) + 1);
      int y = w * (col * (Params::PATH_WIDTH + Params::WALL_WIDTH) + 1);

      Item item;
      item.init({x + width / 4, y + width / 4, width / 2, width / 2}, type);

      items.push_back(item);
    }
  }

  for (auto it = bots.begin(); it != bots.end(); it++) {
    if (it->getHP() <= 0 && it->explosion_status == -1) {
      it = bots.erase(it) - 1;
    }
  }
  if (player1.getHP() <= 0) {
    ;
  }

  for (int i = 0; i < bots.size(); i++) {
    if (isServer && bots[i].shouldUpdate()) {
      bots[i].update(player1.getLocation(), &maze);
    }
    if (i < 10 && bots[i].shouldFire()) {
      Bullet b = bots[i].fireBullet();
      bullets.push_back(b);
      unsynced_bullets.push_back(b);
    }
    bots[i].move(&maze);
  }

  player1.move(&maze);
  player1.updateItems();

  player2.move(&maze);
  player2.updateItems();

  for (auto it = items.begin(); it != items.end(); it++) {
    bool b1 = it->checkCollected(player1);
    if (b1) {
      player1.collectItem(*it);
      it = items.erase(it) - 1;
      continue;
    }

    bool b2 = it->checkCollected(player2);
    if (b2) {
      player1.collectItem(*it);
      it = items.erase(it) - 1;
      continue;
    }
  }

  for (auto bullet_it = bullets.begin(); bullet_it != bullets.end();
       bullet_it++) {
    bool hit = false;
    for (int i = 0; i < bots.size() + 2; i++) {
      if (i < bots.size())
        hit = bullet_it->hitTarget(bots[i]);
      else if (i == bots.size())
        hit = bullet_it->hitTarget(player1);
      else
        hit = bullet_it->hitTarget(player2);
      if (hit) break;
    }
    if (!hit) {
      bullet_it->move(&maze);
      if (!bullet_it->isMoving()) {
        bullet_it = bullets.erase(bullet_it);
        bullet_it--;
      }
    }
  }

  for (auto bullet_it = other_bullets.begin(); bullet_it != other_bullets.end();
       bullet_it++) {
    bool hit = false;
    for (int i = 0; i < bots.size(); i++) {
      if (bullet_it->hitTarget(bots[i])) {
        hit = true;
        break;
      }
    }
    if (!hit) {
      bullet_it->move(&maze);
      if (!bullet_it->isMoving()) {
        bullet_it = other_bullets.erase(bullet_it);
        bullet_it--;
      }
    }
  }
}

void Game::sync() {
  std::stringstream ss_send;
  for (Bullet &b : unsynced_bullets) {
    ss_send << b.to_string() << '\n';
  }
  c_sock->send(ss_send.str());

  std::stringstream ss_recv(c_sock->recv());
  char delim = '\n';
  std::string word;
  Bot dummy;
  dummy.init({0, 0, 0, 0});
  while (std::getline(ss_recv, word, delim)) {
    Bullet b;
    b.init({0, 0, 0, 0}, false, &dummy, 1);
    b.from_string(word);
    other_bullets.push_back(b);
  }

  c_sock->send(player1.to_string());
  player2.from_string(c_sock->recv());

  // std::stringstream ss_send;
  // for (Bot &b : bots) {
  //   ss_send << b.to_string() << '\n';
  // }
  // c_sock->send(ss_send.str());

  // std::stringstream ss_recv(c_sock->recv());
  // char delim = '\n';
  // std::string word;
  // int index = 0;
  // while (std::getline(ss_recv, word, delim)) {
  //   bots[index].from_string(word);
  // }
}

void Game::render() {
  int w = Params::ACTUAL_CELL_SIZE;
  auto maze_V = maze.getPixelV();
  for (int x = 0; x < maze_V.size(); x++) {
    for (int y = 0; y < maze_V[0].size(); y++) {
      if (maze_V[x][y] == 1)
        win.render({w * y, w * x, w, w}, {0, 0, 10, 10}, maze.maze_tex);
    }
  }
  win.render(player1);
  win.render(player2);

  for (auto &bot : bots) {
    win.render(bot);
  }

  for (auto &bullet : bullets) {
    win.render(bullet);
  }
  for (auto &b : other_bullets) {
    win.render(b);
  }
  for (auto &item : items) {
    win.render(item);
  }
}
void Game::wait(int delay) { SDL_Delay(delay); }

void Game::quit() {
  running = false;
  win.destroy();
}

bool Game::isRunning() { return running; };
