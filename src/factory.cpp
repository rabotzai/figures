#include <figures.hpp>
#include "factory.hpp"

#include <memory>
#include <sstream>
#include <stdexcept>

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