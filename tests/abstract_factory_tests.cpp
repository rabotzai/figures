#include <gtest/gtest.h>

#include "abstract_factory.hpp"
#include "factory.hpp"
#include "figures.hpp"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

TEST(AbstractFactoryTest, CreatesRandomFactoryWithoutStream)
{
    FigureFactoryCreator creator;
    auto factory = creator.createFactory("random");

    ASSERT_NE(factory, nullptr);
    EXPECT_NE(dynamic_cast<RandomFigureFactory*>(factory.get()), nullptr);
    EXPECT_NE(factory->create(), nullptr);
}

TEST(AbstractFactoryTest, CreatesStreamFactoryForStdin)
{
    std::istringstream input("circle 2\n");
    FigureFactoryCreator creator;
    auto factory = creator.createFactory("stdin", &input);

    ASSERT_NE(factory, nullptr);
    EXPECT_NE(dynamic_cast<StreamFigureFactory*>(factory.get()), nullptr);
    auto figure = factory->create();
    ASSERT_NE(figure, nullptr);
    EXPECT_EQ(figure->toString(), "circle 2");
}

TEST(AbstractFactoryTest, CreatesStreamFactoryForFile)
{
    std::istringstream input("rectangle 3 4\n");
    FigureFactoryCreator creator;
    auto factory = creator.createFactory("file", &input);

    ASSERT_NE(factory, nullptr);
    EXPECT_NE(dynamic_cast<StreamFigureFactory*>(factory.get()), nullptr);
    auto figure = factory->create();
    ASSERT_NE(figure, nullptr);
    EXPECT_EQ(figure->toString(), "rectangle 3 4");
}

TEST(AbstractFactoryTest, RejectsUnknownInputSource)
{
    FigureFactoryCreator creator;
    EXPECT_THROW(creator.createFactory("unknown"), std::invalid_argument);
    EXPECT_THROW(creator.createFactory(""), std::invalid_argument);
}

TEST(AbstractFactoryTest, FileRequiresStream)
{
    FigureFactoryCreator creator;
    EXPECT_THROW(creator.createFactory("file"), std::invalid_argument);
}

TEST(AbstractFactoryTest, RejectsFailedFileStream)
{
    std::istringstream input("circle 2\n");
    input.setstate(std::ios::badbit);

    FigureFactoryCreator creator;
    EXPECT_THROW(creator.createFactory("file", &input), std::ios_base::failure);
}

TEST(AbstractFactoryTest, FactorySurvivesCreatorDestruction)
{
    std::istringstream input("triangle 3 4 5\n");
    std::unique_ptr<FigureFactory> factory;
    {
        FigureFactoryCreator creator;
        factory = creator.createFactory("stdin", &input);
    }

    auto figure = factory->create();
    ASSERT_NE(figure, nullptr);
    EXPECT_EQ(figure->toString(), "triangle 3 4 5");
}
