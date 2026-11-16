#pragma once

#include "figures.hpp"
#include <string>
#include <memory>
#include <sstream>
#include <random>

class StringToFigure
{
public:
    std::unique_ptr<Figure> createFrom(const std::string &representation) const;
};

class FigureFactory
{
public:
    virtual std::unique_ptr<Figure> create() = 0;
    virtual ~FigureFactory() = default;
};
class StreamFigureFactory : public FigureFactory
{
private:
    std::istream &input_;
    StringToFigure stringToFigure;

public:
    StreamFigureFactory(std::istream &);
    std::unique_ptr<Figure> create() override;
};
class RandomFigureFactory : public FigureFactory
{
private:
    std::mt19937 generator_ = std::mt19937(std::random_device{}());
    int generateTypeFigure() noexcept;

public:
    RandomFigureFactory() = default;
    explicit RandomFigureFactory(std::mt19937::result_type seed);

    std::unique_ptr<Figure> create() override;
};
