cmake_minimum_required(VERSION 3.19.2)
project(automaton CXX)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")

file(GLOB_RECURSE SOURCES "automaton/*.cpp")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtkmm-3.0 cairomm-1.0 glibmm-2.4)

add_executable(${PROJECT_NAME} automaton/main.cpp ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${GTK3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})
