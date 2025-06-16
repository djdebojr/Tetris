cmake_minimum_required(VERSION 3.10)
project(TetrisConsole)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная игра
add_executable(tetris
    main.cpp
    src/GameEngine.cpp
)

# Подключаем include-папку с заголовочными файлами
target_include_directories(tetris PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Тесты
enable_testing()

# Добавляем GoogleTest через FetchContent (если используешь его)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Тестовый проект
add_executable(tetris_test
    test/test_game_engine.cpp
    src/GameEngine.cpp
)

target_include_directories(tetris_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(tetris_test gtest_main)

include(GoogleTest)
gtest_discover_tests(tetris_test)
