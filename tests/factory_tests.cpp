#include <gtest/gtest.h>
#include "factory.hpp"
#include "figures.hpp"

#include <string>
#include <numbers>

TEST(FactoryTest, CreatesTriangle)
{
    std::string representation = "triangle 3 4 5";

    StringToFigure factory;
    auto figure = factory.createFrom(representation);

    ASSERT_NE(figure.get(), nullptr);
    EXPECT_NE(dynamic_cast<Triangle *>(figure.get()), nullptr);
    EXPECT_EQ(figure->toString(), representation);
    EXPECT_DOUBLE_EQ(figure->perimeter(), 12.0);
}
TEST(FactoryTest, CreatesCircle)
{
    std::string representation = "circle 2.5";

    StringToFigure factory;
    auto figure = factory.createFrom(representation);

    ASSERT_NE(figure.get(), nullptr);
    EXPECT_NE(dynamic_cast<Circle *>(figure.get()), nullptr);
    EXPECT_EQ(figure->toString(), representation);
    EXPECT_DOUBLE_EQ(figure->perimeter(), 5 * std::numbers::pi);
}
TEST(FactoryTest, CreatesRectangle)
{
    std::string representation = "rectangle 3 4";

    StringToFigure factory;
    auto figure = factory.createFrom(representation);

    ASSERT_NE(figure.get(), nullptr);
    EXPECT_NE(dynamic_cast<Rectangle *>(figure.get()), nullptr);
    EXPECT_EQ(figure->toString(), representation);
    EXPECT_DOUBLE_EQ(figure->perimeter(), 14.0);
}

TEST(FactoryTest, RejectsEmptyRepresentation)
{
    StringToFigure factory;
    EXPECT_THROW(factory.createFrom(""), std::invalid_argument);
}
TEST(FactoryTest, RejectsUnknownFigureType)
{
    StringToFigure factory;
    EXPECT_THROW(factory.createFrom("square 3 4"), std::invalid_argument);
}
TEST(FactoryTest, RejectsMissingParameters)
{
    StringToFigure factory;
    EXPECT_THROW(factory.createFrom("triangle 3 4"), std::invalid_argument);
}
TEST(FactoryTest, RejectsInvalidParameterType)
{
    StringToFigure factory;
    EXPECT_THROW(factory.createFrom("circle abc"), std::invalid_argument);
}
TEST(FactoryTest, RejectsExtraParameters)
{
    StringToFigure factory;
    EXPECT_THROW(factory.createFrom("circle 5 abc"), std::invalid_argument);
}

TEST(FactoryTest, RejectsInvalidDimensions)
{
    StringToFigure factory;
    EXPECT_THROW(factory.createFrom("circle -5"), std::invalid_argument);
    EXPECT_THROW(factory.createFrom("rectangle 0 4"), std::invalid_argument);
    EXPECT_THROW(factory.createFrom("triangle 1 2 3"), std::invalid_argument);
}
TEST(FactoryTest, PropagatesOverflow)
{
    StringToFigure factory;
    EXPECT_THROW(factory.createFrom("circle 1e308"), std::overflow_error);
    EXPECT_THROW(factory.createFrom("rectangle 1e308 1e308"), std::overflow_error);
    EXPECT_THROW(factory.createFrom("triangle 1e308 1e308 1e308"), std::overflow_error);
}
TEST(FactoryTest, RejectsExtraParametersBeforeConstruction)
{
    StringToFigure factory;
    EXPECT_THROW(factory.createFrom("circle 1e308 abc"), std::invalid_argument);
}