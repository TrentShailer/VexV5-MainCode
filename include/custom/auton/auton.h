#ifndef Auton_H
#define Auton_H

#include "custom/util.h"
#include "math.h"
#include "vex.h"
#include <iostream>
#include <vector>

// ---
// This code aims to make writing reliable autons much quicker and easier by
// turning the field into a grid of co-ordinates and pathfinding from the bot's
// current position to a desired target, the movement could be made much more accurate
// by using a V5 inertial sensor
// This code is currently limited by only being able to track it's position when being moved
// by this code, it could be expanded upon by allowing diagonal movement and the ability to
// move the bot back onto the grid when it is moved directly
// ---

using namespace vex;

class auton {

  class Edge {
  public:
    Vector2 _tile1 = Vector2(-1, -1);
    Vector2 _tile2 = Vector2(-1, -1);
    Edge(Vector2 tile1, Vector2 tile2) { _tile1 = tile1; }
  };

  enum orientation { north = 0, east = 90, south = 180, west = 270, none = 0 };

  bool IsEqual(Vector2 a, Vector2 b) {
    if (a.x == b.x && a.y == b.y)
      return true;
    return false;
  }
  std::vector<Edge> edges = {
      Edge(Vector2(2, 0), Vector2(3, 0)), Edge(Vector2(0, 3), Vector2(0, 2)),
      Edge(Vector2(2, 5), Vector2(3, 5)), Edge(Vector2(5, 3), Vector2(5, 2)),
      Edge(Vector2(2, 2), Vector2(3, 2)), Edge(Vector2(2, 2), Vector2(2, 3)),
      Edge(Vector2(3, 3), Vector2(2, 3)), Edge(Vector2(3, 3), Vector2(3, 2))};

  std::vector<Vector2> GetEdges(Vector2 tile) {
    std::vector<Vector2> allEdges;
    std::vector<Vector2>::iterator it;
    it = allEdges.begin();

    for (int i = 0; i < edges.size(); i++) {
      if (IsEqual(edges[i]._tile1, tile)) {
        it = allEdges.insert(it, edges[i]._tile2);
      } else if (IsEqual(edges[i]._tile2, tile)) {
        it = allEdges.insert(it, edges[i]._tile1);
      }
    }
    return allEdges;
  }
public:
  int width = 6;
  int height = 6;

  Vector2 currentPos = Vector2(0, 0);
  orientation currentDir = north;
private:
  std::vector<Vector2> GetAdjacent(Vector2 pos) {
    std::vector<Vector2> adjacent;
    std::vector<Vector2>::iterator it;
    it = adjacent.begin();

    for (int x = pos.x - 1; x <= pos.x + 1; x++) {
      if (x >= 0 && x < width && x != pos.x) {
        adjacent.insert(it, Vector2(x, pos.y));
      }
    }
    for (int y = pos.y - 1; y <= pos.y + 1; y++) {
      if (y >= 0 && y < height && y != pos.y) {
        adjacent.insert(it, Vector2(pos.x, y));
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
    std::vector<Vector2>::iterator pIt;
    pIt = path.begin();

    std::vector<Vector2> queue = {pos};
    std::vector<Vector2>::iterator qIt;
    std::vector<Vector2>::iterator qIt2;
    qIt2 = queue.begin();

    int t = 0;

    while (queue.size() > 0 && t < 50) {
      qIt = prev(queue.end());
      Vector2 node = queue[queue.size() - 1];
      queue.erase(qIt);

      Vector2 bestPos = Vector2(0, 0);
      float bestScore = 10000;
      std::vector<Vector2> adjacent = GetAdjacent(node);
      for (int i = 0; i < adjacent.size(); i++) {
        float score = GetDistance(adjacent[i], target);
        if (score < bestScore) {
          std::vector<Vector2> allEdges = GetEdges(node);
          bool flag = false;

          if (allEdges.size() > 0) {
            for (int j = 0; j < allEdges.size(); j++) {
              if (IsEqual(allEdges[j], adjacent[i])) {
                flag = true;
              }
            }
          }
          if (flag == true) {
            continue;
          }
          bestScore = score;
          bestPos = adjacent[i];
        }
      }
      queue.insert(qIt2, bestPos);
      path.insert(pIt, bestPos);

      if (bestScore == 0 || bestScore == 10000)
        return path;
      t++;
    }
    return std::vector<Vector2>{};
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
      return none;
  }

  double GetAngle(orientation a, orientation b) {
    double ang = b - a;
    return ang;
  }
public:
  void TurnToTile(Vector2 target) {
    orientation targetDir = GetDir(currentPos, target);
    if (targetDir != none) {
      double ang = GetAngle(currentDir, targetDir);
      if (ang == 0 || ang == 360)
        return;
      Drivetrain.turnFor(vex::right, ang, deg);
      wait(500, msec);
    }
  }

  void MoveTo(Vector2 target) {
    std::vector<Vector2> path = GeneratePath(currentPos, target);
    for (int i = 0; i < path.size(); i++) {
      Vector2 nextTile = path[i];
      TurnToTile(nextTile);
      Drivetrain.driveFor(600, mm, 50, rpm);
      wait(500, msec);
    }
  }

  void Initialise(util::side selectedSide) {
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
};
// Grid is taken from your teams bottom left
#endif
