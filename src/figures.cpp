#include "figures.hpp"

#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <numbers>

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
double Triangle::perimeter() const
{
    return a_ + b_ + c_;
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
double Circle::perimeter() const
{
    return 2 * std::numbers::pi * r_;
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
double Rectangle::perimeter() const
{
    return 2 * (a_ + b_);
}