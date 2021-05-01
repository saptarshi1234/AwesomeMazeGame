#include "game.hpp"

#include <algorithm>
#include <iostream>

#include "bullet.hpp"
#include "entity.hpp"
#include "maze.hpp"
#include "window.hpp"

Game::Game(WindowManager win) : win(win) {
  player1.init({0, 0, 20, 20}, win.loadTexture("res/textures/player0.png"));
}

void Game::initialize() {
  Maze m;
  m.setSize(15, 10);
  m.setPathLength(2);
  m.setWallLength(2);
  m.generate();
  std::vector<std::vector<int>> v = m.render();
  std::cout << v.size() << "\n";
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

void Game::loadGame() { running = true; }

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
      if (it != dkey_stack.end()) dkey_stack.erase(it);
      s = dkey_stack.size();
      if (dkey_stack.size() == 0)
        player1.stopMoving();
      else
        player1.setDirection(dkey_stack[dkey_stack.size() - 1]);
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
  player1.move();

  for (auto bullet_it = bullets.begin(); bullet_it != bullets.end();
       bullet_it++) {
    bullet_it->move();
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