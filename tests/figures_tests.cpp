#include <gtest/gtest.h>
#include "figures.hpp"

#include <numbers>
#include <stdexcept>
#include <limits>

constexpr double nan = std::numeric_limits<double>::quiet_NaN();
constexpr double inf = std::numeric_limits<double>::infinity();

TEST(TriangleTest, RejectsNonFiniteSides)
{
    EXPECT_THROW((Triangle(inf, 3.0, 4.0)), std::invalid_argument);
    EXPECT_THROW((Triangle(3.0, inf, 4.0)), std::invalid_argument);
    EXPECT_THROW((Triangle(3.0, 4.0, inf)), std::invalid_argument);
    EXPECT_THROW((Triangle(inf, inf, inf)), std::invalid_argument);

    EXPECT_THROW((Triangle(-inf, 3.0, 4.0)), std::invalid_argument);
    EXPECT_THROW((Triangle(3.0, -inf, 4.0)), std::invalid_argument);
    EXPECT_THROW((Triangle(3.0, 4.0, -inf)), std::invalid_argument);
    EXPECT_THROW((Triangle(-inf, -inf, -inf)), std::invalid_argument);

    EXPECT_THROW((Triangle(nan, 3.0, 4.0)), std::invalid_argument);
    EXPECT_THROW((Triangle(3.0, nan, 4.0)), std::invalid_argument);
    EXPECT_THROW((Triangle(3.0, 4.0, nan)), std::invalid_argument);
    EXPECT_THROW((Triangle(nan, nan, nan)), std::invalid_argument);
}
TEST(CircleTest, RejectsNonFiniteRadius)
{
    EXPECT_THROW(Circle{inf}, std::invalid_argument);

    EXPECT_THROW(Circle{-inf}, std::invalid_argument);

    EXPECT_THROW(Circle{nan}, std::invalid_argument);
}
TEST(RectangleTest, RejectsNonFiniteSides)
{
    EXPECT_THROW((Rectangle(inf, 3.0)), std::invalid_argument);
    EXPECT_THROW((Rectangle(3.0, inf)), std::invalid_argument);
    EXPECT_THROW((Rectangle(inf, inf)), std::invalid_argument);

    EXPECT_THROW((Rectangle(-inf, 3.0)), std::invalid_argument);
    EXPECT_THROW((Rectangle(3.0, -inf)), std::invalid_argument);
    EXPECT_THROW((Rectangle(-inf, -inf)), std::invalid_argument);

    EXPECT_THROW((Rectangle(nan, 3.0)), std::invalid_argument);
    EXPECT_THROW((Rectangle(3.0, nan)), std::invalid_argument);
    EXPECT_THROW((Rectangle(nan, nan)), std::invalid_argument);
}

TEST(TriangleTest, RejectsNonPositiveSides)
{
    EXPECT_THROW(Triangle(0.0, 4.0, 5.0), std::invalid_argument);
    EXPECT_THROW(Triangle(3.0, 0.0, 5.0), std::invalid_argument);
    EXPECT_THROW(Triangle(3.0, 4.0, 0.0), std::invalid_argument);
    EXPECT_THROW(Triangle(0.0, 0.0, 0.0), std::invalid_argument);
    EXPECT_THROW(Triangle(-3.0, 4.0, 5.0), std::invalid_argument);
    EXPECT_THROW(Triangle(3.0, -4.0, 5.0), std::invalid_argument);
    EXPECT_THROW(Triangle(3.0, 4.0, -5.0), std::invalid_argument);
    EXPECT_THROW(Triangle(-3.0, -4.0, -5.0), std::invalid_argument);
}
TEST(CircleTest, RejectsNonPositiveRadius)
{
    EXPECT_THROW(Circle(0.0), std::invalid_argument);
    EXPECT_THROW(Circle(-5.0), std::invalid_argument);
}
TEST(RectangleTest, RejectsNonPositiveSides)
{
    EXPECT_THROW(Rectangle(0.0, 4.0), std::invalid_argument);
    EXPECT_THROW(Rectangle(3.0, 0.0), std::invalid_argument);
    EXPECT_THROW(Rectangle(0.0, 0.0), std::invalid_argument);
    EXPECT_THROW(Rectangle(-3.0, 4.0), std::invalid_argument);
    EXPECT_THROW(Rectangle(3.0, -4.0), std::invalid_argument);
    EXPECT_THROW(Rectangle(-3.0, -4.0), std::invalid_argument);
}

