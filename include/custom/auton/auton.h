#ifndef Auton_H
#define Auton_H

#include "custom/util.h"
#include "math.h"
#include "vex.h"
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

// ---
// This code aims to make writing reliable autons much quicker and easier by
// turning the field into a grid of co-ordinates and pathfinding from the bot's
// current position to a desired target, the movement could be made much more
// accurate by using a V5 inertial sensor This code is currently limited by only
// being able to track it's position when being moved by this code, it could be
// expanded upon by allowing diagonal movement and the ability to move the bot
// back onto the grid when it is moved directly
// ---

using namespace vex;

class Edge {
public:
  Vector2 _tile1 = Vector2(-1, -1);
  Vector2 _tile2 = Vector2(-1, -1);
  Edge(Vector2 tile1, Vector2 tile2) { _tile1 = tile1; }
};

enum orientation { north = 0, east = 90, south = 180, west = 270, noO = 0 };

bool IsEqual(Vector2 a, Vector2 b) {
  if (a.x == b.x && a.y == b.y)
    return true;
  return false;
}

Edge edges[8] = {
    Edge(Vector2(2, 0), Vector2(3, 0)), Edge(Vector2(0, 3), Vector2(0, 2)),
    Edge(Vector2(2, 5), Vector2(3, 5)), Edge(Vector2(5, 3), Vector2(5, 2)),
    Edge(Vector2(2, 2), Vector2(3, 2)), Edge(Vector2(2, 2), Vector2(2, 3)),
    Edge(Vector2(3, 3), Vector2(2, 3)), Edge(Vector2(3, 3), Vector2(3, 2))};

std::vector<Vector2> GetEdges(Vector2 tile) {
  std::vector<Vector2> allEdges;

  for (int i = 0; i < 8; i++) {
    if (IsEqual(edges[i]._tile1, tile)) {
      allEdges.push_back(edges[i]._tile2);
    } else if (IsEqual(edges[i]._tile2, tile)) {
      allEdges.push_back(edges[i]._tile1);
    }
  }
  return allEdges;
}

int width = 6;
int height = 6;

Vector2 currentPos = Vector2(0, 0);
orientation currentDir = north;

std::vector<Vector2> GetAdjacent(Vector2 pos) {
  std::vector<Vector2> adjacent;

  for (int x = pos.x - 1; x <= pos.x + 1; x++) {
    if (x >= 0 && x < width && x != pos.x) {
      adjacent.push_back(Vector2(x, pos.y));
    }
  }
  for (int y = pos.y - 1; y <= pos.y + 1; y++) {
    if (y >= 0 && y < height && y != pos.y) {
      adjacent.push_back(Vector2(pos.x, y));
    }
  }
  return adjacent;
}

float GetDistance(Vector2 a, Vector2 b) {
  float px = powf(b.x - a.x, 2);
  float py = powf(b.y - a.y, 2);
  float dist = sqrtf(px + py);
  return dist;
}

std::vector<Vector2> GeneratePath(Vector2 pos, Vector2 target) {
  std::vector<Vector2> path;

  std::queue<Vector2> curQueue;
  curQueue.push(pos);
  int t = 0;
  while (!curQueue.empty() && t < 50) {
    Vector2 node = curQueue.front();
    curQueue.pop();

    Vector2 bestPos = Vector2(0, 0);
    float bestScore = 10000;
    std::vector<Vector2> adjacent = GetAdjacent(node);
    for (auto n : adjacent) {
      float score = GetDistance(n, target);
      if (score < bestScore) {
        std::vector<Vector2> allEdges = GetEdges(node);
        bool flag = false;
        if (allEdges.size() > 0) {
          for (auto j : adjacent) {
            if (IsEqual(j, n)) {
              flag = true;
            }
          }
        }
        if (flag == true) {
          continue;
        }
        bestScore = score;
        bestPos = n;
      }
    }
    curQueue.push(bestPos);
    path.push_back(bestPos);

    if (bestScore == 0 || bestScore == 10000)
      return path;
    t++;
  }
  std::vector<Vector2> failSafe;
  return failSafe;
}

orientation GetDir(Vector2 a, Vector2 b) {
  if (b.x > a.x)
    return east;
  else if (a.x > b.x)
    return west;
  else if (b.y > a.y)
    return north;
  else if (a.y > b.y)
    return south;
  else
    return noO;
}

double GetAngle(orientation a, orientation b) {
  double ang = b - a;
  return ang;
}

void TurnToTile(Vector2 target) {
  orientation targetDir = GetDir(currentPos, target);
  if (targetDir != noO) {
    double ang = GetAngle(currentDir, targetDir) / 2;
    if (ang == 0 || ang == 360)
      return;
    if(ang * 2 == 270) ang = -90 / 2;
    std::cout << "Turning for " << ang * 2 << " degrees" << std::endl;
    Drivetrain.turnFor(vex::right, ang, degrees);
    wait(500, msec);
  }
}

void MoveTo(Vector2 target) {

  std::vector<Vector2> path = GeneratePath(currentPos, target);

  for (auto p : path) {
    Vector2 nextTile = p;
    TurnToTile(nextTile);
    Drivetrain.driveFor(600, mm, 100, rpm);
    wait(500, msec);
  }
}

void Initialise(util::side selectedSide) {
  std::cout << "Auton Initialised" << std::endl;
  int x = 0;
  int y = 0;
  currentDir = north;
  if (selectedSide == util::left) {
    x = 1;
  } else {
    x = 4;
  }
  currentPos = Vector2(x, y);
}

// Grid is taken from your teams bottom left
#endif
