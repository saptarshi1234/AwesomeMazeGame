#include "maze.hpp"

#include <random>

#include "params.hpp"

#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

Maze::Maze() {}

Maze::~Maze() {}

void Maze::setSize(int w, int h) {
  mazeHeight = h;
  mazeWidth = w;
}

void Maze::setPathLength(int l) { pathLength = l; }

void Maze::setWallLength(int l) { wallLength = l; }

void Maze::generate() {
  maze = new cell*[mazeWidth];
  for (int i = 0; i < mazeWidth; i++) {
    maze[i] = new cell[mazeWidth];
  }
  for (int i = 0; i < mazeWidth; i++) {
    for (int j = 0; j < mazeHeight; j++) {
      maze[i][j].X = i;
      maze[i][j].Y = j;
    }
  }
  int numVisited = 0;
  cell curCell = maze[0][0];
  maze[0][0].visited = true;
  numVisited++;
  while (numVisited < mazeHeight * mazeWidth) {
    std::vector<int> possibleDirs;
    if (curCell.Y > 0 && maze[curCell.X][curCell.Y - 1].visited == false)
      possibleDirs.push_back(1);

    if (curCell.X > 0 && maze[curCell.X - 1][curCell.Y].visited == false)
      possibleDirs.push_back(2);

    if (curCell.Y < mazeHeight - 1 &&
        maze[curCell.X][curCell.Y + 1].visited == false)
      possibleDirs.push_back(3);

    if (curCell.X < mazeWidth - 1 &&
        maze[curCell.X + 1][curCell.Y].visited == false)
      possibleDirs.push_back(4);

    if (possibleDirs.size() != 0) {
      numVisited++;
      int randDir = possibleDirs[rand() % possibleDirs.size()];
      switch (randDir) {
        case UP:
          maze[curCell.X][curCell.Y - 1].visited = true;
          maze[curCell.X][curCell.Y - 1].down = true;
          maze[curCell.X][curCell.Y - 1].prvsX = curCell.X;
          maze[curCell.X][curCell.Y - 1].prvsY = curCell.Y;
          curCell = maze[curCell.X][curCell.Y - 1];
          break;
        case LEFT:
          maze[curCell.X - 1][curCell.Y].visited = true;
          maze[curCell.X - 1][curCell.Y].right = true;
          maze[curCell.X - 1][curCell.Y].prvsX = curCell.X;
          maze[curCell.X - 1][curCell.Y].prvsY = curCell.Y;
          curCell = maze[curCell.X - 1][curCell.Y];
          break;
        case DOWN:
          maze[curCell.X][curCell.Y + 1].visited = true;
          maze[curCell.X][curCell.Y].down = true;
          maze[curCell.X][curCell.Y + 1].prvsX = curCell.X;
          maze[curCell.X][curCell.Y + 1].prvsY = curCell.Y;
          curCell = maze[curCell.X][curCell.Y + 1];
          break;
        case RIGHT:
          maze[curCell.X + 1][curCell.Y].visited = true;
          maze[curCell.X][curCell.Y].right = true;
          maze[curCell.X + 1][curCell.Y].prvsX = curCell.X;
          maze[curCell.X + 1][curCell.Y].prvsY = curCell.Y;
          curCell = maze[curCell.X + 1][curCell.Y];
          break;
      }
    } else
      curCell = maze[curCell.prvsX][curCell.prvsY];
  }
}

std::vector<std::vector<int>> Maze::render() {
  pixelV.resize(
      (mazeWidth) * (pathLength + wallLength) - wallLength,
      std::vector<int>((mazeHeight) * (pathLength + wallLength) - wallLength));

  for (int i = 0; i < (mazeWidth) * (pathLength + wallLength) - wallLength;
       i++) {
    for (int j = 0; j < (mazeHeight) * (pathLength + wallLength) - wallLength;
         j++) {
      pixelV[i][j] = 1;
    }
  }

  for (int i = 0; i < mazeWidth; i++) {
    for (int j = 0; j < mazeHeight; j++) {
      int startX = (pathLength + wallLength) * i;
      int startY = (pathLength + wallLength) * j;
      int w = pathLength;
      int h = pathLength;
      if (maze[i][j].down) h += wallLength;
      if (maze[i][j].right) w += wallLength;
      for (int x = startX; x < startX + w; x++) {
        for (int y = startY; y < startY + h; y++) {
          pixelV[x][y] = 0;
        }
      }
    }
  }
  return pixelV;
}

std::vector<std::vector<int>> Maze::getPixelV() { return pixelV; }
