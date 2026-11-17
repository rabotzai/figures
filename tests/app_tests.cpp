#include "app.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>
#include <string>
#include <vector>

namespace
{
    std::filesystem::path tempPath(const std::string &name)
    {
        static std::atomic<unsigned> sequence{0};
        return std::filesystem::temp_directory_path() /
               ("figures_step7_" + name + "_" +
                std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
                "_" + std::to_string(sequence++) + ".txt");
    }

    struct TemporaryFile
    {
        std::filesystem::path path;
        explicit TemporaryFile(const std::string &name) : path(tempPath(name)) {}
        ~TemporaryFile()
        {
            std::error_code ignored;
            std::filesystem::remove(path, ignored);
        }
    };

    std::string readFile(const std::filesystem::path &path)
    {
        std::ifstream file(path);
        std::ostringstream result;
        result << file.rdbuf();
        return result.str();
    }
}

TEST(AppTest, ReadsStdinRetriesInvalidFigureAndSupportsMenuActions)
{
    TemporaryFile outputFile("roundtrip");
    const std::string inputText =
        "stdin\n2\ncircle -5\ncircle 5\nrectangle 3 4\n"
        "1\n3\n1\n2\n2\n4\n" +
        outputFile.path.string() + "\n5\n";

    std::istringstream input(inputText);
    std::ostringstream output, error;
    App app(input, output, error);

    EXPECT_EQ(app.run(), 0);
    EXPECT_NE(error.str().find("Cannot create figure #1"), std::string::npos);
    EXPECT_NE(output.str().find("Figure #1 cloned as figure #3"), std::string::npos);
    EXPECT_NE(output.str().find("Figure #2 deleted"), std::string::npos);
    EXPECT_EQ(readFile(outputFile.path), "circle 5\ncircle 5\n");
}

TEST(AppTest, LoadsFileAndWritesPlainFigureRepresentations)
{
    TemporaryFile inputFile("input"), outputFile("output");
    {
        std::ofstream file(inputFile.path);
        file << "circle 2\ntriangle 3 4 5\n";
    }

    std::istringstream input("file\n" + inputFile.path.string() +
                             "\n2\n4\n" + outputFile.path.string() + "\n5\n");
    std::ostringstream output, error;
    App app(input, output, error);

    EXPECT_EQ(app.run(), 0);
    EXPECT_TRUE(error.str().empty());
    EXPECT_EQ(readFile(outputFile.path), "circle 2\ntriangle 3 4 5\n");
}

TEST(AppTest, RandomSourceCreatesRequestedCount)
{
    std::istringstream input("random\n3\n1\n5\n");
    std::ostringstream output, error;
    App app(input, output, error);

    EXPECT_EQ(app.run(), 0);
    EXPECT_NE(output.str().find("Successfully created 3 figure(s)"), std::string::npos);
    EXPECT_TRUE(error.str().empty());
}

TEST(AppTest, ReportsShortFileAsError)
{
    TemporaryFile inputFile("short");
    {
        std::ofstream file(inputFile.path);
        file << "circle 2\n";
    }
    std::istringstream input("file\n" + inputFile.path.string() + "\n2\n");
    std::ostringstream output, error;
    App app(input, output, error);

    EXPECT_EQ(app.run(), 1);
    EXPECT_NE(error.str().find("Not enough figures in the file"), std::string::npos);
}

TEST(AppTest, ReportsMalformedFileAsError)
{
    TemporaryFile inputFile("bad");
    {
        std::ofstream file(inputFile.path);
        file << "rectangle a b\n";
    }
    std::istringstream input("file\n" + inputFile.path.string() + "\n1\n");
    std::ostringstream output, error;
    App app(input, output, error);

    EXPECT_EQ(app.run(), 1);
    EXPECT_NE(error.str().find("Cannot create figure #1"), std::string::npos);
}

TEST(AppTest, RejectsInvalidMenuAndIndexButContinues)
{
    std::istringstream input("stdin\n1\ncircle 3\nxyz\n2\n99\n1\n1\n5\n");
    std::ostringstream output, error;
    App app(input, output, error);

    EXPECT_EQ(app.run(), 0);
    EXPECT_NE(output.str().find("Figure #1 deleted"), std::string::npos);
    EXPECT_NE(output.str().find("No figures in the list"), std::string::npos);
}

TEST(AppTest, EndsGracefullyOnEof)
{
    std::istringstream input("random\n2\n");
    std::ostringstream output, error;
    App app(input, output, error);

    EXPECT_EQ(app.run(), 0);
    EXPECT_NE(output.str().find("Input ended. Goodbye!"), std::string::npos);
}
