#include "app.hpp"

#include "factory.hpp"

#include <charconv>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <system_error>
#include <utility>

App::App(std::istream &input, std::ostream &output, std::ostream &error)
    : input_(input), output_(output), error_(error) {}

std::string App::trim(const std::string &text)
{
    constexpr const char *whitespace = " \t\r\n";
    const auto first = text.find_first_not_of(whitespace);
    if (first == std::string::npos)
        return "";
    const auto last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

std::string App::toLower(std::string text)
{
    for (char &ch : text)
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return text;
}

std::optional<std::size_t> App::parsePositiveNumber(const std::string &text)
{
    const std::string cleaned = trim(text);
    if (cleaned.empty())
        return std::nullopt;

    std::size_t value = 0;
    const char *first = cleaned.data();
    const char *last = first + cleaned.size();
    const auto [end, error] = std::from_chars(first, last, value);
    if (error != std::errc{} || end != last || value == 0)
        return std::nullopt;
    return value;
}

std::optional<std::string> App::askNonEmptyLine(const std::string &prompt)
{
    output_ << prompt;
    std::string line;
    while (std::getline(input_, line))
    {
        line = trim(line);
        if (!line.empty())
            return line;
        output_ << "Input cannot be empty. Please try again:\n> ";
    }
    return std::nullopt;
}

std::optional<std::string> App::askSource()
{
    output_ << "Choose input source (stdin/file/random):\n> ";
    std::string line;
    while (std::getline(input_, line))
    {
        line = toLower(trim(line));
        if (line == "stdin" || line == "file" || line == "random")
            return line;
        output_ << "Unknown source. Enter stdin, file or random:\n> ";
    }
    return std::nullopt;
}

std::optional<std::size_t> App::askNumber(
    const std::string &prompt, std::size_t min, std::size_t max)
{
    output_ << prompt;
    std::string line;
    while (std::getline(input_, line))
    {
        auto value = parsePositiveNumber(line);
        if (value && *value >= min && *value <= max)
            return value;
        if (max == std::numeric_limits<std::size_t>::max())
            output_ << "Enter a positive whole number:\n> ";
        else
            output_ << "Enter a whole number from " << min << " to " << max << ":\n> ";
    }
    return std::nullopt;
}

std::optional<std::size_t> App::askFigureIndex()
{
    if (figures_.empty())
    {
        output_ << "No figures in the list.\n";
        return std::nullopt;
    }

    auto number = askNumber("Enter figure number:\n> ", 1, figures_.size());
    if (!number)
        return std::nullopt;
    return *number - 1;
}

App::LoadResult App::loadFigures()
{
    const auto source = askSource();
    if (!source)
        return LoadResult::Cancelled;

    std::ifstream file;
    if (*source == "file")
    {
        auto path = askNonEmptyLine("Enter input file path:\n> ");
        if (!path)
            return LoadResult::Cancelled;

        file.open(*path);
        if (!file.is_open())
        {
            error_ << "Cannot open input file: " << *path << '\n';
            return LoadResult::Error;
        }
    }

    std::unique_ptr<FigureFactory> factory;
    if (*source == "file")
        factory = factoryCreator_.createFactory(*source, &file);
    else if (*source == "stdin")
        factory = factoryCreator_.createFactory(*source, &input_);
    else
        factory = factoryCreator_.createFactory(*source);

    const auto count = askNumber("How many figures?\n> ", 1,
                                 std::numeric_limits<std::size_t>::max());
    if (!count)
        return LoadResult::Cancelled;

    while (figures_.size() < *count)
    {
        if (*source == "stdin")
            output_ << "Enter figure #" << figures_.size() + 1
                    << " (e.g., circle 2.2):\n> ";

        try
        {
            auto figure = factory->create();
            if (!figure)
            {
                if (*source == "stdin")
                {
                    output_ << "Input ended after " << figures_.size()
                            << " of " << *count << " figures.\n";
                    return LoadResult::Cancelled;
                }
                error_ << "Not enough figures in the file: " << figures_.size()
                       << " of " << *count << " loaded.\n";
                return LoadResult::Error;
            }
            figures_.push_back(std::move(figure));
        }
        catch (const std::exception &e)
        {
            error_ << "Cannot create figure #" << figures_.size() + 1
                   << ": " << e.what() << '\n';
            if (*source != "stdin")
                return LoadResult::Error;
            if (input_.eof())
                return LoadResult::Cancelled;
            if (!input_)
                return LoadResult::Error;
            error_ << "Please enter the figure again.\n";
        }
    }

    output_ << "Successfully created " << figures_.size() << " figure(s).\n";
    return LoadResult::Success;
}

void App::writeFigures(std::ostream &stream, bool numberEntries) const
{
    for (std::size_t i = 0; i < figures_.size(); ++i)
    {
        if (numberEntries)
            stream << i + 1 << ". ";
        stream << figures_[i]->toString() << '\n';
    }
}

void App::listFigures() const
{
    if (figures_.empty())
    {
        output_ << "No figures in the list.\n";
        return;
    }
    writeFigures(output_, true);
}

bool App::deleteFigure()
{
    if (figures_.empty())
    {
        output_ << "No figures to delete.\n";
        return true;
    }

    const auto index = askFigureIndex();
    if (!index)
        return false;

    figures_.erase(figures_.begin() + static_cast<std::ptrdiff_t>(*index));
    output_ << "Figure #" << *index + 1 << " deleted.\n";
    return true;
}

bool App::cloneFigure()
{
    if (figures_.empty())
    {
        output_ << "No figures to clone.\n";
        return true;
    }

    const auto index = askFigureIndex();
    if (!index)
        return false;

    auto copy = figures_[*index]->clone();
    if (!copy)
        throw std::logic_error("Figure::clone() returned nullptr");

    figures_.push_back(std::move(copy));
    output_ << "Figure #" << *index + 1
            << " cloned as figure #" << figures_.size() << ".\n";
    return true;
}

bool App::saveFigures()
{
    const auto path = askNonEmptyLine("Enter output file path:\n> ");
    if (!path)
        return false;

    std::ofstream file(*path);
    if (!file.is_open())
    {
        error_ << "Cannot open output file: " << *path << '\n';
        return true;
    }

    writeFigures(file, false);
    file.close();
    if (!file)
    {
        error_ << "Failed to write figures to: " << *path << '\n';
        return true;
    }

    output_ << "Saved " << figures_.size() << " figure(s) to " << *path << ".\n";
    return true;
}

int App::menuLoop()
{
    for (;;)
    {
        output_ << "\nChoose an action:\n"
                << "1. List all figures\n"
                << "2. Delete a figure\n"
                << "3. Clone a figure\n"
                << "4. Save figures to file\n"
                << "5. Exit\n> ";

        const auto action = askNumber("", 1, 5);
        if (!action)
        {
            output_ << "Input ended. Goodbye!\n";
            return 0;
        }

        switch (*action)
        {
        case 1:
            listFigures();
            break;
        case 2:
            if (!deleteFigure())
                return 0;
            break;
        case 3:
            if (!cloneFigure())
                return 0;
            break;
        case 4:
            if (!saveFigures())
                return 0;
            break;
        case 5:
            output_ << "Goodbye!\n";
            return 0;
        }
    }
}

int App::run()
{
    try
    {
        figures_.clear();
        switch (loadFigures())
        {
        case LoadResult::Success:
            return menuLoop();
        case LoadResult::Cancelled:
            output_ << "Input terminated. Goodbye!\n";
            return 0;
        case LoadResult::Error:
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        error_ << "Application error: " << e.what() << '\n';
    }
    return 1;
}
