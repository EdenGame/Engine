cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME "ProjectEden")
set(UNIT_TESTS_EXECUTABLE "UnitTests")

project(${PROJECT_NAME})

# Project Configuration Variables

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

configure_file(config.h.in src/config.h)

# Generate Main Executable

set(MAIN_SOURCES
	src/main.cpp)

set(CLIENT_SOURCES
	src/client/client.cpp
	src/client/window.cpp)

set(UTIL_SOURCES
	src/utilities/version.cpp
	src/utilities/range.cpp
	src/utilities/person.cpp)

add_executable(${PROJECT_NAME}
	${MAIN_SOURCES}
	${CLIENT_SOURCES}
	${UTIL_SOURCES})

add_custom_target(game)
add_dependencies(game ${PROJECT_NAME})

# Generate Unit Tests Executable

set(TEST_SOURCES
	tests/client/window.cpp
	tests/utilities/version.cpp
	tests/utilities/range.cpp
	tests/utilities/person.cpp)

add_executable(${UNIT_TESTS_EXECUTABLE}
	${TEST_SOURCES}
	${CLIENT_SOURCES}
	${UTIL_SOURCES})

target_include_directories(${UNIT_TESTS_EXECUTABLE} PUBLIC src)
add_custom_target(tests)
add_dependencies(tests ${UNIT_TESTS_EXECUTABLE})

# Include SDL2

find_package(PkgConfig REQUIRED)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${UNIT_TESTS_EXECUTABLE} ${SDL2_LIBRARIES})

# Include GLAD

add_library(glad
	lib/glad/glad.c
	lib/glad/glad.h)

target_link_libraries(glad
	${CMAKE_DL_LIBS})

set(GLAD_LIBRARIES glad)

include_directories(lib)

target_link_libraries(${PROJECT_NAME}
	${GLAD_LIBRARIES})
target_link_libraries(${UNIT_TESTS_EXECUTABLE}
	${GLAD_LIBRARIES})

# Include JsonCPP

add_library(json
	lib/json/jsoncpp.cpp
	lib/json/json.h
	lib/json/json-forwards.h)

set(JSON_LIBRARIES json)

target_link_libraries(${PROJECT_NAME}
	${JSON_LIBRARIES})
target_link_libraries(${UNIT_TESTS_EXECUTABLE}
	${JSON_LIBRARIES})

# Include Catch2

add_library(catch2 STATIC EXCLUDE_FROM_ALL
	lib/catch2/catch.cpp
	lib/catch2/catch.hpp)

set(CATCH_LIBRARIES catch2)

target_link_libraries(${UNIT_TESTS_EXECUTABLE}
	${CATCH_LIBRARIES})
