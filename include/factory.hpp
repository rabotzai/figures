#pragma once

#include "figures.hpp"
#include <string>

class StringToFigure
{
public:
    std::unique_ptr<Figure> createFrom(const std::string& representation) const;
};