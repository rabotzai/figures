#include "factory.hpp"

#include "figures.hpp"
#include <string>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <random>
#include <cmath>

std::unique_ptr<Figure> StringToFigure::createFrom(const std::string &representation) const
{
    std::istringstream iss(representation);

    std::string type;
    double a, b, c;
    if (!(iss >> type))
    {
        throw std::invalid_argument("Empty or invalid representation");
    }

    if (type == "triangle")
    {
        if (!(iss >> a >> b >> c))
        {
            throw std::invalid_argument("Invalid parameters for triangle");
        }
    }
    else if (type == "circle")
    {
        if (!(iss >> a))
        {
            throw std::invalid_argument("Invalid parameters for circle");
        }
    }
    else if (type == "rectangle")
    {
        if (!(iss >> a >> b))
        {
            throw std::invalid_argument("Invalid parameters for rectangle");
        }
    }
    else
    {
        throw std::invalid_argument("Unknown figure type: " + type);
    }

    std::string extra;
    if (iss >> extra)
    {
        throw std::invalid_argument("Too many arguments in representation");
    }

    if (type == "triangle")
    {
        return std::make_unique<Triangle>(a, b, c);
    }
    else if (type == "circle")
    {
        return std::make_unique<Circle>(a);
    }
    else if (type == "rectangle")
    {
        return std::make_unique<Rectangle>(a, b);
    }

    throw std::logic_error("Unexpected figure type");
}

StreamFigureFactory::StreamFigureFactory(std::istream& input) : input_(input) {}
std::unique_ptr<Figure> StreamFigureFactory::create()
{
    std::string representation;
    if (!std::getline(input_, representation))
    {
        if (input_.eof() && !input_.bad())
            return nullptr;

        throw std::ios_base::failure("Critical error occurred while reading stream");
    }
    if (representation.empty())
        throw std::invalid_argument("Empty line in figure stream");

    return stringToFigure.createFrom(representation);
}

RandomFigureFactory::RandomFigureFactory(std::mt19937::result_type seed) : generator_(seed) {}
int RandomFigureFactory::generateTypeFigure() noexcept
{
    std::uniform_int_distribution<int> distribution(1, 3);
    int type = distribution(generator_);

    return type;
}
std::unique_ptr<Figure> RandomFigureFactory::create()
{
    std::uniform_real_distribution<double> distribution(1.0, 100.0);

    int type = generateTypeFigure();
    switch (type)
    {
    case 1:
    {
        double a = distribution(generator_);
        double b = distribution(generator_);
        double c;

        do
        {
            c = distribution(generator_);
        } while (c <= std::abs(a - b) || c >= a + b);

        return std::make_unique<Triangle>(a, b, c);
    }
    case 2:
    {
        double r = distribution(generator_);

        return std::make_unique<Circle>(r);
    }
    case 3:
    {
        double a = distribution(generator_);
        double b = distribution(generator_);

        return std::make_unique<Rectangle>(a, b);
    }
    }

    throw std::logic_error("something came up");
}