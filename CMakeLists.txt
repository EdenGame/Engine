cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_NAME "ProjectEden")

project(${PROJECT_NAME})

# Main Sources

set(MAIN_SOURCES
	src/main.cpp)

set(CLIENT_SOURCES
	src/client/window.cpp)

add_executable(${PROJECT_NAME}
	${MAIN_SOURCES}
	${CLIENT_SOURCES})

# Include SDL2

find_package(PkgConfig REQUIRED)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

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
