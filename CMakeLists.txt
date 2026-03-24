cmake_minimum_required(VERSION 3.10)
project(TouristBureau)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(includes)

file(GLOB SRC_SOURCES "src/*.cpp")

add_executable(TouristBureau ${SRC_SOURCES})

find_package(SQLite3 REQUIRED)
target_link_libraries(TouristBureau SQLite::SQLite3)

enable_testing()
add_subdirectory(googletest)

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
list(REMOVE_ITEM TEST_SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

file(GLOB SRC_NO_MAIN "src/*.cpp")
list(REMOVE_ITEM SRC_NO_MAIN ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(tests ${TEST_SOURCES} ${SRC_NO_MAIN})
target_link_libraries(tests gtest gtest_main SQLite::SQLite3 pthread)

add_test(NAME tests COMMAND tests)