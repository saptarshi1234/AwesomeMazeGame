#include "maze.hpp"

#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

#include "params.hpp"

#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
using namespace std;

struct queueNode {
  int dist = INT32_MAX;
  int x, y;
};

bool compNode(queueNode n1, queueNode n2) { return n1.dist > n2.dist; }
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
  int arrW = (mazeWidth) * (pathLength + wallLength) - wallLength;
  int arrH = (mazeHeight) * (pathLength + wallLength) - wallLength;
  int nodeW = mazeWidth * 2 - 1;
  int nodeH = mazeHeight * 2 - 1;
  pixelV.resize(arrW, std::vector<int>(arrH));

  for (int i = 0; i < arrW; i++) {
    for (int j = 0; j < arrH; j++) {
      pixelV[i][j] = 1;
    }
    std::vector<int> temp(nodeH, 1);
    if (i < nodeW) nodes.push_back(temp);
  }

  for (int i = 0; i < mazeWidth; i++) {
    for (int j = 0; j < mazeHeight; j++) {
      int startX = (pathLength + wallLength) * i;
      int startY = (pathLength + wallLength) * j;
      int w = pathLength;
      int h = pathLength;
      if (maze[i][j].down) h += wallLength;
      if (maze[i][j].right) w += wallLength;
      int temp = rand() % 100;
      int temp2 = rand() % 100;
      if (i != mazeWidth - 1 && temp < 15) w = pathLength + wallLength;

      if (j != mazeHeight - 1 && temp2 < 15) h = pathLength + wallLength;

      for (int x = startX; x < startX + w; x++) {
        for (int y = startY; y < startY + h; y++) {
          if (x < startX + pathLength || y < startY + pathLength)
            pixelV[x][y] = 0;
        }
      }

      nodes[2 * i][2 * j] = 0;
      if (w > pathLength) nodes[2 * i + 1][2 * j] = 0;
      if (h > pathLength) nodes[2 * i][2 * j + 1] = 0;
    }
  }
  for (int i = 0; i < mazeWidth - 1; i++) {
    for (int j = 0; j < mazeHeight - 1; j++) {
      int wallX = pathLength + (pathLength + wallLength) * i;
      int wallY = pathLength + (pathLength + wallLength) * j;
      if (pixelV[wallX - 1][wallY] + pixelV[wallX][wallY - 1] +
              pixelV[wallX + wallLength][wallY] +
              pixelV[wallX][wallY + wallLength] ==
          0)
        for (int x = wallX; x < wallX + wallLength; x++) {
          for (int y = wallY; y < wallY + wallLength; y++) {
            pixelV[x][y] = 0;
          }
        }
    }
  }
  for (int i = 0; i < nodeW; i++) {
    for (int j = 0; j < nodeH; j++) {
      if (nodes[i][j] == 1)
        cout << "* ";
      else
        cout << "  ";
    }
    cout << endl;
  }
  return pixelV;
}

void Maze::addPadding() {
  for (auto& row : pixelV) {
    row.push_back(1);
    row.insert(row.begin(), 1);
  }
  pixelV.push_back(std::vector<int>(pixelV[0].size(), 1));
  pixelV.insert(pixelV.begin(), std::vector<int>(pixelV[0].size(), 1));
}

std::vector<std::vector<int>> Maze::getPixelV() { return pixelV; }

void Maze::setPixelV(std::vector<std::vector<int>> v) { pixelV = v; }

