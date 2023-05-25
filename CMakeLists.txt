cmake_minimum_required(VERSION 3.3)
project(uppexo DESCRIPTION "General purpose vulkan engine")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_BUILD_PARALLEL_LEVEL 8)

find_program(GLSLC_EXECUTABLE glslc)
if(NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "Could not find glslc")
endif()

find_program(BIN2C_EXECUTABLE bin2c)
if(NOT BIN2C_EXECUTABLE)
    message(FATAL_ERROR "Could not find bin2c")
endif()

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED)
include_directories(${library_INCLUDE_DIRS} include/ third_party/)

file(GLOB_RECURSE srcs src/*.cpp)
file(GLOB demo demo/*.cpp)
file(GLOB tests tests/*.cpp)
file(GLOB_RECURSE GLSL_SOURCE_FILES demo/shader/*.frag demo/shader/*.vert demo/shader/*.comp)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME_WE)
  set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
  set(INCLUDE_HEAD "${PROJECT_BINARY_DIR}/include/generated/${FILE_NAME}.h")
  add_custom_command(
    OUTPUT ${SPIRV} ${INCLUDE_HEAD}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
    COMMAND ${GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
    COMMAND ${BIN2C_EXECUTABLE} -n ${FILE_NAME} -o ${INCLUDE_HEAD} ${SPIRV} 
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
  list(APPEND SPIRV_HEADER_FILES ${INCLUDE_HEAD})
endforeach(GLSL)

add_custom_target(Shaders DEPENDS ${SPIRV_BINARY_FILES} ${SPIRV_HEADER_FILES})

add_subdirectory(third_party/obj)
add_library(uppexo SHARED ${srcs})
set_property(TARGET uppexo PROPERTY CXX_STANDARD 17)

add_executable(uppexo_demo ${demo})
target_link_libraries(uppexo_demo ${Boost_LIBRARY} ${Vulkan_LIBRARY} glfw uppexo tinyobjloader glm)
set_property(TARGET uppexo_demo PROPERTY CXX_STANDARD 17)
add_dependencies(uppexo_demo Shaders)

enable_testing()

foreach(test ${tests})
  get_filename_component(test_name ${test} NAME_WE)
  add_executable(${test_name} ${test})
  target_link_libraries(${test_name} ${Boost_LIBRARY} ${Vulkan_LIBRARY} glfw uppexo tinyobjloader glm)
  set_property(TARGET ${test_name} PROPERTY CXX_STANDARD 17)
  add_dependencies(${test_name} Shaders)
  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
endforeach()


