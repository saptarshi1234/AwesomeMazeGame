#include "game.hpp"

#include <iostream>

#include "window.hpp"

Game::Game(WindowManager win) : win(win) {
  e1.init({0, 0, 20, 20}, win.loadTexture("res/textures/player0.png"));
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
            e1.move(TOP);
            break;
          case SDLK_DOWN:
            e1.move(BOTTOM);
            break;
          case SDLK_LEFT:
            e1.move(LEFT);
            break;
          case SDLK_RIGHT:
            e1.move(RIGHT);
            break;
          default:
            break;
        }
    }
  }
}

void Game::update() {}
void Game::render() { win.render(e1); }
void Game::wait() { SDL_Delay(10); }

void Game::quit() {
  running = false;
  win.destroy();
}

bool Game::isRunning() { return running; };
