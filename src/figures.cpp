#include "figures.hpp"

#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <sstream>

Triangle::Triangle(double a, double b, double c) : a_(a), b_(b), c_(c)
{
    bool isInfiniteSides = !std::isfinite(a) || !std::isfinite(b) || !std::isfinite(c);
    bool isNotPositiveSides = a <= 0.0 || b <= 0.0 || c <= 0.0;
    if (isInfiniteSides || isNotPositiveSides)
        throw std::invalid_argument("Triangle sides must be finite and greater than 0.");

    double sides[3] = {a, b, c};
    std::sort(sides, sides + 3);
    bool isInvalidTriangle = (sides[2] - sides[1]) >= sides[0];
    if (isInvalidTriangle)
        throw std::invalid_argument("These sides form an invalid or degenerate triangle.");

    bool willBeOverflow = !std::isfinite(a + b + c);
    if (willBeOverflow)
        throw std::overflow_error("Triangle perimeter exceeds maximum double value.");
}
std::unique_ptr<Figure> Triangle::clone() const {
    return std::make_unique<Triangle>(*this);
}
double Triangle::perimeter() const
{
    return a_ + b_ + c_;
}
std::string Triangle::toString() const
{
    std::ostringstream oss;
    oss << "triangle " << a_ << " " << b_ << " " << c_;

    return oss.str();
}

Circle::Circle(double r) : r_(r)
{
    bool isInvalidRadius = !std::isfinite(r) || r <= 0.0;
    if (isInvalidRadius)
        throw std::invalid_argument("Radius must be finite and greater than 0.");

    bool willBeOverflow = !std::isfinite(2 * std::numbers::pi * r_);
    if (willBeOverflow)
        throw std::overflow_error("Circle perimeter exceeds maximum double value.");
}
std::unique_ptr<Figure> Circle::clone() const {
    return std::make_unique<Circle>(*this);
}
double Circle::perimeter() const
{
    return 2 * std::numbers::pi * r_;
}
std::string Circle::toString() const
{
    std::ostringstream oss;
    oss << "circle " << r_;

    return oss.str();
}

Rectangle::Rectangle(double a, double b) : a_(a), b_(b)
{
    bool isInvalidSides = !std::isfinite(a) || a <= 0.0 || !std::isfinite(b) || b <= 0.0;
    if (isInvalidSides)
        throw std::invalid_argument("Rectangle sides be finite and greater than 0.");

    bool willBeOverflow = !std::isfinite(2 * (a_ + b));
    if (willBeOverflow)
        throw std::overflow_error("Rectangle perimeter exceeds maximum double value.");
}
std::unique_ptr<Figure> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);
}
double Rectangle::perimeter() const
{
    return 2 * (a_ + b_);
}
std::string Rectangle::toString() const
{
    std::ostringstream oss;
    oss << "rectangle " << a_ << " " << b_;

    return oss.str();
}