void Maze::calcDistances() {
  distances.resize(nodes.size());
  int nodeW = nodes.size();
  int nodeH = nodes[0].size();
  for (int i = 0; i < nodeW; i++) {
    for (int j = 0; j < nodeH; j++) {
      vector<vector<int>> tempD(nodeW, vector<int>(nodeH, -1));
      vector<vector<queueNode>> tempNodes(nodeW, vector<queueNode>(nodeH));
      if (nodes[i][j] == 1) {
        distances[i].push_back(tempD);
        continue;
      }
      priority_queue<queueNode, vector<queueNode>,
                     std::function<bool(queueNode, queueNode)>>
          pq(compNode);
      for (int k = 0; k < nodeW; k++) {
        for (int p = 0; p < nodeH; p++) {
          if (nodes[k][p] == 1) {
            continue;
          }
          if (k == i && p == j) {
            tempNodes[k][p] = {0, k, p};
            continue;
          }
          tempNodes[k][p] = {INT32_MAX, k, p};
        }
      }
      pq.push(tempNodes[i][j]);
      int currX = i, currY = j;
      while (!pq.empty()) {
        queueNode n = pq.top();
        pq.pop();
        currX = n.x;
        currY = n.y;
        if (currX - 1 >= 0 && nodes[currX - 1][currY] == 0) {
          if (tempNodes[currX][currY].dist + 1 <
              tempNodes[currX - 1][currY].dist) {
            tempNodes[currX - 1][currY].dist = tempNodes[currX][currY].dist + 1;
            pq.push(tempNodes[currX - 1][currY]);
          }
        }

        if (currY - 1 >= 0 && nodes[currX][currY - 1] == 0) {
          if (tempNodes[currX][currY].dist + 1 <
              tempNodes[currX][currY - 1].dist) {
            tempNodes[currX][currY - 1].dist = tempNodes[currX][currY].dist + 1;
            pq.push(tempNodes[currX][currY - 1]);
          }
        }

        if (currX + 1 < nodeW && nodes[currX + 1][currY] == 0) {
          if (tempNodes[currX][currY].dist + 1 <
              tempNodes[currX + 1][currY].dist) {
            tempNodes[currX + 1][currY].dist = tempNodes[currX][currY].dist + 1;
            pq.push(tempNodes[currX + 1][currY]);
          }
        }

        if (currY + 1 < nodeH && nodes[currX][currY + 1] == 0) {
          if (tempNodes[currX][currY].dist + 1 <
              tempNodes[currX][currY + 1].dist) {
            tempNodes[currX][currY + 1].dist = tempNodes[currX][currY].dist + 1;
            pq.push(tempNodes[currX][currY + 1]);
          }
        }
      }
      for (int k = 0; k < nodeW; k++) {
        for (int p = 0; p < nodeH; p++) {
          if (nodes[k][p] == 1) {
            continue;
          }
          tempD[k][p] = tempNodes[k][p].dist;
        }
      }
      distances[i].push_back(tempD);
    }
  }

  for (int i = 0; i < nodeW; i++) {
    for (int j = 0; j < nodeH; j++) {
      cout << distances[2][0][i][j] << " ";
    }
    cout << endl;
  }
}
int Maze::dist(int x1, int y1, int x2, int y2) {
  int xV1 = y1 / Params::ACTUAL_CELL_SIZE - 1;
  int yV1 = x1 / Params::ACTUAL_CELL_SIZE - 1;
  int xV2 = y2 / Params::ACTUAL_CELL_SIZE - 1;
  int yV2 = x2 / Params::ACTUAL_CELL_SIZE - 1;
  xV1 /= 2;
  xV2 /= 2;
  yV1 /= 2;
  yV2 /= 2;
  if (xV1 < 0 || xV1 >= nodes.size() || xV2 < 0 || xV2 >= nodes.size() ||
      yV1 < 0 || yV1 >= nodes[0].size() || yV2 < 0 || yV2 >= nodes[0].size()) {
    return -1;
  }
  return distances[xV1][yV1][xV2][yV2];
}

// assumes input to be valid
int Maze::dirFromTo(int x1, int y1, int x2, int y2) {
  int xV1 = y1 / Params::ACTUAL_CELL_SIZE - 1;
  int yV1 = x1 / Params::ACTUAL_CELL_SIZE - 1;
  int xV2 = y2 / Params::ACTUAL_CELL_SIZE - 1;
  int yV2 = x2 / Params::ACTUAL_CELL_SIZE - 1;
  xV1 /= 2;
  xV2 /= 2;
  yV1 /= 2;
  yV2 /= 2;
  if (xV1 < 0 || xV1 >= nodes.size() || xV2 < 0 || xV2 >= nodes.size() ||
      yV1 < 0 || yV1 >= nodes[0].size() || yV2 < 0 || yV2 >= nodes[0].size()) {
    return -1;
  }
  int d = distances[xV1][yV1][xV2][yV2];
  vector<int> v;
  if (xV1 > 0 && distances[xV1 - 1][yV1][xV2][yV2] != -1 &&
      distances[xV1 - 1][yV1][xV2][yV2] < d)
    v.push_back(0);
  if (xV1 < nodes.size() - 1 && distances[xV1 + 1][yV1][xV2][yV2] != -1 &&
      distances[xV1 + 1][yV1][xV2][yV2] < d)
    v.push_back(2);
  if (yV1 > 0 && distances[xV1][yV1 - 1][xV2][yV2] != -1 &&
      distances[xV1][yV1 - 1][xV2][yV2] < d)
    v.push_back(1);
  if (yV1 < nodes[0].size() - 1 && distances[xV1][yV1 + 1][xV2][yV2] != -1 &&
      distances[xV1][yV1 + 1][xV2][yV2] < d)
    v.push_back(3);

  if (v.size() == 0) return -1;
  int in = rand() % v.size();
  return v[in];
}