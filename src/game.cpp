#include "game.hpp"

#include <iostream>

#include "bullet.hpp"
#include "entity.hpp"
#include "window.hpp"

Game::Game(WindowManager win) : win(win) {
  player1.init({0, 0, 20, 20}, win.loadTexture("res/textures/player0.png"));
}

void Game::initialize() {}

void Game::loadGame() { running = true; }

void Game::handleEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        quit();
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_UP:
            player1.setDirection(TOP);
            break;
          case SDLK_DOWN:
            player1.setDirection(BOTTOM);
            break;
          case SDLK_LEFT:
            player1.setDirection(LEFT);
            break;
          case SDLK_RIGHT:
            player1.setDirection(RIGHT);
            break;
          default:
            break;
        }
      case SDL_MOUSEBUTTONDOWN:
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
    if (bullet_it->getDirection() == STOP) {
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
