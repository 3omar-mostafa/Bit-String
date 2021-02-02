cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME Bit_String)

project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES_LIST ${PROJECT_NAME}/*.h)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_SOURCE_DIR}/Bit_String)

set(PROJECT_TEST_EXECUTABLE test_bit_string)
add_executable(${PROJECT_TEST_EXECUTABLE} test.cpp ${SOURCE_FILES_LIST})

########################################### For Visual Studio ###########################################

# Generate Folder Hierarchy instead of adding all files in the same folder
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES_LIST})

# Group Cmake predefined projects in CMakePredefinedTargets folder (as ZERO_CHECK , ALL_BUILD)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the test executable project as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_TEST_EXECUTABLE})