
cmake_minimum_required(VERSION 3.20)

project(MyProject LANGUAGES CXX)

# 1. Основная библиотека
add_library(figures_lib STATIC
    src/figures.cpp
    src/factory.cpp
    src/abstract_factory.cpp
    src/app.cpp
)

target_compile_features(figures_lib PUBLIC cxx_std_20)

target_include_directories(figures_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 2. Приложение
add_executable(app src/main.cpp)

target_link_libraries(app PRIVATE figures_lib)

# 3. Включаем поддержку тестирования
include(CTest)

if(BUILD_TESTING)

    # 4. Подключаем GoogleTest
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.18.0
        GIT_SHALLOW TRUE
    )

    # Для совместимости runtime на Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    # 5. Программа с тестами
    add_executable(figures_tests
        tests/figures_tests.cpp
        tests/factory_tests.cpp
        tests/abstract_factory_tests.cpp
        tests/app_tests.cpp
    )

    target_link_libraries(figures_tests
        PRIVATE
            figures_lib
            GTest::gtest_main
    )

    # 6. Регистрация тестов
    include(GoogleTest)
    gtest_discover_tests(figures_tests)

endif()