TEST(TriangleTest, RejectsTriangleInequalityViolation)
{
    EXPECT_THROW(Triangle(30.0, 4.0, 5.0), std::invalid_argument);
    EXPECT_THROW(Triangle(3.0, 40.0, 5.0), std::invalid_argument);
    EXPECT_THROW(Triangle(3.0, 4.0, 50.0), std::invalid_argument);
}

TEST(TriangleTest, RejectsDegenerateTriangle)
{
    EXPECT_THROW(Triangle(1.0, 2.0, 3.0), std::invalid_argument);
    EXPECT_THROW(Triangle(2.0, 4.0, 6.0), std::invalid_argument);
    EXPECT_THROW(Triangle(5.0, 5.0, 10.0), std::invalid_argument);
}

TEST(TriangleTest, RejectsOverflow)
{
    EXPECT_THROW(Triangle(1e308, 1e308, 1e308), std::overflow_error);
}
TEST(CircleTest, RejectsOverflow)
{
    EXPECT_THROW(Circle(1e308), std::overflow_error);
}
TEST(RectangleTest, RejectsOverflow)
{
    EXPECT_THROW(Rectangle(1e308, 1e308), std::overflow_error);
}

TEST(TriangleTest, AcceptsSmallPositiveSide)
{
    EXPECT_NO_THROW(Triangle(1e-10, 1.0, 1.0));
    EXPECT_NO_THROW(Triangle(1.0, 1e-10, 1.0));
    EXPECT_NO_THROW(Triangle(1.0, 1.0, 1e-10));
    EXPECT_NO_THROW(Triangle(1e-10, 1e-10, 1e-10));
}

TEST(TriangleTest, CalculatesPerimeter)
{
    Triangle tri(3.0, 4.0, 5.0);
    EXPECT_DOUBLE_EQ(tri.perimeter(), 12.0);
}
TEST(CircleTest, CalculatesPerimeter)
{
    Circle circ(2.0);
    EXPECT_DOUBLE_EQ(circ.perimeter(), 4 * std::numbers::pi);
}
TEST(RectangleTest, CalculatesPerimeter)
{
    Rectangle rect(3.0, 4.0);
    EXPECT_DOUBLE_EQ(rect.perimeter(), 14.0);
}

TEST(TriangleTest, ConvertsToString)
{
    Triangle tri(3.0, 4.0, 5.0);
    EXPECT_EQ(tri.to_string(), "triangle 3 4 5");
}
TEST(CircleTest, ConvertsToString)
{
    Circle circ(3.0);
    EXPECT_EQ(circ.to_string(), "circle 3");
}
TEST(RectangleTest, ConvertsToString)
{
    Rectangle rect(3.0, 4.0);
    EXPECT_EQ(rect.to_string(), "rectangle 3 4");
}

TEST(FigureTest, PolymorphicToString)
{
    Triangle tri(3.0, 4.0, 5.0);
    Circle circ(3.0);
    Rectangle rect(3.0, 4.0);

    const Figure &figureT = tri;
    const Figure &figureC = circ;
    const Figure &figureR = rect;

    EXPECT_EQ(figureT.to_string(), "triangle 3 4 5");
    EXPECT_EQ(figureC.to_string(), "circle 3");
    EXPECT_EQ(figureR.to_string(), "rectangle 3 4");
}

