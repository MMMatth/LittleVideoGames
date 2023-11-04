#ifndef POINT_HPP
#define POINT_HPP

#include <string>

class Point {
public:
  /* constructor */
  Point() : m_x(0), m_y(0) {}
  Point(int x, int y) : m_x(x), m_y(y) {}
  /* destructor */
  ~Point() {}
  /* getters */
  int getX() const { return m_x; }
  int getY() const { return m_y; }
  /* setters */
  void setX(int x) { m_x = x; }
  void setY(int y) { m_y = y; }
  /* other */
  bool isEqual(Point other) {
    return (m_x == other.getX() && m_y == other.getY());
  }
  std::string toString() {
    return "(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ")";
  };

private:
  int m_x;
  int m_y;
};

#endif