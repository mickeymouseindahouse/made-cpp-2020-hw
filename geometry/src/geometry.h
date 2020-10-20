#pragma once

#include <cmath>
#include <tuple>
#include <utility>
#include <vector>

#define PI 3.14159265

auto square = [](const double &x) -> double {
    return x * x;
};

auto radian = [](const double &angle) -> double {
    return PI * angle / 180.0;
};

class Line;

struct Point {
    double x;
    double y;

    Point(const double &x, const double &y) : x(x), y(y) {};

    bool operator==(const Point &other) {
        return this->x == other.x && this->y == other.y;
    }

    bool operator!=(const Point &other) {
        return !(*this == other);
    }

    double distance(const Point &other) {
        return sqrt(square(x - other.x) + square(y - other.y));
    }

    Point(const Point &copy) {
        x = copy.x;
        y = copy.y;
    }

    void rotate(const Point &pivot, const double &angle) {
        double shiftedX = x - pivot.x;
        double shiftedY = y - pivot.y;

        x = pivot.x + (shiftedX * cos(radian(angle)) - shiftedY * sin(radian(angle)));
        y = pivot.y + (shiftedX * sin(radian(angle)) + shiftedY * cos(radian(angle)));
    }

    void reflex(const Point &pivot) {
        x = 2.0 * pivot.x - x;
        y = 2.0 * pivot.y - y;
    }

    void reflex(const Line &line);

    void scale(const Point &pivot, const double &coefficient) {
        double shiftedX = x - pivot.x;
        double shiftedY = y - pivot.y;

        x = pivot.x + coefficient * shiftedX;
        y = pivot.y + coefficient * shiftedY;
    }

    void operator=(const Point &other) {
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
    Line(const double &k, const double &b) : k_(k), b_(b) {}

    Line(const Point &p1, const Point &p2) {
        k_ = (p2.y - p1.y) / (p2.x - p1.x);
        b_ = p1.y - k_ * p1.x;
    }

    Line(const Point &p, const double &k) {
        this->k_ = k;
        this->b_ = p.y - k * p.x;
    }

    double k() const {
        return k_;
    }

    double b() const {
        return b_;
    }

    bool operator==(const Line &other) {
        return this->k_ == other.k_ && this->b_ == other.b_;
    }

    bool operator!=(const Line &other) {
        return !(*this == other);
    }
};

void Point::reflex(const Line &line) {
    double xRefl =
            ((1 - square(line.k())) * x + 2.0 * line.k() * y - 2.0 * line.k() * line.b()) / (square(line.k()) + 1);
    double yRefl = ((square(line.k()) - 1) * y + 2.0 * line.k() * x + 2.0 * line.b()) / (square(line.k()) + 1);

    x = xRefl;
    y = yRefl;
}

class Shape {
public:
    virtual double perimeter() const = 0;

    virtual double area() const = 0;

    virtual bool operator==(const Shape &other) const = 0;

    virtual bool operator!=(const Shape& other) const = 0;

    virtual void rotate(const Point &pivot, const double &angle) = 0;

    virtual void reflex(const Point &pivot) = 0;

    virtual void reflex(const Line &line) = 0;

    virtual void scale(const Point &pivot, const double &coefficient) = 0;

    virtual ~Shape() = 0;
};

Shape::~Shape() = default;

class Polygon : virtual public Shape {
private:
    std::vector<Point> *vertices;
    size_t vNum;
public:
    explicit Polygon(const std::vector<Point> &vertices) {
        this->vertices = new std::vector<Point>(vertices);
        this->vNum = this->vertices->size();
    }

    size_t verticesCount() const {
        return vNum;
    }

    std::vector<Point> getVertices() const {
        return *vertices;
    }

    double perimeter() const override {
        double sum = 0.0;
        for (size_t i = 1; i < vNum; ++i) {
            sum = (*vertices)[i].distance((*vertices)[i - 1]);
        }
        sum += (*vertices)[0].distance((*vertices)[vNum - 1]);
        return sum;
    }

