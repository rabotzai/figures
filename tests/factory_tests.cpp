#include <gtest/gtest.h>
#include "figures.hpp"
#include "factory.hpp"

#include <string>
#include <numbers>
#include <sstream>
#include <cmath>

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

TEST(StreamFactoryTest, ReadsFiguresSequentially)
{
    std::istringstream input(
        "triangle 3 4 5\n"
        "circle 2\n"
        "rectangle 3 4\n");
    StreamFigureFactory factory(input);

    auto first = factory.create();
    ASSERT_NE(first, nullptr);
    EXPECT_NE(dynamic_cast<Triangle *>(first.get()), nullptr);
    EXPECT_EQ(first->toString(), "triangle 3 4 5");

    auto second = factory.create();
    ASSERT_NE(second, nullptr);
    EXPECT_NE(dynamic_cast<Circle *>(second.get()), nullptr);
    EXPECT_EQ(second->toString(), "circle 2");

    auto third = factory.create();
    ASSERT_NE(third, nullptr);
    EXPECT_NE(dynamic_cast<Rectangle *>(third.get()), nullptr);
    EXPECT_EQ(third->toString(), "rectangle 3 4");

    auto forth = factory.create();
    EXPECT_EQ(forth, nullptr);
}
TEST(StreamFactoryTest, ReadsLastLineWithoutNewline)
{
    std::istringstream input("circle 2");
    StreamFigureFactory factory(input);

    auto figure = factory.create();
    ASSERT_NE(figure, nullptr);
    EXPECT_NE(dynamic_cast<Circle *>(figure.get()), nullptr);
    EXPECT_EQ(figure->toString(), "circle 2");

    EXPECT_EQ(factory.create(), nullptr);
}

TEST(StreamFactoryTest, RejectsEmptyLine)
{
    std::istringstream input("\n");
    StreamFigureFactory factory(input);

    EXPECT_THROW(factory.create(), std::invalid_argument);
}
TEST(StreamFactoryTest, ContinuesAfterEmptyLine)
{
    std::istringstream input(
        "triangle 3 4 5\n"
        "\n"
        "rectangle 3 4\n");
    StreamFigureFactory factory(input);

    auto first = factory.create();
    ASSERT_NE(first, nullptr);
    EXPECT_NE(dynamic_cast<Triangle *>(first.get()), nullptr);
    EXPECT_EQ(first->toString(), "triangle 3 4 5");

    EXPECT_THROW(factory.create(), std::invalid_argument);

    auto third = factory.create();
    ASSERT_NE(third, nullptr);
    EXPECT_NE(dynamic_cast<Rectangle *>(third.get()), nullptr);
    EXPECT_EQ(third->toString(), "rectangle 3 4");

    auto forth = factory.create();
    EXPECT_EQ(forth, nullptr);
}

TEST(StreamFactoryTest, ThrowsOnStreamFailure)
{
    std::istringstream input("circle 2");
    input.setstate(std::ios::badbit);
    StreamFigureFactory factory(input);

    EXPECT_THROW(factory.create(), std::ios_base::failure);
}

TEST(RandomFactoryTest, CreatesValidFigures)
{
    RandomFigureFactory factory;

    for (std::size_t i = 0; i < 1000; ++i)
    {
        auto figure = factory.create();

        ASSERT_NE(figure, nullptr);

        bool isTriangle = dynamic_cast<Triangle *>(figure.get()) != nullptr;
        bool isCircle = dynamic_cast<Circle *>(figure.get()) != nullptr;
        bool isRectangle = dynamic_cast<Rectangle *>(figure.get()) != nullptr;

        EXPECT_TRUE(isTriangle || isCircle || isRectangle);
        EXPECT_TRUE(std::isfinite(figure->perimeter()));
        EXPECT_GT(figure->perimeter(), 0.0);
    }
}

TEST(RandomFactoryTest, GeneratesAllFigureTypes)
{
    RandomFigureFactory factory;

    int triangles = 0;
    int circles = 0;
    int rectangles = 0;

    for (std::size_t i = 0; i < 3000; ++i)
    {
        auto figure = factory.create();

        ASSERT_NE(figure, nullptr);

        bool isTriangle = dynamic_cast<Triangle *>(figure.get()) != nullptr;
        bool isCircle = dynamic_cast<Circle *>(figure.get()) != nullptr;
        bool isRectangle = dynamic_cast<Rectangle *>(figure.get()) != nullptr;

        if (isTriangle)
        {
            triangles++;
        }
        else if (isCircle)
        {
            circles++;
        }
        else if (isRectangle)
        {
            rectangles++;
        }
    }

    int figures = triangles + circles + rectangles;

    EXPECT_GT(triangles, 0);
    EXPECT_GT(circles, 0);
    EXPECT_GT(rectangles, 0);
    EXPECT_EQ(figures, 3000);
}

TEST(RandomFactoryTest, CircleRadiusWithinRange)
{
    RandomFigureFactory factory;
    int circles = 0;

    for (std::size_t i = 0; i < 1000; ++i)
    {
        auto figure = factory.create();

        ASSERT_NE(figure, nullptr);

        double perimeter = figure->perimeter();

        if (dynamic_cast<Circle *>(figure.get()) != nullptr)
        {
            double radius = perimeter / 2 / std::numbers::pi;

            EXPECT_GE(radius, 1.0);
            EXPECT_LT(radius, 100.0);

            circles++;
        }
    }

    EXPECT_GT(circles, 0);
}

TEST(RandomFactoryTest, SameSeedProducesSameSequence)
{
    RandomFigureFactory factory1(42);
    RandomFigureFactory factory2(42);

    for (std::size_t i = 0; i < 100; ++i)
    {
        auto figure1 = factory1.create();
        auto figure2 = factory2.create();

        ASSERT_NE(figure1, nullptr);
        ASSERT_NE(figure2, nullptr);

        EXPECT_DOUBLE_EQ(figure1->perimeter(), figure2->perimeter());
        EXPECT_EQ(figure1->toString(), figure2->toString());
    }
}
TEST(RandomFactoryTest, FactoriesHaveIndependentGenerators)
{
    RandomFigureFactory factory1(42);
    RandomFigureFactory factory2(42);
    RandomFigureFactory factory3(123);

    for (std::size_t i = 0; i < 100; ++i)
    {
        auto figure1 = factory1.create();
        factory3.create();
        factory3.create();
        factory3.create();
        auto figure2 = factory2.create();

        ASSERT_NE(figure1, nullptr);
        ASSERT_NE(figure2, nullptr);

        EXPECT_DOUBLE_EQ(figure1->perimeter(), figure2->perimeter());
        EXPECT_EQ(figure1->toString(), figure2->toString());
    }
}