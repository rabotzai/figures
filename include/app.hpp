#pragma once

#include "abstract_factory.hpp"
#include "figures.hpp"

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class App
{
public:
    App(std::istream &input, std::ostream &output, std::ostream &error);

    int run();

private:
    enum class LoadResult
    {
        Success,
        Cancelled,
        Error
    };

    std::istream &input_;
    std::ostream &output_;
    std::ostream &error_;
    FigureFactoryCreator factoryCreator_;
    std::vector<std::unique_ptr<Figure>> figures_;

    static std::string trim(const std::string &text);
    static std::string toLower(std::string text);
    static std::optional<std::size_t> parsePositiveNumber(const std::string &text);

    std::optional<std::string> askNonEmptyLine(const std::string &prompt);
    std::optional<std::string> askSource();
    std::optional<std::size_t> askNumber(
        const std::string &prompt, std::size_t min, std::size_t max);
    std::optional<std::size_t> askFigureIndex();

    LoadResult loadFigures();
    int menuLoop();
    void writeFigures(std::ostream &stream, bool numberEntries) const;
    void listFigures() const;
    bool deleteFigure();
    bool cloneFigure();
    bool saveFigures();
};