    double area() const  override {
        double area = 0.0;
        for (size_t i = 1; i < vNum; ++i) {
            area += ((*vertices)[i].x + (*vertices)[i - 1].x) * ((*vertices)[i].y - (*vertices)[i - 1].y);
        }
        area += ((*vertices)[0].x + (*vertices)[vNum - 1].x) * ((*vertices)[0].y - (*vertices)[vNum - 1].y);
        return fabs(area) / 2.0;
    }

    void rotate(const Point &pivot, const double &angle) override {
        for (auto vertex : *vertices) {
            vertex.rotate(pivot, angle);
        }
    }

    void reflex(const Point &pivot) override {
        for(auto vertex : *vertices) {
            vertex.reflex(pivot);
        }
    }

    void reflex(const Line& line) override {
        for(auto vertex : *vertices) {
            vertex.reflex(line);
        }
    }

    void scale(const Point& pivot, const double& coefficient) override {
        for(auto vertex : *vertices) {
            vertex.scale(pivot, coefficient);
        }
    }

    bool operator==(const Shape &other) const override {
        auto p = dynamic_cast<const Polygon*>(& other);
        return this->vNum == p->vNum && this->area() == p->area() && this->perimeter() == p->perimeter();
    }

    bool operator!=(const Shape& other) const override {
        return !(*this == other);
    }

    ~Polygon() override {
        delete vertices;
    }
};

class Ellipse : virtual public Shape {
private:
    Point *focus1;
    Point *focus2;
    double a;
    double b;
    double c;

public:
    Ellipse(const Point &p1, const Point &p2, const double &distSum) {
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

    double perimeter() const override {
        return 4 * a * std::comp_ellint_2(sqrt(square(a) - square(b)) / a);
    }

    double area() const override {
        return PI * a * b;
    }

    void rotate(const Point &pivot, const double &angle) override {
        focus1->rotate(pivot, angle);
        focus2->rotate(pivot, angle);
    }

    void reflex(const Point &pivot) override {
        focus1->reflex(pivot);
        focus2->reflex(pivot);
    }

    void reflex(const Line &line) override {
        focus1->reflex(line);
        focus2->reflex(line);
    }

    void scale(const Point &pivot, const double &coefficient) override {
        focus1->scale(pivot, coefficient);
        focus2->scale(pivot, coefficient);
        a *= coefficient;
        b *= coefficient;
        c *= coefficient;
    }

    bool operator==(const Shape &other) const override {
        auto e = dynamic_cast<const Ellipse *>(&other);
        return this->a == e->a && this->b == e->b && this->c == e->c;
    }

    bool operator!=(const Shape& other) const override {
        return !(*this == other);
    }

    ~Ellipse() override {
        delete focus1;
        delete focus2;
    }
};

class Circle : public Ellipse {
private:
    Point *center_;
    double radius_;

public:
    Circle(const Point &p, double radius) : Ellipse(p, p, radius * 2.0) {
        center_ = new Point(p);
        radius_ = radius;
    }

    double radius() {
        return radius_;
    }

    double perimeter() const override {
        return 2 * PI * radius_;
    }

    double area() const override {
        return PI * square(radius_);
    }

    void rotate(const Point &pivot, const double &angle) override {
        center_->rotate(pivot, angle);
    }

    void reflex(const Point &pivot) override {
        center_->reflex(pivot);
    }

    void reflex(const Line &line) override {
        center_->reflex(line);
    }

    void scale(const Point &pivot, const double &coefficient) override {
        center_->scale(pivot, coefficient);
        radius_ *= coefficient;
    }

    bool operator==(const Shape &other) const override {
        auto c = dynamic_cast<const Circle *>(&other);
        return center_ == c->center_ && radius_ == c->radius_;
    }

    bool operator!=(const Shape& other) const override {
        return !(*this == other);
    }

    ~Circle() override {
        delete center_;
    }
};