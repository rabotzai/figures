#pragma once

#include "factory.hpp"

#include <memory>
#include <string>
#include <istream>
#include <sstream>

enum class TypeFactory
{
    TerminalFactory,
    FileFactory,
    RandomFactory,
};

class FigureFactoryCreator
{
private:
    static TypeFactory parseType(const std::string &source);

public:
    std::unique_ptr<FigureFactory> createFactory(
        const std::string &source, std::istream *stream = nullptr) const;
};