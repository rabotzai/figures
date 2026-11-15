#pragma once

class Figure
{    
public:
    virtual double perimeter() const = 0;
    virtual ~Figure() = default;
};

class Triangle : public Figure
{
private:
    const double a_;
    const double b_;
    const double c_;

public:
    Triangle(double, double, double);
    double perimeter() const override;
};
class Circle : public Figure
{
private:
    const double r_;

public:
    Circle(double);
    double perimeter() const override;
};
class Rectangle : public Figure
{
private:
    const double a_;
    const double b_;

public:
    Rectangle(double, double);
    double perimeter() const override;
};
