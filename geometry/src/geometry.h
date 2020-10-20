#pragma once

#include <cmath>
#include <tuple>
#include <utility>

#define PI 3.14159265359

auto square = [] (const double& x) -> double {
  return x * x;
};

auto radian = [] (const double& angle) -> double {
  return PI * angle / 180.0;
};

class Line;

struct Point {
  double x;
  double y;

  Point(const double& x, const double& y) : x(x), y(y) {};

  bool operator==(const Point &other) {
    return this->x == other.x && this->y == other.y;
  }

  bool operator!=(const Point &other) {
    return !(*this == other);
  }

  double distance(const Point& other) {
    return sqrt(square(x - other.x) + square(y - other.y));
  }

  Point(const Point& copy) {
    x = copy.x;
    y = copy.y;
  }

  void rotate(const Point& pivot, const double& angle) {
    double shiftedX = x - pivot.x;
    double shiftedY = y - pivot.y;

    x = pivot.x + (shiftedX * cos(radian(angle)) - shiftedY * sin(radian(angle)));
    y = pivot.y + (shiftedX * sin(radian(angle)) + shiftedY * cos(radian(angle)));
  }

  void reflex(const Point& pivot) {
    x = 2.0 * pivot.x - x;
    y = 2.0 * pivot.y - y;
  }

  void reflex(const Line& line);

  void operator=(const Point& other) {
    x = other.x;
    y = other.y;
  }
};

/**
 * y = kx + b
 */
class Line {
private:
  double k_;
  double b_;

public:
  Line(const double& k, const double & b) : k_(k), b_(b) {}

  Line(const Point& p1, const Point& p2) {
    k_ = (p2.y - p1.y) / (p2.x - p1.x);
    b_ = p1.y - k_ * p1.x;
  }

  Line(const Point& p, const double& k) {
    this->k_ = k;
    this->b_ = p.y - k * p.x;
  }

  double k() const {
    return k_;
  }

  double b() const {
    return b_;
  }

  bool operator==(const Line& other) {
    return this->k_ == other.k_ && this->b_ == other.b_;
  }

  bool operator!=(const Line& other) {
    return !(*this == other);
  }
};

void Point::reflex(const Line &line) {
  double xRefl = ((1 - square(line.k())) * x + 2.0 * line.k() * y - 2.0 * line.k() * line.b()) / (square(line.k()) + 1);
  double yRefl = ((square(line.k()) - 1) * y + 2.0 * line.k() * x + 2.0 * line.b()) / (square(line.k()) + 1);

  x = xRefl;
  y = yRefl;
}

class Shape {
public:
  virtual double perimeter() = 0;
  virtual double area() = 0;
  virtual bool operator==(Shape& other) const = 0;
  virtual void rotate(const Point& pivot, const double& angle) = 0;
  virtual void reflex(const Point& pivot) = 0;
  virtual void reflex(const Line& line) = 0;
  virtual void scale(const Point& pivot, const double& coefficient) = 0;
  virtual ~Shape() = 0;
};

Shape::~Shape() = default;

class Ellipse : virtual public Shape {
private:
  Point* focus1;
  Point* focus2;
  double a;
  double b;
  double c;

public:
  Ellipse(const Point& p1, const Point& p2, const double& distSum) {
    focus1 = new Point(p1);
    focus2 = new Point(p2);
    a = distSum / 2.0;
    c = focus1->distance(*focus2) / 2.0;
    b = sqrt(square(a) - square(c));
  }

  std::pair<Point, Point> focuses() {
    return std::make_pair(*focus1, *focus2);
  }

  Point center() {
    return Point((focus1->x + focus2->x) / 2.0, (focus1->y + focus2->y) / 2.0);
  }

  double eccentricity() {
    return c / a;
  }

  double perimeter() {
    return 4 * (PI * a * b + square(a - b)) / (a + b);
  }

  double area() {
    return PI * a * b;
  }

  void rotate(const Point& pivot, const double& angle) {
    focus1->rotate(pivot, angle);
    focus2->rotate(pivot, angle);
  }

  void reflex(const Point& pivot) {
    focus1->reflex(pivot);
    focus2->reflex(pivot);
  }

  void reflex(const Line& line) {
    focus1->reflex(line);
    focus2->reflex(line);
  }

  bool operator==(Shape& other) const {
    const Ellipse* e = dynamic_cast<const Ellipse*>(& other);
    return this->a == e->a && this->b == e->b && this->c == e->c;
  }

  ~Ellipse() {
    delete focus1;
    delete focus2;
  }
};

class Circle : public Ellipse {
private:
  Point* center;
  double radius_;

public:
  Circle(const Point& p, double radius) : Ellipse(p, p, radius * 2.0) {
    center = new Point(p);
    radius_ = radius;
  }

  double radius() {
    return radius_;
  }

  double perimeter() {
    return 2 * PI * radius_;
  }

  double area() {
    return PI * square(radius_);
  }

  void rotate(const Point& pivot, const double& angle) {
    center->rotate(pivot, angle);
  }

  void reflex(const Point& pivot) {
    center->reflex(pivot);
  }

  void reflex(const Line& line) {
    center->reflex(line);
  }

  bool operator==(Shape& other) const {
    const Circle* c = dynamic_cast<const Circle*>(&other);
    return center == c->center && radius_ == c->radius_;
  }



};