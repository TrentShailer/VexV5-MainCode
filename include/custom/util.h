#ifndef util
#define util

// ---
// This is a collection of utilities I've used throughout this project
// --

enum side {
  left,
  right,
  none
};

// A custom datatype to hold an x and y co-ordinate
class Vector2 {
public:
  Vector2(float x, float y);
  float x;
  float y;
  bool within(float v);
};
Vector2::Vector2(float _x, float _y) {
  x = _x;
  y = _y;
}

bool floatwithin(float v1, float v2) {
  if (v1 < -v2) {
    return false;
  }
  if (v1 > v2) {
    return false;
  }
  return true;
}
bool Vector2::within(float v) {
  if (!floatwithin(x, v)) {
    return false;
  }
  if (!floatwithin(y, v)) {
    return false;
  }
  return true;
}

#endif