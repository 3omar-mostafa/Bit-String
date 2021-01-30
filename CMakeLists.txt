cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME Bit_String)

project(${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES_LIST ${PROJECT_NAME}/*.h)

add_executable(test_bit_string test.cpp ${SOURCE_FILES_LIST})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES_LIST})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

########################################### For Visual Studio ###########################################

# Generate Folder Hierarchy instead of adding all files in the same folder
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES_LIST})

# Group Cmake predefined projects in CMakePredefinedTargets folder (as ZERO_CHECK , ALL_BUILD)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