TEST(TriangleTest, ConvertsFractionalSidesToString)
{

    EXPECT_EQ(Triangle(2.5, 2.5, 2.5).to_string(), "triangle 2.5 2.5 2.5");
    EXPECT_EQ(Triangle(3.14159, 3.14159, 3.14159).to_string(), "triangle 3.14159 3.14159 3.14159");
    EXPECT_EQ(Triangle(1.23456789, 1.23456789, 1.23456789).to_string(), "triangle 1.23457 1.23457 1.23457");
    EXPECT_EQ(Triangle(123.456789, 123.456789, 123.456789).to_string(), "triangle 123.457 123.457 123.457");
}
TEST(CircleTest, ConvertsFractionalRadiusToString)
{

    EXPECT_EQ(Circle(2.5).to_string(), "circle 2.5");
    EXPECT_EQ(Circle(3.14159).to_string(), "circle 3.14159");
    EXPECT_EQ(Circle(1.23456789).to_string(), "circle 1.23457");
    EXPECT_EQ(Circle(123.456789).to_string(), "circle 123.457");
}
TEST(RectangleTest, ConvertsFractionalSidesToString)
{

    EXPECT_EQ(Rectangle(2.5, 2.5).to_string(), "rectangle 2.5 2.5");
    EXPECT_EQ(Rectangle(3.14159, 3.14159).to_string(), "rectangle 3.14159 3.14159");
    EXPECT_EQ(Rectangle(1.23456789, 1.23456789).to_string(), "rectangle 1.23457 1.23457");
    EXPECT_EQ(Rectangle(123.456789, 123.456789).to_string(), "rectangle 123.457 123.457");
}

TEST(TriangleTest, ClonesCorrectly)
{
    Triangle original(3, 4, 5);
    auto copy = original.clone();

    ASSERT_NE(copy, nullptr);
    EXPECT_NE(copy.get(), &original);
    EXPECT_EQ(copy->perimeter(), original.perimeter());
    EXPECT_EQ(copy->to_string(), original.to_string());
}
TEST(CircleTest, ClonesCorrectly)
{
    Circle original(3);
    auto copy = original.clone();

    ASSERT_NE(copy, nullptr);
    EXPECT_NE(copy.get(), &original);
    EXPECT_EQ(copy->perimeter(), original.perimeter());
    EXPECT_EQ(copy->to_string(), original.to_string());
}
TEST(RectangleTest, ClonesCorrectly)
{
    Rectangle original(3, 4);
    auto copy = original.clone();

    ASSERT_NE(copy, nullptr);
    EXPECT_NE(copy.get(), &original);
    EXPECT_EQ(copy->perimeter(), original.perimeter());
    EXPECT_EQ(copy->to_string(), original.to_string());
}

TEST(FigureTest, PolymorphicCloning)
{
    Triangle originalT(3, 4, 5);
    const Figure &figureT = originalT;
    auto copyT = figureT.clone();

    ASSERT_NE(copyT, nullptr);
    EXPECT_NE(copyT.get(), &originalT);
    EXPECT_NE(dynamic_cast<Triangle*>(copyT.get()), nullptr);
    EXPECT_EQ(copyT->perimeter(), originalT.perimeter());
    EXPECT_EQ(copyT->to_string(), originalT.to_string());

    Circle originalC(3);
    const Figure &figureC = originalC;
    auto copyC = figureC.clone();

    ASSERT_NE(copyC, nullptr);
    EXPECT_NE(copyC.get(), &originalC);
    EXPECT_NE(dynamic_cast<Circle*>(copyC.get()), nullptr);
    EXPECT_EQ(copyC->perimeter(), originalC.perimeter());
    EXPECT_EQ(copyC->to_string(), originalC.to_string());

    Rectangle originalR(3, 4);
    const Figure &figureR = originalR;
    auto copyR = figureR.clone();

    ASSERT_NE(copyR, nullptr);
    EXPECT_NE(copyR.get(), &originalR);
    EXPECT_NE(dynamic_cast<Rectangle*>(copyR.get()), nullptr);
    EXPECT_EQ(copyR->perimeter(), originalR.perimeter());
    EXPECT_EQ(copyR->to_string(), originalR.to_string());
}