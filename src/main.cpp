#include "app.hpp"

#include <iostream>

int main()
{
    App app(std::cin, std::cout, std::cerr);
    return app.run();
}
