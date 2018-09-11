cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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

set(PACKAGES_SOURCES
	src/packages/package.cpp)

set(UTIL_SOURCES
	src/utilities/version.cpp
	src/utilities/range.cpp
	src/utilities/person.cpp
	src/utilities/validate.cpp
	src/utilities/utility.cpp)

add_executable(${PROJECT_NAME}
	${MAIN_SOURCES}
	${CLIENT_SOURCES}
	${PACKAGES_SOURCES}
	${UTIL_SOURCES})

add_custom_target(game)
add_dependencies(game ${PROJECT_NAME})

# Generate Unit Tests Executable

set(TEST_SOURCES
	tests/client/window.cpp
	tests/utilities/version.cpp
	tests/utilities/range.cpp
	tests/utilities/person.cpp
	tests/utilities/validate.cpp
	tests/utilities/utility.cpp
	tests/packages/package.cpp)

add_executable(${UNIT_TESTS_EXECUTABLE}
	${TEST_SOURCES}
	${CLIENT_SOURCES}
	${PACKAGES_SOURCES}
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

# Include Lua

set(LUA_SOURCE_PATH lib/lua/src)
set(COMMON_CFLAGS "${COMMON_CFLAGS} -DLUA_USE_LINUX")

add_library(lua
	${LUA_SOURCE_PATH}/linit.c
	${LUA_SOURCE_PATH}/lundump.c
	${LUA_SOURCE_PATH}/ldblib.c
	${LUA_SOURCE_PATH}/lauxlib.c
	${LUA_SOURCE_PATH}/lstrlib.c
	${LUA_SOURCE_PATH}/lobject.c
	${LUA_SOURCE_PATH}/ldebug.c
	${LUA_SOURCE_PATH}/loslib.c
	${LUA_SOURCE_PATH}/lfunc.c
	${LUA_SOURCE_PATH}/lgc.c
	${LUA_SOURCE_PATH}/lcorolib.c
	${LUA_SOURCE_PATH}/lstate.c
	${LUA_SOURCE_PATH}/ltable.c
	${LUA_SOURCE_PATH}/lutf8lib.c
	${LUA_SOURCE_PATH}/ltm.c
	${LUA_SOURCE_PATH}/lparser.c
	${LUA_SOURCE_PATH}/lopcodes.c
	${LUA_SOURCE_PATH}/loadlib.c
	${LUA_SOURCE_PATH}/ldo.c
	${LUA_SOURCE_PATH}/llex.c
	${LUA_SOURCE_PATH}/lcode.c
	${LUA_SOURCE_PATH}/ltablib.c
	${LUA_SOURCE_PATH}/lctype.c
	${LUA_SOURCE_PATH}/lmathlib.c
	${LUA_SOURCE_PATH}/lvm.c
	${LUA_SOURCE_PATH}/lbitlib.c
	${LUA_SOURCE_PATH}/lzio.c
	${LUA_SOURCE_PATH}/lmem.c
	${LUA_SOURCE_PATH}/lapi.c
	${LUA_SOURCE_PATH}/lbaselib.c
	${LUA_SOURCE_PATH}/ldump.c
	${LUA_SOURCE_PATH}/liolib.c
	${LUA_SOURCE_PATH}/lstring.c)

set(LUA_LIBRARIES lua)

include_directories(${LUA_SOURCE_PATH})

target_link_libraries(${PROJECT_NAME}
	${LUA_LIBRARIES})
target_link_libraries(${UNIT_TESTS_EXECUTABLE}
	${LUA_LIBRARIES})

# Include Catch2

add_library(catch2 STATIC EXCLUDE_FROM_ALL
	lib/catch2/catch.cpp
	lib/catch2/catch.hpp)

set(CATCH_LIBRARIES catch2)

target_link_libraries(${UNIT_TESTS_EXECUTABLE}
	${CATCH_LIBRARIES})

# COMMON_CFLAGS has no effect without this line
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_CFLAGS}")
