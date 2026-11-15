#pragma once

#include <string>
#include <memory>

class IStringConvertible {
    public:
        virtual std::string to_string() const = 0;
        virtual ~IStringConvertible() = default;
};

class Figure : public IStringConvertible
{    
public:
    virtual double perimeter() const = 0;
    virtual std::unique_ptr<Figure> clone() const = 0;
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
    std::unique_ptr<Figure> clone() const override;

    double perimeter() const override;
    std::string to_string() const override;
};
class Circle : public Figure
{
private:
    const double r_;

public:
    Circle(double);
    std::unique_ptr<Figure> clone() const override;

    double perimeter() const override;
    std::string to_string() const override;
};
class Rectangle : public Figure
{
private:
    const double a_;
    const double b_;

public:
    Rectangle(double, double);
    std::unique_ptr<Figure> clone() const override;

    double perimeter() const override;
    std::string to_string() const override;
};
