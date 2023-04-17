cmake_minimum_required(VERSION 3.3)
project(uppexo DESCRIPTION "General purpose vulkan engine")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED)
include_directories(${library_INCLUDE_DIRS} include/)

file(GLOB_RECURSE srcs src/*.cpp)
file(GLOB demo demo/*.cpp)

add_library(uppexo SHARED ${srcs})
set_property(TARGET uppexo PROPERTY CXX_STANDARD 17)

add_executable(uppexo_demo ${demo})
target_link_libraries(uppexo_demo ${Boost_LIBRARY} ${Vulkan_LIBRARY} glfw uppexo)
set_property(TARGET uppexo_demo PROPERTY CXX_STANDARD 17)
