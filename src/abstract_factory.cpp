#include "abstract_factory.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

TypeFactory FigureFactoryCreator::parseType(const std::string &source)
{
    if (source == "stdin")
        return TypeFactory::TerminalFactory;
    if (source == "file")
        return TypeFactory::FileFactory;
    if (source == "random")
        return TypeFactory::RandomFactory;

    throw std::invalid_argument("Unknown input source: " + source);
}

std::unique_ptr<FigureFactory> FigureFactoryCreator::createFactory(
    const std::string &source, std::istream *stream) const
{
    switch (parseType(source))
    {
    case TypeFactory::TerminalFactory:
        return std::make_unique<StreamFigureFactory>(stream ? *stream : std::cin);

    case TypeFactory::FileFactory:
        if (stream == nullptr)
            throw std::invalid_argument("File input requires an input stream");
        if (!*stream)
            throw std::ios_base::failure("File input stream is not ready");
        return std::make_unique<StreamFigureFactory>(*stream);

    case TypeFactory::RandomFactory:
        return std::make_unique<RandomFigureFactory>();
    }

    throw std::logic_error("Unsupported factory type");
}
