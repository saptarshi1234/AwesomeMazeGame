#include "game.hpp"

#include <algorithm>
#include <iostream>

#include "bullet.hpp"
#include "entity.hpp"
#include "maze.hpp"
#include "params.hpp"
#include "window.hpp"

Game::Game(WindowManager win) : win(win) {}

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

void Game::initialize() {
  maze.setSize(Params::NUM_CELLS_X, Params::NUM_CELLS_Y);
  maze.setPathLength(Params::PATH_WIDTH);
  maze.setWallLength(Params::WALL_WIDTH);
  maze.generate();
  maze.maze_tex = win.loadTexture("res/textures/ground.png");

  maze.render();
  maze.addPadding();

  printMaze(maze.getPixelV());
}

void Game::loadGame() {
  int w = Params::ACTUAL_CELL_SIZE;
  player1.init({w, w, 2 * w, 2 * w},
               win.loadTexture("res/textures/player0.png"));
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
      int s;
      s = dkey_stack.size();
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
          s = dkey_stack.size();
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