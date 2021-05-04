#include "game.hpp"

#include <algorithm>
#include <iostream>
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

  } else {
    cout << "Client" << endl;
    client.initialize();
    client.connect(ip);
    client.send("hello", 5);
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
  for (int i = 0; i < Params::SCREEN_HEIGHT / 10; i++) {
    std::vector<int> row;
    for (int j = 0; j < Params::SCREEN_WIDTH / 10; j++) {
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
    server.send(s, s.size());

  } else {
    auto data =
        client.recv(Params::SCREEN_HEIGHT / 10 * Params::SCREEN_WIDTH / 10);
    maze.setPixelV(deserialize(data));
  }
  maze.maze_tex = win.loadTexture("res/textures/ground.png");
}

void Game::loadGame() {
  int w = Params::ACTUAL_CELL_SIZE;
  SDL_Texture *player_tex = win.loadTexture("res/textures/player0.png");
  if (isServer) {
    player1.init({w, w, 2 * w, 2 * w}, player_tex);
    player2.init({Params::SCREEN_WIDTH - 3 * w, w, 2 * w, 2 * w}, player_tex);
  } else {
    player2.init({w, w, 2 * w, 2 * w}, player_tex);
    player1.init({Params::SCREEN_WIDTH - 3 * w, w, 2 * w, 2 * w}, player_tex);
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
          it = std::find(dkey_stack.begin(), dkey_stack.end(), TOP);
          break;
        case SDLK_DOWN:
          it = std::find(dkey_stack.begin(), dkey_stack.end(), BOTTOM);
          break;
        case SDLK_LEFT:
          it = std::find(dkey_stack.begin(), dkey_stack.end(), LEFT);
          break;
        case SDLK_RIGHT:
          it = std::find(dkey_stack.begin(), dkey_stack.end(), RIGHT);
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
        case SDLK_UP:
          player1.setDirection(TOP);
          if (dkey_stack.size() != 0) {
            if (dkey_stack[dkey_stack.size() - 1] != TOP)
              dkey_stack.push_back(TOP);
          } else
            dkey_stack.push_back(TOP);
          break;
        case SDLK_DOWN:
          player1.setDirection(BOTTOM);
          if (dkey_stack.size() != 0) {
            if (dkey_stack[dkey_stack.size() - 1] != BOTTOM)
              dkey_stack.push_back(BOTTOM);
          } else
            dkey_stack.push_back(BOTTOM);
          break;
        case SDLK_LEFT:
          player1.setDirection(LEFT);
          if (dkey_stack.size() != 0) {
            if (dkey_stack[dkey_stack.size() - 1] != LEFT)
              dkey_stack.push_back(LEFT);
          } else
            dkey_stack.push_back(LEFT);
          break;
        case SDLK_RIGHT:
          player1.setDirection(RIGHT);
          if (dkey_stack.size() != 0) {
            if (dkey_stack[dkey_stack.size() - 1] != RIGHT)
              dkey_stack.push_back(RIGHT);
          } else
            dkey_stack.push_back(RIGHT);
          break;
        case SDLK_SPACE: {
          SDL_Texture *tex = win.loadTexture("res/textures/player0.png");
          Bullet b = player1.fireBullet(tex);
          bullets.push_back(b);
          break;
        }
        default:
          break;
      }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        SDL_Texture *tex = win.loadTexture("res/textures/player0.png");
        Bullet b = player1.fireBullet(tex);
        bullets.push_back(b);
      }
    }
  }
}

void Game::update() {
  player1.move(&maze);
  for (auto bullet_it = bullets.begin(); bullet_it != bullets.end();
       bullet_it++) {
    bullet_it->move(&maze);
    if (!bullet_it->isMoving()) {
      bullet_it = bullets.erase(bullet_it);
      bullet_it--;
      // std::cout << "deleting bullet" << std::endl;
    }
  }
}
void Game::render() {
  win.render(player1);
  win.render(player2);
  for (auto &bullet : bullets) {
    win.render(bullet);
  }
  int w = Params::ACTUAL_CELL_SIZE;
  auto maze_V = maze.getPixelV();
  for (int x = 0; x < maze_V.size(); x++) {
    for (int y = 0; y < maze_V[0].size(); y++) {
      if (maze_V[x][y] == 1) win.render({w * y, w * x, w, w}, maze.maze_tex);
    }
  }
}
void Game::wait() { SDL_Delay(10); }

void Game::quit() {
  running = false;
  win.destroy();
}

bool Game::isRunning() { return running; };

// 0 0 0 0 0 1
// 0 0 0 0 0
// 1 1 1